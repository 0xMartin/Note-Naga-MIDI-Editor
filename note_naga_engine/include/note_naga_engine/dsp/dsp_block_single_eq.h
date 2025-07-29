#pragma once

#include <note_naga_engine/core/dsp_block_base.h>
#include <cmath>

/**
 * @brief DSP Block for a single band EQ effect (peak filter, biquad).
 */
class DSPBlockSingleEQ : public NoteNagaDSPBlockBase {
public:
    /**
     * @brief Constructor for the single band EQ block.
     *
     * @param freq The center frequency in Hz.
     * @param gain The gain in dB.
     * @param q The Q factor (quality factor).
     */
    DSPBlockSingleEQ(float freq, float gain, float q);

    void process(float* left, float* right, size_t numFrames) override;

    std::vector<DSPParamDescriptor> getParamDescriptors() override;
    float getParamValue(size_t idx) const override;
    void setParamValue(size_t idx, float value) override;
    std::string getBlockName() const override { return "Single EQ"; }

    // Recalculate biquad coefficients
    void recalcCoeffs();

private:
    float freq_ = 1000.0f;
    float gain_ = 0.0f;
    float q_ = 1.0f;

    // DSP biquad state and coeffs
    float a0_ = 1.0f, a1_ = 0.0f, a2_ = 0.0f, b1_ = 0.0f, b2_ = 0.0f;
    float z1l_ = 0.0f, z2l_ = 0.0f, z1r_ = 0.0f, z2r_ = 0.0f;
    float sampleRate_ = 44100.0f; // TODO: make configurable
};