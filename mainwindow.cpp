#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->controller = new MotorController(this);
    connect(this->controller, SIGNAL(statusChanged(MotorStatus)), this, SLOT(onStatusChange(MotorStatus)));
    this->onStatusChange(this->controller->getStatus());
}

MainWindow::~MainWindow() {
    delete this->controller;
    delete ui;
}

void MainWindow::onUp() {
    int speed = this->ui->speedSlider->value();
    this->controller->goRelative(speed);
}

void MainWindow::onDown() {
    int speed = this->ui->speedSlider->value();
    this->controller->goRelative(-speed);
}

void MainWindow::onGoto() {
    bool ok = false;
    int target = QInputDialog::getInt(this, "Go To", "Target:", 0, -2147483647, 2147483647, 1, &ok);
    if (ok) this->controller->goAbsolute(target);
}

void MainWindow::onStop() {
    this->controller->stop();
}

void MainWindow::onStatusChange(MotorStatus status) {
    this->ui->positionLabel->setText(QString::number(status.position));
    this->ui->targetLabel->setText(QString::number(status.target));

    QPalette* palette = new QPalette();

    switch (status.state) {
        case MotorState::ACTIVE:
            palette->setColor(QPalette::WindowText, Qt::blue);
            break;

        case MotorState::READY:
            palette->setColor(QPalette::WindowText, Qt::green);
            break;

        default:
            palette->setColor(QPalette::WindowText, Qt::red);
            break;
    }

    this->ui->statusLabel->setPalette(*palette);
}
