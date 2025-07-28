#include <note_naga_engine/module/dsp_engine.h>

#include <algorithm>
#include <cstring>

NoteNagaDSPEngine::NoteNagaDSPEngine(NoteNagaProject *project){
    this->project_ = project;
    metronome_.setProject(project);
    metronome_.setSampleRate(44100);
    NOTE_NAGA_LOG_INFO("DSP Engine initialized");
}

void NoteNagaDSPEngine::addSynth(INoteNagaSoftSynth* synth) {
    std::lock_guard<std::mutex> lock(synths_mutex_);
    synths_.push_back(synth);
}

void NoteNagaDSPEngine::removeSynth(INoteNagaSoftSynth* synth) {
    std::lock_guard<std::mutex> lock(synths_mutex_);
    synths_.erase(std::remove(synths_.begin(), synths_.end(), synth), synths_.end());
}

void NoteNagaDSPEngine::renderBlock(float* output, size_t num_frames) {
    // Prepare mix buffers
    if (mix_left_.size() < num_frames) mix_left_.resize(num_frames, 0.0f);
    if (mix_right_.size() < num_frames) mix_right_.resize(num_frames, 0.0f);
    std::fill(mix_left_.begin(), mix_left_.begin() + num_frames, 0.0f);
    std::fill(mix_right_.begin(), mix_right_.begin() + num_frames, 0.0f);

    // Render all synths and mix
    std::lock_guard<std::mutex> lock(synths_mutex_);
    for (auto* synth : synths_) {
        synth->renderAudio(mix_left_.data(), mix_right_.data(), num_frames);
    }

    // Metronome rendering
    metronome_.render(mix_left_.data(), mix_right_.data(), num_frames);

    // Interleave and write to output buffer efficiently
    float* left = mix_left_.data();
    float* right = mix_right_.data();
    float* out = output;
    for (size_t i = 0; i < num_frames; ++i) {
        *out++ = left[i];
        *out++ = right[i];
    }
}
