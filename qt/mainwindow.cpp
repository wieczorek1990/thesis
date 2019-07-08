#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "window.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Handwritten music notation editor");
    connect(ui->next, SIGNAL(clicked()), ui->widget, SLOT(oscNext()));
    connect(ui->prev, SIGNAL(clicked()), ui->widget, SLOT(oscPrev()));
    connect(ui->next_measure, SIGNAL(clicked()), ui->widget, SLOT(oscNextMeasure()));
    connect(ui->prev_measure, SIGNAL(clicked()), ui->widget, SLOT(oscPrevMeasure()));
    connect(ui->back, SIGNAL(clicked()), ui->widget, SLOT(oscBack()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionVariables_triggered() {
    SettingsDialog settings;
    settings.exec();
}
