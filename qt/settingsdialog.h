#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "settings.h"
#include <QDialog>
#include <QSettings>

namespace Ui {
class Dialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
};

#endif // SETTINGSDIALOG_H
