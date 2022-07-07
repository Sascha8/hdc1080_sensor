

#include <Arduino.h>
#include <HDC1080_sensor.h>
#include <Wire.h>


HDC1080_sensor::HDC1080_sensor(){}

HDC1080_sensor::~HDC1080_sensor() {}


void HDC1080_sensor::begin(TwoWire* theWire, int pinSDA, int pinSCL){
	_pinSDA = pinSDA;
	_pinSCL = pinSCL;
	_myWire = theWire;
	_myWire->clearWriteError();
	_myWire->begin(_pinSDA, _pinSCL);
}

void HDC1080_sensor::begin(int pinSDA, int pinSCL) {
	_myWire = &Wire;
	_pinSDA = pinSDA;
	_pinSCL = pinSCL;
	this->begin(_myWire, _pinSDA, _pinSCL);
}
void HDC1080_sensor::begin() {
	_myWire = &Wire;
	_pinSDA = SDA;
	_pinSCL = SCL;
	this->begin(_myWire,_pinSDA, _pinSCL);
}

boolean HDC1080_sensor::sensorFound() {
	_myWire->beginTransmission(_address);
	uint8_t ack = _myWire->endTransmission();
	return ack == 0;
}


uint16_t HDC1080_sensor::getManufactorID() {
	if (writeCommand(cmdMaufactorID) != 0) Serial.println("getManID error!");
	uint8_t	buf[2] = { 0,0 };
	uint8_t reqF = _myWire->requestFrom(_address, uint8_t(2));
	uint8_t read = _myWire->readBytes(buf, (uint8_t)2);
	uint16_t res = buf[0] << 8;
	res |= buf[1];
	return res;
}

uint16_t HDC1080_sensor::getDeviceID() {
	if (writeCommand(cmdDeviceID) != 0) Serial.println("getDevID error!");
	uint8_t	buf[2] = { 0,0 };
	uint8_t reqF = _myWire->requestFrom(_address, uint8_t(2));
	uint8_t read = _myWire->readBytes(buf, (uint8_t)2);
	uint16_t res = buf[0] << 8;
	res |= buf[1];
	return res;
}
serialID_data HDC1080_sensor::getSerialID() {
	serialID_data result;
	uint16_t snHigh = 0;
	uint16_t snMid = 0;
	uint16_t snLow = 0;
	uint8_t	buf[2] = { 0,0 };
	if (writeCommand(cmdSerialIDHigh) != 0) Serial.println("getSerIDHigh error!");
	uint8_t reqF = _myWire->requestFrom(_address, uint8_t(2));
	uint8_t read = _myWire->readBytes(buf, (uint8_t)2);
	snHigh = buf[0] << 8 | buf[1];
	
	buf[0] = 0; buf[1] = 0;
	if (writeCommand(cmdSerialIDMid) != 0) Serial.println("getSerIDMid error!");
	reqF = _myWire->requestFrom(_address, uint8_t(2));
	read = _myWire->readBytes(buf, (uint8_t)2);
	snMid = buf[0] << 8 | buf[1];

	buf[0] = 0; buf[1] = 0;
	if (writeCommand(cmdSerialIDLow) != 0) Serial.println("getSerID-Low error!");
	reqF = _myWire->requestFrom(_address, uint8_t(2));
	read = _myWire->readBytes(buf, (uint8_t)2);
	snLow = buf[0] << 8 | buf[1];

	result.day = (snHigh & 0x3E) >> 1;
	result.month = (snHigh & 0x7C0) >> 6;
	result.year = (snHigh & 0x1) << 4;
	result.year |= (snMid & 0xF000) >> 12;

	result.data = (snMid & 0x0FFF)<< 8;
	result.data |= (snLow & 0xFF80) >> 8;
	result.dataFull = snHigh;
	result.dataFull = result.dataFull << 24;
	result.dataFull |= snMid << 8;
	result.dataFull |= snLow ;
	return result;
}


uint16_t HDC1080_sensor::read(uint8_t* data, uint8_t numOfBytes)
{
	uint8_t	buf[20];
	Wire.requestFrom(_address, numOfBytes);

	int counter = 0;

	for (counter = 0; counter < numOfBytes; counter++) {
		Wire.readBytes(buf, numOfBytes);
		data[counter] = (buf[counter]);
	}

	return 0;
}

uint16_t HDC1080_sensor::writeCommand(uint8_t command) {
	_myWire->beginTransmission(_address);
	_myWire->write(command);
	return Wire.endTransmission();
}


uint16_t HDC1080_sensor::heaterEnable() {
	return writeCommand(0);
}

uint16_t HDC1080_sensor::heaterDisable() {
	return writeCommand(0);
}


clima_data HDC1080_sensor::update()
{
	uint16_t rawTemp;
	uint16_t rawHum;
	clima_data result;
	
	uint32_t error;
	uint8_t	buf[4] = { 0,0,0,0 };
	uint8_t data = 0;
	data = B00010000;

	_myWire->beginTransmission(_address);
	uint8_t write = _myWire->write(cmdSetConfig);
	uint8_t write2 = _myWire->write(data);
	uint8_t write3 = _myWire->write(0);
	uint8_t ack = _myWire->endTransmission();
	
	_myWire->beginTransmission(_address);
	write = _myWire->write(cmdReadTemp);
	ack = _myWire->endTransmission();
	delay(20);

	uint8_t reqF = _myWire->requestFrom(_address, uint8_t(4));
	buf[0] = _myWire->read();
	buf[1] = _myWire->read();
	buf[2] = _myWire->read();
	buf[3] = _myWire->read();

	rawTemp = buf[0] << 8 | buf[1];
	rawHum = buf[2] << 8 | buf[3];
	result.Temp = calculateTemperature(rawTemp);
	result.Hum = calculateHumidity(rawHum);
	return result;
}


float HDC1080_sensor::calculateTemperature(uint16_t rawValue)
{
	return ((float)rawValue / 65535.0f) * 165.0f - 40.0f;
}


float HDC1080_sensor::calculateHumidity(uint16_t rawValue)
{
	return ((float)rawValue / 65535.0f) * 100.0f ;
}


