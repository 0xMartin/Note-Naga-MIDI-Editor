#include <note_naga_engine/module/audio_worker.h>

#include <cstring>

NoteNagaAudioWorker::NoteNagaAudioWorker(NoteNagaDSPEngine *dsp) {
    this->setDSPEngine(dsp);
    NOTE_NAGA_LOG_INFO("Audio worker initialized");
}

NoteNagaAudioWorker::~NoteNagaAudioWorker() { stop(); }

void NoteNagaAudioWorker::setDSPEngine(NoteNagaDSPEngine *dsp) { this->dsp_engine = dsp; }

bool NoteNagaAudioWorker::start(unsigned int sampleRate, unsigned int blockSize) {
    if (this->stream_open) {
        NOTE_NAGA_LOG_WARNING("Audio worker is already running");
        return false;
    }

    this->sample_rate = sampleRate;
    this->block_size = blockSize;

    RtAudio::StreamParameters params;
    params.deviceId = this->audio.getDefaultOutputDevice();
    params.nChannels = 2;
    params.firstChannel = 0;

    this->audio.openStream(&params, nullptr, RTAUDIO_FLOAT32, this->sample_rate, &this->block_size,
                      &NoteNagaAudioWorker::audioCallback, this);
    this->audio.startStream();
    this->stream_open = true;

    NOTE_NAGA_LOG_INFO("Audio worker started");
    return true;
}

bool NoteNagaAudioWorker::stop() {
    if (this->stream_open) {
        try {
            if (this->audio.isStreamRunning()) {
                this->audio.stopStream();
                NOTE_NAGA_LOG_INFO("Audio stream stopped");
            } else {
                NOTE_NAGA_LOG_WARNING("Audio stream was not running");
            }
            if (this->audio.isStreamOpen()) {
                this->audio.closeStream();
                NOTE_NAGA_LOG_INFO("Audio stream closed");
            } else {
                NOTE_NAGA_LOG_WARNING("Audio stream was not running or already closed");
            }
        } catch (const std::exception &e) { NOTE_NAGA_LOG_ERROR(e.what()); }
        this->stream_open = false;
        NOTE_NAGA_LOG_INFO("Audio worker stopped");
        return true;
    } else {
        NOTE_NAGA_LOG_WARNING("Audio worker is not running");
    }

    return false;
}

int NoteNagaAudioWorker::audioCallback(void *outputBuffer, void *, unsigned int nFrames, double,
                                       RtAudioStreamStatus, void *userData) {
    NoteNagaAudioWorker *self = static_cast<NoteNagaAudioWorker *>(userData);
    float *out = static_cast<float *>(outputBuffer);

    // Zkontrolujeme, zda je worker ztlumený (muted) nebo nemá DSP engine.
    // .load() bezpečně přečte hodnotu z atomické proměnné.
    if (self->is_muted.load(std::memory_order_relaxed) || !self->dsp_engine) {
        // Pokud ano, vyplníme buffer tichem (nulami).
        std::memset(out, 0, sizeof(float) * nFrames * 2); // 2 pro stereo
    } else {
        // Pokud není ztlumený, renderujeme normálně.
        self->dsp_engine->render(out, nFrames, true);
    }
    return 0;
}

void NoteNagaAudioWorker::mute() {
    this->is_muted.store(true, std::memory_order_relaxed);
}

void NoteNagaAudioWorker::unmute() {
    this->is_muted.store(false, std::memory_order_relaxed);
}

bool NoteNagaAudioWorker::isMuted() const {
    return this->is_muted.load(std::memory_order_relaxed);
}