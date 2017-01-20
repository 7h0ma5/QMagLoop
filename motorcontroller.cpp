#include "motorcontroller.h"
#include <QDebug>
#include <QSettings>
#include <QSerialPortInfo>

MotorController::MotorController(QObject* parent) : QObject(parent)
{
    QSettings settings;
    qDebug() << settings.fileName();

    this->status.connected = false;
    this->status.state = MotorState::UNKNOWN;
    this->status.position = settings.value("motor/position", 0).value<int>();
    this->status.target = 0;

    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();

    for (QSerialPortInfo info : list) {
        qDebug() << info.portName();
    }

    this->port = new QSerialPort("COM3", this);
    this->port->setBaudRate(115200);
    this->port->setReadBufferSize(512);

    connect(this->port, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(onError(QSerialPort::SerialPortError)));
    connect(this->port, SIGNAL(readyRead()), this, SLOT(onReadData()));

    if (this->port->open(QIODevice::ReadWrite)) {
        qDebug() << "connected" << this->port->readBufferSize();
        this->status.connected = true;

    }
    else {
        qDebug() << this->port->error();
    }
}

MotorController::~MotorController() {
    QSettings settings;

    settings.setValue("motor/position", this->status.position);
    settings.sync();
}

void MotorController::goRelative(int target) {
    this->port->write(("G;" + QString::number(target) + ";").toUtf8());
    this->port->flush();
}

void MotorController::goAbsolute(int target) {
    this->port->write(("G;" + QString::number(target - this->status.position) + ";").toUtf8());
    this->port->flush();
}

void MotorController::stop() {
    this->port->write(QString("S;").toUtf8());
    this->port->flush();
}

MotorStatus MotorController::getStatus() {
    return status;
}

void MotorController::onReadData() {
    qDebug() << "onReadData";
    while (this->port->canReadLine()) {
        QByteArray data = this->port->readLine().trimmed();
        QList<QByteArray> params = data.split(';');

        qDebug() << data;

        if (params.length() < 1 && params[0].length() < 1) return;

        switch (params[0].at(0)) {
            case 'R':
                this->status.state = MotorState::READY;
                break;

            case 'A':
                this->status.state = MotorState::ACTIVE;
                break;

            case 'T':
                this->status.target = QString::fromUtf8(params[1]).toInt();
                break;

            case 'S':
                this->status.position += QString::fromUtf8(params[1]).toInt();
                break;
        }

        emit statusChanged(this->status);
    }
}

void MotorController::onError(QSerialPort::SerialPortError error) {
    qDebug() << error;
}
