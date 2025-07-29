#include "dsp_widget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QIcon>
#include <QFrame>
#include <QSlider>
#include <QSpacerItem>
#include "../nn_gui_utils.h"

DSPWidget::DSPWidget(NoteNagaEngine *engine, QWidget *parent)
    : QWidget(parent)
{
    this->engine = engine;
    this->title_widget = nullptr;
    initTitleUI();
    initUI();
}

void DSPWidget::initTitleUI()
{
    // Vertikální panel s tlačítky vlevo
    if (this->title_widget) return;
    this->title_widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(title_widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QPushButton *btn_add = create_small_button(":/icons/add.svg", "Add DSP module", "btn_add");
    QPushButton *btn_remove = create_small_button(":/icons/remove.svg", "Remove selected DSP", "btn_remove");
    QPushButton *btn_clear = create_small_button(":/icons/clear.svg", "Remove all DSP modules", "btn_clear");

    layout->addWidget(btn_add, 0, Qt::AlignBottom | Qt::AlignHCenter);
    layout->addWidget(btn_remove, 0, Qt::AlignBottom | Qt::AlignHCenter);
    layout->addWidget(btn_clear, 0, Qt::AlignBottom | Qt::AlignHCenter);

    connect(btn_add, &QPushButton::clicked, this, &DSPWidget::addDSPClicked);
    connect(btn_remove, &QPushButton::clicked, this, &DSPWidget::removeDSPClicked);
    connect(btn_clear, &QPushButton::clicked, this, &DSPWidget::removeAllDSPClicked);
}

void DSPWidget::initUI()
{
    main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(5, 5, 5, 5);
    main_layout->setSpacing(0);

    // Vertikální header vlevo
    main_layout->addWidget(title_widget, 0);

    // Horizontální scrollovací prostor pro DSP moduly
    dsp_container = new QWidget();
    dsp_layout = new QHBoxLayout(dsp_container);
    dsp_layout->setContentsMargins(0, 0, 0, 0);
    dsp_layout->setSpacing(8);
    dsp_layout->addStretch(1);

    dsp_scroll_area = new QScrollArea();
    dsp_scroll_area->setWidgetResizable(true);
    dsp_scroll_area->setFrameShape(QFrame::NoFrame);
    dsp_scroll_area->setStyleSheet("QScrollArea { background: transparent; padding: 0px; }");
    dsp_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    dsp_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dsp_scroll_area->setWidget(dsp_container);

    main_layout->addWidget(dsp_scroll_area, 1);

    // Pravý info panel s volume bary
    info_panel = new QWidget();
    info_panel->setFixedWidth(60);
    QVBoxLayout *info_layout = new QVBoxLayout(info_panel);
    info_layout->setContentsMargins(8, 8, 8, 8);
    info_layout->setSpacing(8);

    QLabel *lbl_info = new QLabel("Output");
    lbl_info->setAlignment(Qt::AlignCenter);
    lbl_info->setStyleSheet("font-size: 13px; color: #ccc;");

    QSlider *volume_left = new QSlider(Qt::Vertical);
    volume_left->setRange(0, 100);
    volume_left->setValue(80);
    volume_left->setStyleSheet("QSlider { height: 60px; }");
    QLabel *lbl_left = new QLabel("L");
    lbl_left->setAlignment(Qt::AlignCenter);

    QSlider *volume_right = new QSlider(Qt::Vertical);
    volume_right->setRange(0, 100);
    volume_right->setValue(80);
    volume_right->setStyleSheet("QSlider { height: 60px; }");
    QLabel *lbl_right = new QLabel("R");
    lbl_right->setAlignment(Qt::AlignCenter);

    info_layout->addWidget(lbl_info);
    info_layout->addWidget(volume_left, 1);
    info_layout->addWidget(lbl_left);
    info_layout->addWidget(volume_right, 1);
    info_layout->addWidget(lbl_right);
    info_layout->addStretch(1);

    main_layout->addWidget(info_panel, 0);
    setLayout(main_layout);
}