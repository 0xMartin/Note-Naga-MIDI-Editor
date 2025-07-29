#include "dsp_block_widget.h"
#include "../nn_gui_utils.h"
#include <QButtonGroup>
#include <QDebug>
#include <QIcon>
#include <QResizeEvent>
#include <QSpacerItem>
#include <cmath>

static constexpr int DIAL_MIN_WIDTH = 40;
static constexpr int DIAL_MIN_HEIGHT = 60;
static constexpr int VSLIDER_WIDTH = 30;
static constexpr int MAIN_PADDING = 4;
static constexpr int BUTTON_BAR_HEIGHT = 32;
static constexpr int TITLE_BAR_HEIGHT = 32;

DSPBlockWidget::DSPBlockWidget(NoteNagaDSPBlockBase *block, QWidget *parent)
    : QFrame(parent), block_(block), mainLayout_(nullptr), deactivateBtn_(nullptr),
      deleteBtn_(nullptr), buttonBar_(nullptr), buttonBarLayout_(nullptr), dialGridWidget_(nullptr),
      dialGridLayout_(nullptr), vSliderWidget_(nullptr), vSliderLayout_(nullptr) {
    setObjectName("DSPBlockWidget");
    setStyleSheet(R"(
        QFrame#DSPBlockWidget {
            background-color: #32353b;
            border: 1px solid #19191f;
            border-radius: 6px;
        }
    )");
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    buildUi();
}

