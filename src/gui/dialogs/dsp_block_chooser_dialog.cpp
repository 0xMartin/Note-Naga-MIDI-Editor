#include "dsp_block_chooser_dialog.h"

DSPBlockChooserDialog::DSPBlockChooserDialog(QWidget *parent)
    : QDialog(parent), selected_factory_(nullptr) {
    setWindowTitle("Add DSP Block");
    setMinimumSize(260, 320);
    QVBoxLayout *layout = new QVBoxLayout(this);

    list_ = new QListWidget(this);
    for (const auto &entry : DSPBlockFactory::allBlocks()) {
        list_->addItem(QString::fromStdString(entry.name));
    }
    layout->addWidget(list_);

    QPushButton *btnOk = new QPushButton("Add", this);
    QPushButton *btnCancel = new QPushButton("Cancel", this);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);
    layout->addLayout(btnLayout);

    connect(btnOk, &QPushButton::clicked, this, [this]() {
        int row = list_->currentRow();
        if (row >= 0 && row < int(DSPBlockFactory::allBlocks().size())) {
            selected_factory_ = &DSPBlockFactory::allBlocks()[row];
            accept();
        }
    });
    connect(btnCancel, &QPushButton::clicked, this, [this]() { reject(); });
}