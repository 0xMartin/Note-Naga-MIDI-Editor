#include <note_naga_engine/dsp/dsp_block_gain.h>

void DSPBlockGain::process(float *left, float *right, size_t numFrames) {
    if (!isActive()) return;
    if (gain_ == 0.0f) return; // No change needed
    float appliedGain = powf(10.0f, gain_);
    for (size_t i = 0; i < numFrames; ++i) {
        left[i] *= appliedGain;
        right[i] *= appliedGain;
    }
}

std::vector<DSPParamDescriptor> DSPBlockGain::getParamDescriptors() {
    std::vector<DSPParamDescriptor> params;
    params.push_back(DSPParamDescriptor{"Gain", DSPParamType::Float, DSControlType::SliderVertical, -2.0f, 2.0f, 0.0f});
    return params;
}

float DSPBlockGain::getParamValue(size_t idx) const {
    if (idx == 0) return gain_;
    return 0.0f;
}

void DSPBlockGain::setParamValue(size_t idx, float value) {
    if (idx == 0) gain_ = value;
}