#include "settingsdialog.h"
#include "ui_settings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog) {
    ui->setupUi(this);
    QVector<QLineEdit *> edits{ui->additional_lines, ui->delay, ui->distance, ui->point_size, ui->timeout, ui->osc_port};
    foreach (QLineEdit *e, edits) {
        e->setValidator(new QIntValidator);
    }
    QSettings *s = Settings::s;
    ui->additional_lines->setText(QString::number(s->value("num_additional_lines").toInt()));
    ui->delay->setText(QString::number(s->value("delay").toInt()));
    ui->distance->setText(QString::number(s->value("distance").toInt()));
    ui->point_size->setText(QString::number(s->value("point_size").toInt()));
    ui->timeout->setText(QString::number(s->value("timeout").toInt()));
    ui->osc_port->setText(QString::number(s->value("osc_port").toInt()));
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    QSettings *s = Settings::s;
    s->setValue("num_additional_lines", ui->additional_lines->text().toInt());
    s->setValue("delay", ui->delay->text().toInt());
    s->setValue("distance", ui->distance->text().toInt());
    s->setValue("point_size", ui->point_size->text().toInt());
    s->setValue("timeout", ui->timeout->text().toInt());
    s->setValue("osc_port", ui->osc_port->text().toInt());
}
