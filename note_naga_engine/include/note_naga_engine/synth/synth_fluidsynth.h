#pragma once

#include <note_naga_engine/core/note_naga_synthesizer.h>
#include <note_naga_engine/core/types.h>
#include <fluidsynth.h>
#include <string>

class DSPEngine;

/**
 * FluidSynth syntetizér pro NoteNagaEngine.
 */
class NoteNagaSynthFluidSynth : public NoteNagaSynthesizer, public INoteNagaSoftSynth {
public:
    /**
     * @brief Konstruktor FluidSynth syntetizéru
     * @param name Název syntetizéru
     * @param sf2_path Cesta k SoundFont souboru (.sf2 nebo .sf3)
     */
    NoteNagaSynthFluidSynth(const std::string &name, const std::string &sf2_path);
    ~NoteNagaSynthFluidSynth() override;

    virtual void playNote(const NN_Note_t &note, int channel = 0, float pan = 0.0) override;
    virtual void stopNote(const NN_Note_t &note) override;
    virtual void stopAllNotes(NoteNagaMidiSeq *seq = nullptr, NoteNagaTrack *track = nullptr) override;
    virtual void renderAudio(float* left, float* right, size_t num_frames) override;

    virtual std::string getConfig(const std::string &key) const override;
    virtual bool setConfig(const std::string &key, const std::string &value) override;
    virtual std::vector<std::string> getSupportedConfigKeys() const override;

    /**
     * @brief Get the current SoundFont path
     * @return The path to the currently loaded SoundFont file
     */
    std::string getSoundFontPath() const { return sf2_path_; }

    /**
     * @brief Change the SoundFont file at runtime
     * @param sf2_path Path to the new SoundFont file
     * @return True if the SoundFont was successfully loaded
     */
    bool setSoundFont(const std::string &sf2_path);

protected:
    void ensureFluidsynth();

    // FluidSynth interní struktury
    fluid_settings_t *synth_settings_;
    fluid_synth_t *fluidsynth_;

    // Store the current SoundFont path
    std::string sf2_path_;  
};