void DSPBlockWidget::buildUi() {
    // --- Main layout ---
    if (mainLayout_) {
        delete mainLayout_;
        mainLayout_ = nullptr;
    }
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(0, 0, 0, 0); // NO padding around whole widget
    mainLayout_->setSpacing(0);

    ///////////////////////////////////////////////////////////////////////////////////////////
    // TITLE BAR
    ///////////////////////////////////////////////////////////////////////////////////////////
    topBar_ = new QFrame(this);
    topBar_->setObjectName("TopBar");
    topBar_->setFixedHeight(TITLE_BAR_HEIGHT);
    topBar_->setStyleSheet("QFrame#TopBar {"
                           "  background: #2b2f37;"
                           "  border-bottom: 1px solid #19191f;"
                           "  border-top-left-radius:6px;"
                           "  border-top-right-radius:6px;"
                           "  border-top: none;"
                           "  border-left: none;"
                           "  border-right: none;"
                           "}");
    auto *barLayout = new QHBoxLayout(topBar_);
    barLayout->setContentsMargins(12, 2, 12, 2); // no padding outside of title bar
    barLayout->setSpacing(3);
    auto *title = new QLabel(QString::fromStdString(block_->getBlockName()), topBar_);
    title->setStyleSheet("font-weight: bold; font-size: 15px; color: #fff;");
    barLayout->addWidget(title);
    barLayout->addStretch();

    leftBtn_ = create_small_button(":/icons/left.svg", "Move block left", "leftBtn", 22, topBar_);
    connect(leftBtn_, &QPushButton::clicked, this, &DSPBlockWidget::onLeftClicked);
    barLayout->addWidget(leftBtn_);

    rightBtn_ =
        create_small_button(":/icons/right.svg", "Move block right", "rightBtn", 22, topBar_);
    connect(rightBtn_, &QPushButton::clicked, this, &DSPBlockWidget::onRightClicked);
    barLayout->addWidget(rightBtn_);

    deactivateBtn_ = create_small_button(":/icons/active.svg",
                                         block_->isActive() ? "Deactivate block" : "Activate block",
                                         "deactivateBtn", 22, topBar_);
    deactivateBtn_->setCheckable(true);
    connect(deactivateBtn_, &QPushButton::clicked, this, &DSPBlockWidget::onDeactivateClicked);
    barLayout->addWidget(deactivateBtn_);

    deleteBtn_ = create_small_button(":/icons/close.svg", "Delete block", "deleteBtn", 22, topBar_);
    connect(deleteBtn_, &QPushButton::clicked, this, &DSPBlockWidget::onDeleteClicked);
    barLayout->addWidget(deleteBtn_);

    mainLayout_->addWidget(topBar_);

    ///////////////////////////////////////////////////////////////////////////////////////////
    // BUTTON BAR
    ///////////////////////////////////////////////////////////////////////////////////////////
    buttonBar_ = new QWidget(this);
    buttonBar_->setMinimumHeight(BUTTON_BAR_HEIGHT);
    buttonBarLayout_ = new QHBoxLayout(buttonBar_);
    buttonBarLayout_->setContentsMargins(4, 0, 4, 0);
    buttonBarLayout_->setSpacing(2);

    // Buttony generované z parametru
    auto params = block_->getParamDescriptors();
    paramWidgets_.clear();
    dialWidgets_.clear();
    vSliderWidgets_.clear();

    int buttonCount = 0;
    for (size_t i = 0; i < params.size(); ++i) {
        const auto &desc = params[i];
        QWidget *control = nullptr;
        switch (desc.control_type) {
        case DSControlType::PushButton: {
            auto *btn =
                create_small_button(":/icons/custom_btn.svg", QString::fromStdString(desc.name),
                                    desc.name.c_str(), 24, buttonBar_);
            connect(btn, &QPushButton::clicked, this, [this, i]() {
                block_->setParamValue(i, 1.0f); // nebo custom akce
            });
            buttonBarLayout_->addWidget(btn);
            control = btn;
            ++buttonCount;
            break;
        }
        case DSControlType::ToogleButton: {
            auto *btn =
                create_small_button(":/icons/toggle_btn.svg", QString::fromStdString(desc.name),
                                    desc.name.c_str(), 24, buttonBar_);
            btn->setCheckable(true);
            btn->setChecked(block_->getParamValue(i) > 0.5f);
            connect(btn, &QPushButton::clicked, this, [this, btn, i]() {
                bool checked = btn->isChecked();
                block_->setParamValue(i, checked ? 1.0f : 0.0f);
            });
            buttonBarLayout_->addWidget(btn);
            control = btn;
            ++buttonCount;
            break;
        }
        default:
            break;
        }
        if (control) paramWidgets_.push_back({control, desc.control_type});
    }
    buttonBarLayout_->addStretch(1);
    // Hide button bar if no buttons
    buttonBar_->setVisible(buttonCount > 0);
    if (buttonCount > 0) mainLayout_->addWidget(buttonBar_);

    // --- Center (Dial grid | Vertical slider bar) ---
    centerWidget_ = new QWidget(this);
    centerWidget_->setObjectName("DSPCenterWidget");
    auto *centerLayout = new QHBoxLayout(centerWidget_);
    centerLayout->setContentsMargins(MAIN_PADDING, MAIN_PADDING, MAIN_PADDING, MAIN_PADDING);
    centerLayout->setSpacing(0);

    ///////////////////////////////////////////////////////////////////////////////////////////
    // DIAL GRID
    ///////////////////////////////////////////////////////////////////////////////////////////
    dialGridWidget_ = new QWidget(centerWidget_);
    dialGridWidget_->setObjectName("DialGridWidget");
    dialGridLayout_ = new QGridLayout(dialGridWidget_);
    dialGridLayout_->setContentsMargins(0, 0, 0, 0);
    dialGridLayout_->setSpacing(2);

    for (size_t i = 0; i < params.size(); ++i) {
        const auto &desc = params[i];
        QWidget *control = nullptr;
        if (desc.control_type == DSControlType::Dial ||
            desc.control_type == DSControlType::DialCentered) {
            float value = block_->getParamValue(i);
            if (desc.control_type == DSControlType::Dial) {
                auto *dial = new AudioDial(dialGridWidget_);
                dial->setMinimumSize(DIAL_MIN_WIDTH, DIAL_MIN_HEIGHT);
                dial->setRange(desc.min_value, desc.max_value);
                dial->setValue(value);
                dial->setDefaultValue(desc.default_value);
                dial->setLabel(QString::fromStdString(desc.name));
                dial->setGradient(QColor("#6cb0ff"), QColor("#ae6cff"));
                dial->showLabel(true);
                dial->showValue(true);
                dial->setValueDecimals(2);
                connect(dial, &AudioDial::valueChanged, this,
                        [this, i](float val) { block_->setParamValue(i, val); });
                control = dial;
            } else {
                auto *dial = new AudioDialCentered(dialGridWidget_);
                dial->setMinimumSize(DIAL_MIN_WIDTH, DIAL_MIN_HEIGHT);
                dial->setRange(desc.min_value, desc.max_value);
                dial->setValue(value);
                dial->setDefaultValue(desc.default_value);
                dial->setLabel(QString::fromStdString(desc.name));
                dial->setGradient(QColor("#6cb0ff"), QColor("#ae6cff"));
                dial->showLabel(true);
                dial->showValue(true);
                dial->setValueDecimals(2);
                connect(dial, &AudioDialCentered::valueChanged, this,
                        [this, i](float val) { block_->setParamValue(i, val); });
                control = dial;
            }
            dialWidgets_.push_back(control);
        }
    }
    bool showDialGrid = !dialWidgets_.empty();

    ///////////////////////////////////////////////////////////////////////////////////////////
    // VSLIDER BAR
    ///////////////////////////////////////////////////////////////////////////////////////////
    vSliderWidget_ = new QWidget(centerWidget_);
    vSliderWidget_->setObjectName("VSliderWidget");
    vSliderWidget_->setFixedWidth(VSLIDER_WIDTH);
    vSliderLayout_ = new QVBoxLayout(vSliderWidget_);
    vSliderLayout_->setContentsMargins(0, 0, 0, 0);
    vSliderLayout_->setSpacing(2);

    for (size_t i = 0; i < params.size(); ++i) {
        const auto &desc = params[i];
        if (desc.control_type == DSControlType::SliderVertical) {
            float value = block_->getParamValue(i);
            auto *slider = new AudioVerticalSlider(vSliderWidget_);
            slider->setRange(desc.min_value, desc.max_value);
            slider->setDefaultValue(desc.default_value);
            slider->setValue(value);
            slider->setLabelText(QString::fromStdString(desc.name));
            slider->setLabelVisible(true);
            slider->setValueVisible(true);
            slider->setValueDecimals(2);
            slider->setFixedWidth(VSLIDER_WIDTH); // fixní šířka!
            connect(slider, &AudioVerticalSlider::valueChanged, this,
                    [this, i](float val) { block_->setParamValue(i, val); });
            vSliderLayout_->addWidget(slider);
            vSliderWidgets_.push_back(slider);
        }
    }
    bool showVSliderStack = !vSliderWidgets_.empty();

    // Hide empty dial grid or vslider bar
    dialGridWidget_->setVisible(showDialGrid);
    vSliderWidget_->setVisible(showVSliderStack);

    // Hlavní layout sekcí
    if (showDialGrid) centerLayout->addWidget(dialGridWidget_, 2);
    if (showDialGrid && showVSliderStack)
        centerLayout->addSpacerItem(
            new QSpacerItem(8, 8, QSizePolicy::Expanding, QSizePolicy::Expanding));
    if (showVSliderStack) centerLayout->addWidget(vSliderWidget_, 0);

    // Hide centerWidget if both sections are empty
    centerWidget_->setVisible(showDialGrid || showVSliderStack);
    if (showDialGrid || showVSliderStack) mainLayout_->addWidget(centerWidget_, 1);

    mainLayout_->addStretch();
    updateActivationButton();

    rebuildDialGrid();
}

