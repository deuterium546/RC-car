
#include <Qtnetwork/QudpSocket>
#include <QNetworkDatagram>
#include "udp.h"
#include <string>
#include <QString>
udp::udp(QObject *parent)
	: QObject(parent)
{
	udpSocket = new QUdpSocket(this);
	std::string response;
	//connect(udpSocket, SIGNAL(readyRead()),this, SLOT(initServer()));
}
void udp::initServer() {
	QHostAddress senderaddr;
	while (udpSocket->hasPendingDatagrams()) {
		QByteArray datagram;

		datagram.resize(udpSocket->pendingDatagramSize());
		udpSocket->readDatagram(datagram.data(), datagram.size(), &senderaddr);
		qDebug() << senderaddr.toString();
		//response = datagram.data();
	}
}
void udp::sendData(QString Movement)
{
	QByteArray moveInfo;
	moveInfo.append(Movement);
	udpSocket->writeDatagram(moveInfo.data(), moveInfo.size(), QHostAddress("192.168.88.134"), 2390);
}
bool udp::recieveData()
{
	while (udpSocket->hasPendingDatagrams()) {
		QHostAddress senderaddr;
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());
		udpSocket->readDatagram(datagram.data(), datagram.size(), &senderaddr);

		response = datagram.data();
		if (response == "1") {
			return true;
		}
		else {
			qDebug() << "FALSE";
			return false;
		}
	}
}