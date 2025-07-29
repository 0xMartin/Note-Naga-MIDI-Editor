#pragma once

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <vector>
#include <memory>

#include <note_naga_engine/core/dsp_block_base.h>
#include "../components/audio_dial.h"
#include "../components/audio_dial_centered.h"
#include "../components/audio_vertical_slider.h"

/**
 * @brief Widget for controlling a DSP block (auto-generates UI based on block parameters).
 */
class DSPBlockWidget : public QFrame {
    Q_OBJECT
public:
    explicit DSPBlockWidget(NoteNagaDSPBlockBase* block, QWidget* parent = nullptr);

    NoteNagaDSPBlockBase* block() const { return block_; }

    QSize minimumSizeHint() const override;

protected:
    void resizeEvent(QResizeEvent* event) override;

signals:
    void moveLeftRequested(DSPBlockWidget* widget);
    void moveRightRequested(DSPBlockWidget* widget);
    void deleteRequested(DSPBlockWidget* widget);

private slots:
    void onLeftClicked();
    void onRightClicked();
    void onDeactivateClicked();
    void onDeleteClicked();

private:
    void buildUi();
    void updateActivationButton();
    void rebuildDialGrid();

    NoteNagaDSPBlockBase* block_;

    QVBoxLayout* mainLayout_;
    QFrame* topBar_;
    QWidget* buttonBar_;
    QHBoxLayout* buttonBarLayout_;
    QPushButton* rightBtn_;
    QPushButton* leftBtn_;
    QPushButton* deactivateBtn_;
    QPushButton* deleteBtn_;

    QWidget* centerWidget_;
    QHBoxLayout* centerLayout_;

    // Dial grid
    QWidget* dialGridWidget_;
    QGridLayout* dialGridLayout_;
    std::vector<QWidget*> dialWidgets_;

    // Vertical slider stack
    QWidget* vSliderWidget_;
    QVBoxLayout* vSliderLayout_;
    std::vector<AudioVerticalSlider*> vSliderWidgets_;

    // Param widget info
    struct ParamWidget {
        QWidget* control = nullptr;
        DSControlType control_type;
    };
    std::vector<ParamWidget> paramWidgets_;
};