void DSPBlockWidget::resizeEvent(QResizeEvent *event) {
    QFrame::resizeEvent(event);
    rebuildDialGrid();
}

void DSPBlockWidget::rebuildDialGrid() {
    // Dynamicky přepočítá grid dialů podle šířky
    if (!dialGridLayout_ || dialWidgets_.empty()) return;
    int gridW = dialGridWidget_->width();
    int cols = std::max(1, gridW / DIAL_MIN_WIDTH);
    int rows = (dialWidgets_.size() + cols - 1) / cols;
    // Vyčistit grid
    QLayoutItem *child;
    while ((child = dialGridLayout_->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->setParent(nullptr);
        delete child;
    }
    // Vložit dialy
    int idx = 0;
    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r < rows; ++r) {
            if (idx < dialWidgets_.size()) {
                dialGridLayout_->addWidget(dialWidgets_[idx], r, c);
                ++idx;
            }
        }
    }
    // Spacer vyplní zbytek
    dialGridLayout_->setRowStretch(rows, 1);
    dialGridLayout_->setColumnStretch(cols, 1);
}

void DSPBlockWidget::onLeftClicked() { emit moveLeftRequested(this); }

void DSPBlockWidget::onRightClicked() { emit moveRightRequested(this); }

void DSPBlockWidget::updateActivationButton() {
    if (!deactivateBtn_) return;
    deactivateBtn_->setIcon(
        QIcon(!deactivateBtn_->isChecked() ? ":/icons/active.svg" : ":/icons/inactive.svg"));
    deactivateBtn_->setToolTip(block_->isActive() ? "Deactivate block" : "Activate block");
}

void DSPBlockWidget::onDeactivateClicked() {
    block_->setActive(!deactivateBtn_->isChecked());
    updateActivationButton();
}

void DSPBlockWidget::onDeleteClicked() { emit deleteRequested(this); }

QSize DSPBlockWidget::minimumSizeHint() const {
    int minWidth = 120; // fallback

    // Pokud jsou dialy, vypočítej podle gridu
    int dialCols = 0;
    int dialCount = dialWidgets_.size();
    if (dialCount > 0) {
        dialCols = std::max(1, dialGridWidget_ ? dialGridWidget_->width() / DIAL_MIN_WIDTH : 1);
        int gridWidth = dialCols * DIAL_MIN_WIDTH + (dialCols - 1) * 2;
        minWidth = std::max(minWidth, gridWidth);
    }
    // Pokud jsou slidery, přičti jejich šířku
    int sliderCount = vSliderWidgets_.size();
    if (sliderCount > 0) {
        int sliderWidth = VSLIDER_WIDTH + 4; // slider + mezera
        minWidth += sliderWidth;
    }

    // Pokud není žádný obsah, minimální šířka
    if (dialCount == 0 && sliderCount == 0) { minWidth = 120; }

    // Pokud je buttonBar, přičti jeho případnou šířku (např. 40px)
    if (buttonBar_ && buttonBar_->isVisible()) {
        minWidth = std::max(minWidth, buttonBar_->sizeHint().width() + 20);
    }

    return QSize(minWidth, QFrame::minimumSizeHint().height());
}