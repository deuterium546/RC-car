#pragma once

#include <QObject>
#include <string>
#include <QString>
#include <Qtnetwork/QudpSocket>
class QudpSocket;
class udp : public QObject
{
	Q_OBJECT

public:
	udp(QObject *parent);
	void sendData(QString data);
	bool recieveData();
	std::string response;
	private slots:
	void initServer();
private:

	QHostAddress *recieveAddr;
	int portNum;
	QUdpSocket *udpSocket;
};
