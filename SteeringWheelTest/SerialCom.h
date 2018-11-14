#pragma once
#include <QSerialPort>
class SerialCom: public QSerialPort
{
public:
	SerialCom(QString portName, const int baudRate, QIODevice::OpenMode comType);
	~SerialCom();
	void sendData(QString data);
private:

};


