#include "SteeringWheelTest.h"
#include <QtWidgets/QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SteeringWheelTest w;
	w.show();	
	int ret = a.exec();
	LogiSteeringShutdown();
	return ret;
}
