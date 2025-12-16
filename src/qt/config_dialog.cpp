#include "config_dialog.hpp"
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("CHIP-8 Emulator");
    setMinimumWidth(400);

    auto *mainLayout = new QVBoxLayout(this);

    // Title
    auto *titleLabel = new QLabel("CHIP-8 Emulator Configuration");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin-bottom: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Form layout for settings
    auto *formLayout = new QFormLayout();

    // ROM file selection
    auto *romLayout = new QHBoxLayout();
    romPathEdit = new QLineEdit();
    romPathEdit->setPlaceholderText("Select a ROM file...");
    romPathEdit->setReadOnly(true);
    browseButton = new QPushButton("Browse...");
    romLayout->addWidget(romPathEdit);
    romLayout->addWidget(browseButton);
    formLayout->addRow("ROM File:", romLayout);

    // Scale setting
    scaleSpinBox = new QSpinBox();
    scaleSpinBox->setRange(1, 20);
    scaleSpinBox->setValue(10);
    scaleSpinBox->setSuffix("x");
    formLayout->addRow("Display Scale:", scaleSpinBox);

    // Delay setting
    delaySpinBox = new QSpinBox();
    delaySpinBox->setRange(1, 100);
    delaySpinBox->setValue(2);
    delaySpinBox->setSuffix(" ms");
    formLayout->addRow("Cycle Delay:", delaySpinBox);

    mainLayout->addLayout(formLayout);

    // Info label
    auto *infoLabel = new QLabel(
        "Scale: Multiplier for the 64x32 display (e.g., 10x = 640x320 window)\n"
        "Delay: Milliseconds between CPU cycles (lower = faster)");
    infoLabel->setStyleSheet("color: gray; font-size: 11px; margin-top: 10px;");
    mainLayout->addWidget(infoLabel);

    // Buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    cancelButton = new QPushButton("Cancel");
    startButton = new QPushButton("Start");
    startButton->setDefault(true);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(startButton);
    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(browseButton, &QPushButton::clicked, this, &ConfigDialog::browseRom);
    connect(startButton, &QPushButton::clicked, this, &ConfigDialog::validateAndAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ConfigDialog::browseRom() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select CHIP-8 ROM", QString(),
                                                    "CHIP-8 ROMs (*.ch8 *.c8 *.rom);;All Files (*)");
    if (!fileName.isEmpty()) {
        romPathEdit->setText(fileName);
    }
}

void ConfigDialog::validateAndAccept() {
    if (romPathEdit->text().isEmpty()) {
        QMessageBox::warning(this, "No ROM Selected", "Please select a ROM file to load.");
        return;
    }
    accept();
}
