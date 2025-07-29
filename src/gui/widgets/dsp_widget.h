#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QIcon>
#include <QSpacerItem>
#include <QSlider>
#include <QList>
#include <vector>

#include <note_naga_engine/note_naga_engine.h>

/**
 * @brief DSPWidget provides a user interface for managing DSP modules in the application.
 * It includes a title bar with buttons for adding, removing, and clearing DSP modules,
 * and a scrollable area to display the DSP modules.
 */
class DSPWidget : public QWidget {
    Q_OBJECT
public:
    explicit DSPWidget(NoteNagaEngine *engine, QWidget *parent = nullptr);

    QWidget *getTitleWidget() const { return this->title_widget; }

private:
    NoteNagaEngine * engine;

    QWidget *title_widget;
    QWidget *dsp_container;
    QScrollArea *dsp_scroll_area;
    QWidget *info_panel;

    QHBoxLayout *main_layout;
    QHBoxLayout *dsp_layout;

    void initTitleUI();
    void initUI();

signals:
    void addDSPClicked();
    void removeDSPClicked();
    void removeAllDSPClicked();
};
