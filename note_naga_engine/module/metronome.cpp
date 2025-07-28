#include <note_naga_engine/module/metronome.h>

#include <deque>
#include <cmath>
#include <cstring>

struct RunningClick {
    int samplePos;  // kde v bloku začal
    int age;        // kolik sample už hrál
    bool accent;
};

inline float metronomeTickSample(bool accent, int sampleIdx, int tickLen, int sampleRate) {
    const float freq = accent ? 3500.0f : 2200.0f;
    const float amp  = accent ? 1.0f : 0.7f;
    // 1 ms tick při 44.1kHz = ~44 sample, 2ms = 88 sample
    const float env = amp * std::exp(-8.0f * float(sampleIdx) / float(tickLen));
    return env * std::sin(2.0f * M_PI * freq * float(sampleIdx) / float(sampleRate));
}


NoteNagaMetronome::NoteNagaMetronome()
    : project_(nullptr), enabled_(false), sampleRate_(44100),
      lastTick_(-1), phase_(0), tickSample_(0), ticksPerBeat_(4), tickLength_(200)
{
}

void NoteNagaMetronome::render(float* left, float* right, size_t numFrames) {
    if (!enabled_ || !project_) return;

    int ppq = project_->getPPQ();
    if (ppq <= 0) ppq = 480;

    double us_per_qn = double(project_->getTempo());
    double bpm = 60'000'000.0 / us_per_qn;
    if (bpm <= 0) bpm = 120.0;

    double sec_per_tick = (us_per_qn / 1e6) / double(ppq);
    double samples_per_tick = sec_per_tick * double(sampleRate_);

    int ticks_per_metronome = ppq / ticksPerBeat_;
    int current_tick = project_->getCurrentTick();

    double tick_at_sample0 = double(current_tick);
    double tick_at_sampleN = tick_at_sample0 + double(numFrames) / samples_per_tick;

    int first_metro_tick = int(std::ceil(tick_at_sample0 / ticks_per_metronome)) * ticks_per_metronome;
    if (first_metro_tick < tick_at_sample0) first_metro_tick += ticks_per_metronome;

    // --- Udržuj queue běžících kliků (přes bloky) ---
    static std::deque<RunningClick> runningClicks;

    // Přidej nové kliky z tohoto bloku
    for (int metro_tick = first_metro_tick; double(metro_tick) < tick_at_sampleN; metro_tick += ticks_per_metronome) {
        double tick_offset = double(metro_tick) - tick_at_sample0;
        int sample_offset = int(std::round(tick_offset * samples_per_tick));
        if (sample_offset < 0 || sample_offset >= int(numFrames)) continue;
        bool accent = ((metro_tick / ticks_per_metronome) % ticksPerBeat_ == 0);
        runningClicks.push_back({sample_offset, 0, accent});
    }

    // --- Pro každý sample, mixni všechny běžící kliky ---
    const int tickLen = int(sampleRate_ * 0.002); // 2 ms
    for (size_t i = 0; i < numFrames; ++i) {
        for (auto& click : runningClicks) {
            int sample_in_tick = int(i) - click.samplePos + click.age;
            if (sample_in_tick >= 0 && sample_in_tick < tickLen) {
                float val = metronomeTickSample(click.accent, sample_in_tick, tickLen, sampleRate_);
                left[i] += val;
                right[i] += val;
            }
        }
    }

    // --- Po bloku aktualizuj "age" všech kliků ---
    for (auto& click : runningClicks) click.age += int(numFrames);

    // --- Smaž kliky, které už dozněly ---
    while (!runningClicks.empty() && runningClicks.front().age >= tickLen)
        runningClicks.pop_front();
}