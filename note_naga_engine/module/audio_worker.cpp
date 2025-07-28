#include <note_naga_engine/module/audio_worker.h>

#include <RtAudio.h>
#include <algorithm>
#include <cstring>

// Jednoduchý přehrávač s pomocí RtAudio
AudioWorker::AudioWorker() {}

void AudioWorker::onItem(const MixedBuffer& buffer) {
    // Implementace: pošli data na výstup (např. přes RtAudio)
    // (Inicializace RtAudio a streamu je na tobě, zde jen zápis do streamu)

    // Ukázka: rtaudioStream.write(audio, nFrames)
    // float* interleaved = ... // vytvoř interleaved stereo buffer
    std::vector<float> interleaved(buffer.frames * 2);
    for (size_t i = 0; i < buffer.frames; ++i) {
        interleaved[2 * i] = buffer.left[i];
        interleaved[2 * i + 1] = buffer.right[i];
    }
    // rtaudio_stream.write(interleaved.data(), buffer.frames);
    // (Ošetři případný underrun, případně blokuj na buffer-full)
}