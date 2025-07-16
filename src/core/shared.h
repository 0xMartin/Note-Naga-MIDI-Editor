#pragma once

#include <QString>
#include <QColor>
#include <vector>
#include <optional>

// ---------- Channel colors ----------
extern const std::vector<QColor> DEFAULT_CHANNEL_COLORS;

// ---------- GM Instruments ----------
struct GMInstrument {
    int index;
    QString name;
    QString icon;
};
extern const std::vector<GMInstrument> GM_INSTRUMENTS;

// ---------- Note names ----------
extern const std::vector<QString> NOTE_NAMES;

// ---------- MidiNote ----------
struct MidiNote {
    int note;
    std::optional<int> start;
    std::optional<int> length;
    std::optional<int> channel;
    std::optional<int> velocity;
    std::optional<int> track;
    int note_id;

    MidiNote(): note(0), start(std::nullopt), length(std::nullopt), 
        channel(std::nullopt), velocity(std::nullopt), 
        track(std::nullopt), note_id(rand()) {}

    MidiNote(int note_, 
             std::optional<int> start_ = std::nullopt, 
             std::optional<int> length_ = std::nullopt, 
             std::optional<int> channel_ = std::nullopt, 
             std::optional<int> velocity_ = std::nullopt, 
             std::optional<int> track_ = std::nullopt)
        : note(note_), start(start_), length(length_), channel(channel_), 
        velocity(velocity_), track(track_), note_id(rand()) {}
};

// ---------- Utility functions ----------
QString note_name(int n);
int index_in_octave(int n);
double note_time_ms(const MidiNote& note, int ppq, int tempo);
std::optional<GMInstrument> find_instrument_by_name(const QString& name);
std::optional<GMInstrument> find_instrument_by_index(int index);