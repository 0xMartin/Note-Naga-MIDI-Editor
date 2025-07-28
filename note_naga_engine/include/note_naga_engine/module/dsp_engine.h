#pragma once

#include <note_naga_engine/core/types.h>
#include <note_naga_engine/core/note_naga_synthesizer.h>
#include <note_naga_engine/module/metronome.h>
#include <note_naga_engine/core/project_data.h>
#include <vector>
#include <mutex>

class NoteNagaDSPEngine {
public:
    NoteNagaDSPEngine(NoteNagaProject* project);

    void addSynth(INoteNagaSoftSynth* synth);
    void removeSynth(INoteNagaSoftSynth* synth);

    // Main mix/render function called from AudioWorker for each block
    void renderBlock(float* output, size_t num_frames);

    // Metronome API
    NoteNagaMetronome* metronome() { return &metronome_; }

private:
    NoteNagaProject *project_;
    std::vector<INoteNagaSoftSynth*> synths_;
    std::mutex synths_mutex_;

    // Mixing buffers (always resized as needed)
    std::vector<float> mix_left_;
    std::vector<float> mix_right_;

    NoteNagaMetronome metronome_;
};