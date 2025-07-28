#pragma once

#include <note_naga_engine/core/note_naga_component.h>

#include <note_naga_engine/core/audio_buffer.h>
#include <vector>
#include <mutex>

struct MixedBuffer {
    std::vector<float> left;
    std::vector<float> right;
    size_t frames;
};

class AudioWorker; // forward

class DSPEngine : public NoteNagaEngineComponent<NN_AudioBuffer_t, 256> {
public:
    DSPEngine();
    void setAudioWorker(AudioWorker* w);

protected:
    void onItem(const NN_AudioBuffer_t& buffer) override;

    // Mixovací buffer (interní fronta, thread-safe pokud potřeba)
    std::mutex mix_mutex;
    MixedBuffer mixed;
    AudioWorker* audioWorker = nullptr;

    void mixAudio(const NN_AudioBuffer_t& buffer);
    void flushMixedBuffer(); // až je buffer plný, pushne se do audioWorker
    static constexpr size_t BLOCK_SIZE = 512; // velikost výstupního bloku
};