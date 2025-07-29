#include <note_naga_engine/dsp/dsp_block_pan.h>

#include <cmath>

void DSPBlockPan::process(float* left, float* right, size_t numFrames) {
    if (!isActive()) return;
    float leftGain = std::cos(0.25f * M_PI * (pan_ + 1.0f));
    float rightGain = std::sin(0.25f * M_PI * (pan_ + 1.0f));
    for (size_t i = 0; i < numFrames; ++i) {
        float l = left[i], r = right[i];
        left[i] = l * leftGain;
        right[i] = r * rightGain;
    }
}

std::vector<DSPParamDescriptor> DSPBlockPan::getParamDescriptors() {
    return { DSPParamDescriptor{ "Pan", DSPParamType::Float, DSControlType::DialCentered, -1.0f, 1.0f, 0.0f } };
}
float DSPBlockPan::getParamValue(size_t idx) const { return idx == 0 ? pan_ : 0.0f; }
void DSPBlockPan::setParamValue(size_t idx, float value) { if (idx == 0) pan_ = value; }