#include <note_naga_engine/dsp/dsp_block_single_eq.h>

DSPBlockSingleEQ::DSPBlockSingleEQ(float freq, float gain, float q)
    : freq_(freq), gain_(gain), q_(q)
{
    recalcCoeffs();
}

void DSPBlockSingleEQ::recalcCoeffs() {
    // Peak EQ (biquad)
    float A = powf(10.0f, gain_ / 40.0f);
    float omega = 2.0f * float(M_PI) * freq_ / sampleRate_;
    float sn = sinf(omega);
    float cs = cosf(omega);
    float alpha = sn / (2.0f * q_);
    float beta = sqrtf(A) / q_;

    // Standard RBJ: https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
    float b0 = 1.0f + alpha * A;
    float b1 = -2.0f * cs;
    float b2 = 1.0f - alpha * A;
    float a0 = 1.0f + alpha / A;
    float a1 = -2.0f * cs;
    float a2 = 1.0f - alpha / A;

    // Normalize
    a0_ = b0 / a0;
    a1_ = b1 / a0;
    a2_ = b2 / a0;
    b1_ = a1 / a0;
    b2_ = a2 / a0;
}

void DSPBlockSingleEQ::process(float* left, float* right, size_t numFrames) {
    if (!isActive()) return;

    // If params changed, recalc coefficients
    recalcCoeffs();

    // Apply biquad filter to both channels
    for (size_t i = 0; i < numFrames; ++i) {
        // Left
        float inL = left[i];
        float outL = a0_ * inL + a1_ * z1l_ + a2_ * z2l_ - b1_ * z1l_ - b2_ * z2l_;
        left[i] = outL;
        z2l_ = z1l_;
        z1l_ = inL;

        // Right
        float inR = right[i];
        float outR = a0_ * inR + a1_ * z1r_ + a2_ * z2r_ - b1_ * z1r_ - b2_ * z2r_;
        right[i] = outR;
        z2r_ = z1r_;
        z1r_ = inR;
    }
}

std::vector<DSPParamDescriptor> DSPBlockSingleEQ::getParamDescriptors() {
    return {
        { "Freq", DSPParamType::Float, DSControlType::Dial, 20.0f, 20000.0f, 1000.0f },
        { "Gain", DSPParamType::Float, DSControlType::SliderVertical, -24.0f, 24.0f, 0.0f },
        { "Q",    DSPParamType::Float, DSControlType::Dial, 0.1f, 10.0f, 1.0f }
    };
}

float DSPBlockSingleEQ::getParamValue(size_t idx) const {
    if (idx == 0) return freq_;
    if (idx == 1) return gain_;
    if (idx == 2) return q_;
    return 0.0f;
}

void DSPBlockSingleEQ::setParamValue(size_t idx, float value) {
    if (idx == 0) freq_ = value;
    if (idx == 1) gain_ = value;
    if (idx == 2) q_ = value;
    recalcCoeffs();
}