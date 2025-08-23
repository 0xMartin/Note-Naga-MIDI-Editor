#pragma once

#include <note_naga_engine/note_naga_engine.h>
#include <note_naga_engine/synth/synth_external_midi.h>
#include <note_naga_engine/synth/synth_fluidsynth.h>

#include <QComboBox>
#include <QDialog>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

/**
 * @brief Dialog for configuring synthesizers in the mixer
 *
 * This dialog allows users to:
 * - Add and remove synthesizers
 * - Configure FluidSynth soundfonts
 * - Configure External MIDI ports
 */
class MixerSettingsDialog : public QDialog {
  Q_OBJECT

public:
  /**
   * @brief Constructor
   * @param engine Pointer to the Note Naga Engine
   * @param parent Parent widget
   */
  MixerSettingsDialog(NoteNagaEngine *engine, QWidget *parent = nullptr);

  /**
   * @brief Destructor
   */
  ~MixerSettingsDialog();

private slots:
  /**
   * @brief Add a new synthesizer to the engine
   */
  void addSynthesizer();

  /**
   * @brief Remove the selected synthesizer from the engine
   */
  void removeSynthesizer();

  /**
   * @brief Update the dialog when a synthesizer is selected in the list
   * @param index Selected index in the list
   */
  void onSynthesizerSelected(int index);

  /**
   * @brief Browse for a SoundFont file for FluidSynth
   */
  void browseSoundFont();

  /**
   * @brief Apply changes to the FluidSynth synthesizer
   */
  void applyFluidSynthChanges();

  /**
   * @brief Apply changes to the External MIDI synthesizer
   */
  void applyExternalMidiChanges();

  /**
   * @brief Refresh the list of available MIDI ports
   */
  void refreshMidiPorts();

private:
  /**
   * @brief Set up the UI components
   */
  void setupUI();

  /**
   * @brief Populate the synthesizer list
   */
  void populateSynthesizerList();

  /**
   * @brief Update the visibility of configuration controls based on selected
   * synthesizer type
   */
  void updateConfigurationVisibility();

  // Main UI components
  QListWidget *synthListWidget;
  QComboBox *synthTypeComboBox;
  QPushButton *addButton;
  QPushButton *removeButton;
  QPushButton *closeButton;

  // FluidSynth configuration
  QGroupBox *fluidSynthGroup;
  QLineEdit *soundFontPathEdit;
  QPushButton *browseSoundFontButton;
  QPushButton *applyFluidSynthButton;

  // External MIDI configuration
  QGroupBox *externalMidiGroup;
  QComboBox *midiPortComboBox;
  QPushButton *refreshPortsButton;
  QPushButton *applyExternalMidiButton;

  // Engine reference
  NoteNagaEngine *engine;
};