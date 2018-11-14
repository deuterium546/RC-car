#pragma once

#include <QtWidgets/QMainWindow>
#include <udp.h>
#include "SerialCom.h"
#include<string>
#include <QSerialPort>
#include "ui_SteeringWheelTest.h"
#pragma comment(lib,"LogitechSteeringWheelLib.lib")
#include "LogitechSteeringWheelLib.h"
class QPushButton;
class QLabel;
class QVBoxLayout;
class udp;
class QGroupBox;
class SteeringWheelTest : public QWidget
{
	Q_OBJECT

public:
	SteeringWheelTest(QWidget *parent = Q_NULLPTR);
private slots:
	void getPos();
	//void isConnected();
private:	
	const static int numberOfControlValues = 2;
	const int StringControlValueMessageLengthIncludingDecimalPoint = 8;
	const int maxRawThrottleBrakeVal = -32768;
	const int minRawThrottleBrakeVal = 32767;
	const int maxTransformedThrottleVal = 200;
	const int neutralTransformedThrottleVal = 100;
	const int maxTransformedBrakeVal = 100;
	const int absWheelvalLimit = 2000;
	const float brakingMultiplier = 1.5;
	const int index = 0;
	const float maxSteeringHalfAngle = 0.5;
	const float shiftedSteeringCentre = 90.0;
	bool firstTime = true;
	bool initSucceded = false;
	DIJOYSTATE2 *m_DIJoyState2Device[2];
	QPushButton *exit;
	QPushButton *sendCommand;
	QTimer *Controltimer;
	std::vector<std::pair<QString,QLabel*>> vectorOfLabelPairs;
	QLabel *actualWheelRotationValueLabel;
	QLabel *wheelConnectionStatusValueLabel;
	QLabel *transformedwheelRotationValueLabel;
	QLabel *wheelNameValueLabel;
	QLabel *transformedbrakeValueLabel;
	QLabel *transformedthrottleValueLabel;
	QLabel *transformedSpeedValueLabel;
	SerialCom *serialPort;
	bool isWheelStillReturningAllZeroValues();
	float getClampedTransformedWheelRot();
	float getClampedTransformedSpeedValue(float TransformedThrottleValue, float transformedBrakeValue);
	float getTransformedBrakeValue(float gradient);
	float getTransformedThrottleValue(float gradient);
	float getGradient();
	QString getDataString(float controlValues[numberOfControlValues]);
	QString getFormattedDataStringComponent(float controlValue);
	QString getWheelName(int indexNum);
	void setWheelNameLabel(QString name);
	QGroupBox *createWheelData();
	void setTransformedSpeedText(float value);
	void setActualWheelRotText();
	void setTransformedWheelRotText(float value);
	void setTransformedBrakeText(float value);
	void setTransformedThrottleText(float value);
	void setupExitButton();
	void setupDataLabels();
	QVBoxLayout *CreateDataLabelPairs();
	void setWheelConnectionStatusString(bool isConnected);
	void initialSteeringWheelValueBodge();
	void sendData(QString Command);
};
