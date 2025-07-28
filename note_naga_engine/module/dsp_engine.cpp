#include <note_naga_engine/module/dsp_engine.h>

#include "audio_worker.h"

DSPEngine::DSPEngine() {
    mixed.left.resize(BLOCK_SIZE, 0.0f);
    mixed.right.resize(BLOCK_SIZE, 0.0f);
    mixed.frames = 0;
}
void DSPEngine::setAudioWorker(AudioWorker* w) {
    audioWorker = w;
}

// Mixuje přicházející buffer do interního mix bufferu
void DSPEngine::onItem(const NN_AudioBuffer_t& buffer) {
    std::lock_guard<std::mutex> lock(mix_mutex);

    // Pokud je nový blok, vynuluj (nebo podle timestamp)
    if (mixed.frames == 0) {
        std::fill(mixed.left.begin(), mixed.left.end(), 0.0f);
        std::fill(mixed.right.begin(), mixed.right.end(), 0.0f);
    }

    // Mixuj podle kanálu
    for (size_t i = 0; i < buffer.frames && i < BLOCK_SIZE; ++i) {
        if (buffer.left_channel) {
            mixed.left[i] += buffer.data[i];
        } else {
            mixed.right[i] += buffer.data[i];
        }
    }
    mixed.frames = std::max(mixed.frames, buffer.frames);

    // Pokud je mixovací buffer plný, push do audioWorker
    if (mixed.frames >= BLOCK_SIZE && audioWorker) {
        audioWorker->pushToQueue(mixed); // předáváme kopii
        mixed.frames = 0;
    }
}