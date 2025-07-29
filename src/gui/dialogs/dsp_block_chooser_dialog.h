#pragma once

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <note_naga_engine/dsp/dsp_factory.h>

/**
 * @brief Dialog for choosing a DSP block type.
 */
class DSPBlockChooserDialog : public QDialog {
    Q_OBJECT
public:
    explicit DSPBlockChooserDialog(QWidget* parent = nullptr);

    const DSPBlockFactoryEntry* selectedFactory() const {
        return selected_factory_;
    }

private:
    QListWidget* list_;
    const DSPBlockFactoryEntry* selected_factory_;
};
