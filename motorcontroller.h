#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <QSerialPort>

enum MotorState {
    UNKNOWN,
    READY,
    ACTIVE
};

struct MotorStatus {
    bool connected;
    MotorState state;
    int position;
    int target;
};

class MotorController : public QObject
{
    Q_OBJECT

public:
    MotorController(QObject* parent = 0);
    ~MotorController();

    void goRelative(int target);
    void goAbsolute(int target);
    void stop();
    MotorStatus getStatus();

private slots:
    void onReadData();
    void onError(QSerialPort::SerialPortError error);

signals:
    void statusChanged(MotorStatus);

private:
    MotorStatus status;
    QSerialPort* port;
};

#endif // MOTORCONTROLLER_H
