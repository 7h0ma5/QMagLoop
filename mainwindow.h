#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <motorcontroller.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onUp();
    void onDown();
    void onGoto();
    void onStop();
    void onStatusChange(MotorStatus);

private:
    Ui::MainWindow *ui;
    MotorController* controller;
};

#endif // MAINWINDOW_H
