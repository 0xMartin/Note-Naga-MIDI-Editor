#include "app_context.h"

#include <QFileInfo>
#include <QString>
#include <memory>
#include <iostream>
#include <algorithm>
#include <QDebug>

// ---------- TrackInfo ----------
TrackInfo::TrackInfo(int track_id,
                     const QString& name,
                     int instrument,
                     bool visible,
                     bool playing,
                     float volume,
                     QColor color)
    : track_id(track_id),
      name(name.isEmpty() ? QString("Track %1").arg(track_id + 1) : name),
      instrument(instrument),
      color(color.isValid() ? color : DEFAULT_CHANNEL_COLORS[track_id % DEFAULT_CHANNEL_COLORS.size()]),
      visible(visible),
      playing(playing),
      volume(volume)
{}

QVariantMap TrackInfo::to_dict() const {
    QVariantMap m;
    m["track_id"] = track_id;
    m["name"] = name;
    m["instrument"] = instrument;
    m["color"] = color;
    m["visible"] = visible;
    m["playing"] = playing;
    m["volume"] = volume;
    return m;
}

// ---------- AppContext implementation ----------
AppContext* AppContext::instance() {
    static AppContext* _instance = nullptr;
    if (!_instance) {
        _instance = new AppContext();
    }
    return _instance;
}

AppContext::AppContext(QObject* parent)
    : QObject(parent), ppq(480), tempo(500000), midi_file(nullptr), current_tick(0), max_tick(0)
{
    qDebug() << "AppContext: Initialized with default PPQ and tempo.";
}

void AppContext::clear() {
    std::cout << "AppContext: Clearing context" << std::endl;
    tracks.clear();
    ppq = 480;
    tempo = 500000;
    midi_file = nullptr;
    active_track_id.reset();
    current_tick = 0;
    max_tick = 0;
}

void AppContext::load_from_midi(const QString& midi_file_path) {
    if (midi_file_path.isEmpty()) {
        std::cout << "AppContext: No MIDI file path provided." << std::endl;
        return;
    }
    QFileInfo fi(midi_file_path);
    if (!fi.exists()) {
        std::cout << "AppContext: MIDI file " << midi_file_path.toStdString() << " does not exist." << std::endl;
        return;
    }

    std::cout << "AppContext: Loading MIDI file from " << midi_file_path.toStdString() << std::endl;
    clear();

    std::shared_ptr<MidiFile> midiFile = std::make_shared<MidiFile>();
    if (!midiFile->load(midi_file_path.toStdString())) {
        std::cout << "AppContext: Failed to load MIDI file." << std::endl;
        return;
    }
    this->midi_file = midiFile;
    this->ppq = midiFile->header.division;

    // Load all tracks
    std::vector<std::shared_ptr<TrackInfo>> tracks_tmp;
    for (int i = 0; i < midiFile->getNumTracks(); ++i) {
        const MidiTrack& track = midiFile->getTrack(i);
        int abs_time = 0;
        std::map<std::pair<int,int>, std::pair<int,int>> notes_on; // (note, channel) -> (start, velocity)
        int instrument = 0;
        QString name;
        std::vector<MidiNote> note_buffer;

        for (const auto& evt : track.events) {
            abs_time += evt.delta_time;

            if (evt.type == MidiEventType::Meta && evt.meta_type == 0x03) { // track_name
                name = QString::fromStdString(std::string(evt.meta_data.begin(), evt.meta_data.end()));
            }
            if (evt.type == MidiEventType::ProgramChange) {
                if (!evt.data.empty()) {
                    instrument = evt.data[0];
                }
            }
            if (evt.type == MidiEventType::Meta && evt.meta_type == 0x51 && i == 0) { // set_tempo
                if (evt.meta_data.size() == 3) {
                    tempo = (evt.meta_data[0] << 16) | (evt.meta_data[1] << 8) | evt.meta_data[2];
                }
            }
            if (evt.type == MidiEventType::NoteOn && !evt.data.empty() && evt.data[1] > 0) {
                int note = evt.data[0];
                int velocity = evt.data[1];
                int channel = evt.channel;
                notes_on[std::make_pair(note, channel)] = std::make_pair(abs_time, velocity);
            }
            else if ((evt.type == MidiEventType::NoteOff) || (evt.type == MidiEventType::NoteOn && !evt.data.empty() && evt.data[1] == 0)) {
                int note = evt.data[0];
                int channel = evt.channel;
                auto key = std::make_pair(note, channel);
                auto it = notes_on.find(key);
                if (it != notes_on.end()) {
                    int start = it->second.first;
                    int velocity = it->second.second;
                    note_buffer.push_back(MidiNote(note, start, abs_time - start, channel, velocity, i));
                    notes_on.erase(it);
                }
            }
        }

        auto track_info = std::make_shared<TrackInfo>(
            i,
            name.isEmpty() ? QString("Track %1").arg(i+1) : name,
            instrument,
            true,
            true,
            1.0,
            QColor() // set to default later
        );
        std::sort(note_buffer.begin(), note_buffer.end(), [](const MidiNote& a, const MidiNote& b) { return a.start < b.start; });
        track_info->midi_notes = note_buffer;
        tracks_tmp.push_back(track_info);
    }

    active_track_id = !tracks_tmp.empty() ? tracks_tmp[0]->track_id : -1;
    tracks = tracks_tmp;
    compute_max_tick();
    Q_EMIT midi_file_loaded_signal();
}

std::shared_ptr<TrackInfo> AppContext::get_track_by_id(int track_id) {
    auto it = std::find_if(tracks.begin(), tracks.end(), [track_id](const std::shared_ptr<TrackInfo>& tr) {
        return tr->track_id == track_id;
    });
    if (it != tracks.end()) return *it;
    return nullptr;
}

void AppContext::set_track_attribute(int track_id, const QString& attr, const QVariant& value) {
    auto tr = get_track_by_id(track_id);
    if (tr) {
        if (attr == "name") tr->name = value.toString();
        else if (attr == "instrument") tr->instrument = value.toInt();
        else if (attr == "color") tr->color = value.value<QColor>();
        else if (attr == "visible") tr->visible = value.toBool();
        else if (attr == "playing") tr->playing = value.toBool();
        else if (attr == "volume") tr->volume = value.toFloat();
    }
}

std::vector<QVariantMap> AppContext::get_track_dicts() const {
    std::vector<QVariantMap> out;
    for (const auto& tr : tracks) out.push_back(tr->to_dict());
    return out;
}

int AppContext::compute_max_tick() {
    max_tick = 0;
    for (const auto& track : tracks) {
        for (const auto& note : track->midi_notes) {
            if (note.start.has_value() && note.length.has_value())
                max_tick = std::max(max_tick, note.start.value() + note.length.value());
        }
    }
    return max_tick;
}