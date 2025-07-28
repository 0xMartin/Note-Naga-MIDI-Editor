#pragma once

#include <vector>
#include <cstddef>
#include <memory>

/**
 * @brief Audio buffer structure for storing audio data.
 *
 * This structure is used to hold audio samples, their frame count, and channel information.
 * It can be extended in the future to include more metadata or processing information.
 */
struct NN_AudioBuffer_t {
    std::vector<float> data; ///< Audio sample data, interleaved for stereo (left, right, left, right, ...)
    size_t frames = 0;    /// Number of frames in the audio buffer (samples per channel)>
    bool left_channel;    /// True if the first channel is left, false if right>

    int synth_id = -1;    /// Optional, for future routing
    uint64_t audio_time = 0; /// Optional, for future routing
};