#include "SteeringWheelTest.h"
#include "SerialCom.h"
#include<QPushButton>
#include<qDebug>
#include <QTimer>
#include <QGridLayout>
#include <qmath.h>
#include <iomanip>
#include <QLabel>
#include <QRadioButton>
#include <QSerialPort>
#include <QGroupBox>
#include <QString>
SteeringWheelTest::SteeringWheelTest(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(500, 500);
	QGridLayout *grid = new QGridLayout(this);
	setupDataLabels();
	const QString serialPortName = "COM5";
	const int serialPortBaudRate = 9600;
	serialPort = new SerialCom(serialPortName, serialPortBaudRate, QIODevice::WriteOnly);
	HWND win = (HWND)this->winId(); // because the SDK works on the HWND object, so need to convert	
	
	if (!LogiSteeringInitializeWithWindow(false, win)) {
		initSucceded = false;
		qDebug() << "ERROR";
	}
	else {		
		LogiUpdate();
		grid->addWidget(createWheelData(), 0, 0);
		setLayout(grid);
		Controltimer = new QTimer(this);
		setupExitButton();
		connect(Controltimer, SIGNAL(timeout()), this, SLOT(getPos()));
		sendCommand = new QPushButton("send", this);
		sendCommand->setGeometry(410, 60, 80, 50);
		connect(sendCommand, SIGNAL(released()), this, SLOT(getPos()));
		Controltimer->start(30);
	}	
}
void SteeringWheelTest::setupExitButton() {
	exit = new QPushButton("Exit", this);
	exit->setGeometry(410, 10, 80, 50);
	connect(exit, SIGNAL(released()), QApplication::instance(), SLOT(quit()));
}
void SteeringWheelTest::setupDataLabels() {
	wheelConnectionStatusValueLabel = new QLabel(this);
	transformedwheelRotationValueLabel = new QLabel("0", this);
	transformedbrakeValueLabel = new QLabel("0", this);
	actualWheelRotationValueLabel = new QLabel("0", this);
	transformedthrottleValueLabel = new QLabel("0", this);
	transformedSpeedValueLabel = new QLabel("0", this);
	wheelNameValueLabel = new QLabel(this);
	vectorOfLabelPairs.push_back(std::pair<QString, QLabel*>{"Wheel connection", wheelConnectionStatusValueLabel});
	vectorOfLabelPairs.push_back(std::pair<QString, QLabel*>{"Wheel name", wheelNameValueLabel});
	vectorOfLabelPairs.push_back(std::pair<QString, QLabel*>{"Actual rotation value", actualWheelRotationValueLabel});
	vectorOfLabelPairs.push_back(std::pair<QString, QLabel*>{"Transformed rotation value", transformedwheelRotationValueLabel});
	vectorOfLabelPairs.push_back(std::pair<QString, QLabel*>{"Transformed throttle value", transformedthrottleValueLabel});
	vectorOfLabelPairs.push_back(std::pair<QString, QLabel*>{"Transformed brake value", transformedbrakeValueLabel});
	vectorOfLabelPairs.push_back(std::pair<QString, QLabel*>{"Transformed speed value", transformedSpeedValueLabel});
}
QVBoxLayout *SteeringWheelTest::CreateDataLabelPairs() {
	QVBoxLayout *vbox = new QVBoxLayout;
	for (std::pair<QString,QLabel*>labelPair : vectorOfLabelPairs) {
		QHBoxLayout *tempHboxRow = new QHBoxLayout;
		QLabel *tempLabel = new QLabel(labelPair.first, this);
		tempHboxRow->addWidget(tempLabel, 1);
		tempHboxRow->addWidget(labelPair.second,3);
		vbox->addLayout(tempHboxRow);
	}
	vbox->addStretch(1);
	return vbox;
}
QGroupBox *SteeringWheelTest::createWheelData() {
	QGroupBox *groupBox = new QGroupBox(tr("Wheel Info"));
	QVBoxLayout *vbox = CreateDataLabelPairs();
	groupBox->setLayout(vbox);	
	return groupBox;
}
void SteeringWheelTest::setWheelConnectionStatusString(bool isConnected) {
	if (isConnected) {
		wheelConnectionStatusValueLabel->setText("Wheel detected");
	}
	else {
		wheelConnectionStatusValueLabel->setText("Wheel not detected");
	}
}
QString SteeringWheelTest::getFormattedDataStringComponent(float controlValue) {
	QString value = QString::number(controlValue, 'f', 4);
	QString Output = "";
	for (int i = 0; i < StringControlValueMessageLengthIncludingDecimalPoint - value.length(); i++) {
		Output.append("0");
	}
	return Output.append(value);
}
QString SteeringWheelTest::getDataString(float controlValues[numberOfControlValues]) {
	QString Command = "";
	for (int i = 0; i < numberOfControlValues; i++) {
		Command.append(getFormattedDataStringComponent(controlValues[i]));
	}
	return Command;
}
void SteeringWheelTest::sendData(QString Command) {
	serialPort->sendData(Command);
}
void SteeringWheelTest::getPos() {
	bool isDeviceConnected = LogiIsConnected(index);
	setWheelConnectionStatusString(isDeviceConnected);
	QString deviceName = getWheelName(index);
	setWheelNameLabel(deviceName);
	if (!isDeviceConnected) {
		return;
	}
	if (!LogiUpdate()) {
		qDebug() << "ERROR";
		return;
	}
	if (firstTime) {
		initialSteeringWheelValueBodge();
		firstTime = false;
	}	
	m_DIJoyState2Device[index] = LogiGetState(index);
	if (!isWheelStillReturningAllZeroValues()) {
		float transformedThrottleValue;
		float transformedWheelRotValue;
		float transformedBrakeValue;
		float transformedSpeedValue;
		float gradient = getGradient();
		transformedThrottleValue = getTransformedThrottleValue(gradient);
		transformedBrakeValue = getTransformedBrakeValue(gradient);
		transformedSpeedValue = getClampedTransformedSpeedValue(transformedThrottleValue, transformedBrakeValue);
		setTransformedSpeedText(transformedSpeedValue);
		setActualWheelRotText();
		transformedWheelRotValue = getClampedTransformedWheelRot();
		setTransformedWheelRotText(transformedWheelRotValue);
		setTransformedBrakeText(transformedBrakeValue);
		setTransformedThrottleText(transformedThrottleValue);
		float controlValues[numberOfControlValues] = { transformedSpeedValue,transformedWheelRotValue };
		QString TotalCommandString = getDataString(controlValues);
	}	
}
QString SteeringWheelTest::getWheelName(int index) {
	wchar_t deviceName[128];
	LogiGetFriendlyProductName(index, deviceName, 128);
	QString devNameStr;
	devNameStr = devNameStr.fromWCharArray(deviceName);	
	return devNameStr;
}
void SteeringWheelTest::setWheelNameLabel(QString name) {
	wheelNameValueLabel->setText(name);
}
void SteeringWheelTest::initialSteeringWheelValueBodge() {//without this the sdk returns the wheel and pedal values as all zeros until the user triggers something on the steering wheel / pedal set
	LogiPlayLeds(index, 0, 1, 50);
}
bool SteeringWheelTest::isWheelStillReturningAllZeroValues() { //even with the bodge the first few iterations, the values are still zero
	return !(m_DIJoyState2Device[index]->lX != 0 && m_DIJoyState2Device[index]->lY != 0 && m_DIJoyState2Device[index]->lRz != 0);
}
float SteeringWheelTest::getClampedTransformedWheelRot() {
	float transformedWheelRot = (m_DIJoyState2Device[index]->lX *((float)maxSteeringHalfAngle / absWheelvalLimit))+ shiftedSteeringCentre;
	if (transformedWheelRot > shiftedSteeringCentre + maxSteeringHalfAngle) {
		transformedWheelRot = shiftedSteeringCentre + maxSteeringHalfAngle;
	}
	else if (transformedWheelRot < shiftedSteeringCentre - maxSteeringHalfAngle) {
		transformedWheelRot = shiftedSteeringCentre - maxSteeringHalfAngle;
	}
	return transformedWheelRot;
}
float SteeringWheelTest::getClampedTransformedSpeedValue(float TransformedThrottleValue, float transformedBrakeValue) {
	float transformedSpeedValue = TransformedThrottleValue - brakingMultiplier * transformedBrakeValue;
	if (transformedSpeedValue < 1) {
		transformedSpeedValue = 0;
	}
	return transformedSpeedValue;
}
float SteeringWheelTest::getTransformedBrakeValue(float gradient) {
	float offset = -gradient * minRawThrottleBrakeVal;
	return m_DIJoyState2Device[index]->lRz *gradient + offset;
}
float SteeringWheelTest::getTransformedThrottleValue(float gradient) {
	float offset = (float)neutralTransformedThrottleVal - gradient * minRawThrottleBrakeVal;
	return m_DIJoyState2Device[index]->lY *gradient + offset; //mapped from 32767-> -32767 to 100->200
}
float SteeringWheelTest::getGradient() {
	return ((float)maxTransformedThrottleVal - neutralTransformedThrottleVal) / (maxRawThrottleBrakeVal - minRawThrottleBrakeVal);
}
void SteeringWheelTest::setTransformedSpeedText(float value) {
	transformedSpeedValueLabel->setText(QString::number(value));
}
void SteeringWheelTest::setActualWheelRotText() {
	actualWheelRotationValueLabel->setText(QString::number(m_DIJoyState2Device[index]->lX));
}
void SteeringWheelTest::setTransformedWheelRotText(float value) {
	transformedwheelRotationValueLabel->setText(QString::number(value));
}
void SteeringWheelTest::setTransformedBrakeText(float value) {
	transformedbrakeValueLabel->setText(QString::number(value));
}
void SteeringWheelTest::setTransformedThrottleText(float value) {
	transformedthrottleValueLabel->setText(QString::number(value));
}
