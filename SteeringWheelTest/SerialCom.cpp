#include "SerialCom.h"
#include <QSerialPort>
SerialCom::SerialCom(QString portName,const int baudRate,QIODevice::OpenMode comType):QSerialPort()
{
	this->setPortName(portName);
	this->setBaudRate(baudRate);
	this->open(comType);
	this->setDataBits(QSerialPort::Data8);
	this->setStopBits(QSerialPort::OneStop);
	this->setFlowControl(QSerialPort::NoFlowControl);
	this->setParity(QSerialPort::NoParity);
}
SerialCom::~SerialCom()
{
	this->disconnect();
	this->close();
}
void SerialCom::sendData(QString data) 
{
	this->write(data.toUtf8());
}
