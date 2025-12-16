#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

class ConfigDialog : public QDialog {
    Q_OBJECT

  public:
    explicit ConfigDialog(QWidget *parent = nullptr);

    QString romPath() const { return romPathEdit->text(); }
    int scale() const { return scaleSpinBox->value(); }
    int delay() const { return delaySpinBox->value(); }

  private slots:
    void browseRom();
    void validateAndAccept();

  private:
    QLineEdit *romPathEdit;
    QPushButton *browseButton;
    QSpinBox *scaleSpinBox;
    QSpinBox *delaySpinBox;
    QPushButton *startButton;
    QPushButton *cancelButton;
};
