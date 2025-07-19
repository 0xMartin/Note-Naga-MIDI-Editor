#pragma once

#include <QObject>
#include <QString>
#include <QColor>
#include <vector>
#include <optional>

// comment to disable Qt support
#define NN_QT_ENABLED

// --- Macro for emitting signals depending on NN_QT_EMIT_ENABLED ---
#ifdef NN_QT_ENABLED
#define NN_QT_EMIT(X) emit X
#else
#define NN_QT_EMIT(X)
#endif

// ---------- Channel colors ----------
extern const std::vector<QColor> DEFAULT_CHANNEL_COLORS;

QColor color_blend(const QColor& fg, const QColor& bg, double opacity);

// ---------- GM Instruments ----------
struct GMInstrument
{
    int index;
    QString name;
    QString icon;
};
extern const std::vector<GMInstrument> GM_INSTRUMENTS;

// ---------- Note names ----------
extern const std::vector<QString> NOTE_NAMES;

// ---------- MidiNote ----------
struct NoteNagaNote
{
    // Required for unique identification
    int note_id;

    // Note id (0-127)
    int note;

    // Optional properties
    std::optional<int> start;
    std::optional<int> length;
    std::optional<int> velocity;
    
    NoteNagaNote() : note(0), start(std::nullopt), length(std::nullopt),
                 velocity(std::nullopt), note_id(rand()) {}

    NoteNagaNote(int note_,
             std::optional<int> start_ = std::nullopt,
             std::optional<int> length_ = std::nullopt,
             std::optional<int> velocity_ = std::nullopt,
             std::optional<int> track_ = std::nullopt)
        : note(note_), start(start_), length(length_),
          velocity(velocity_), note_id(rand()) {}
};

// ---------- Track ----------
class NoteNagaTrack : public QObject
{
    Q_OBJECT
public:
    NoteNagaTrack();

    NoteNagaTrack(int track_id,
          const QString &name,
          std::optional<int> instrument,
          std::optional<int> channel);

    std::vector<NoteNagaNote> get_notes() const { return midi_notes; }
    std::optional<int> get_instrument() const { return instrument; }
    std::optional<int> get_channel() const { return channel; }     
    int get_id() const { return track_id; }
    const QString& get_name() const { return name; }
    const QColor& get_color() const { return color; }
    bool is_visible() const { return visible; }
    bool is_muted() const { return muted; }
    bool is_solo() const { return solo; }
    float get_volume() const { return volume; }

    void set_notes(const std::vector<NoteNagaNote>& notes);
    void set_instrument(std::optional<int> instrument);
    void set_channel(std::optional<int> channel);
    void set_id(int new_id);
    void set_name(const QString &new_name);
    void set_color(const QColor &new_color);
    void set_visible(bool is_visible);
    void set_muted(bool is_muted);
    void set_solo(bool is_solo);
    void set_volume(float new_volume);

Q_SIGNALS:
    void meta_changed_signal(int track_id, const QString& param);

protected:
    // META data
    int track_id;
    std::optional<int> instrument;
    std::optional<int> channel;
    QString name;
    QColor color;
    bool visible;
    bool muted;
    bool solo;
    float volume;

    // DATA
    std::vector<NoteNagaNote> midi_notes;
};

// ---------- Utility functions ----------
QString note_name(int n);
int index_in_octave(int n);
double note_time_ms(const NoteNagaNote &note, int ppq, int tempo);
std::optional<GMInstrument> find_instrument_by_name(const QString &name);
std::optional<GMInstrument> find_instrument_by_index(int index);