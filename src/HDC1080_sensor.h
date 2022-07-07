
#pragma once

#include <Arduino.h>
#include <Wire.h>

struct serialID_data {
	uint64_t dataFull;
	uint32_t data;
	uint8_t day,month,year;
};

struct clima_data {
	float Temp;
	float Hum;
};
 

class HDC1080_sensor {
private:
	TwoWire* _myWire;
	int _pinSDA;
	int _pinSCL;

	const uint8_t _address = 0x40;
	const uint8_t cmdReadTemp = 0x00;
	const uint8_t cmdReadHum = 0x01;
	const uint8_t cmdSetConfig = 0x02;
	const uint8_t cmdSerialIDHigh = 0xFB;
	const uint8_t cmdSerialIDMid = 0xFC;
	const uint8_t cmdSerialIDLow = 0xFD;
	const uint8_t cmdMaufactorID = 0xFE;
	const uint8_t cmdDeviceID = 0xFF;
	

	uint16_t writeCommand(uint8_t command);
	uint16_t read(uint8_t* data, uint8_t numOfPair);
	float calculateHumidity(uint16_t rawValue);
	float calculateTemperature(uint16_t rawValue);
public:
	HDC1080_sensor();
	~HDC1080_sensor();
	void begin(TwoWire* theWire, int pinSDA, int pinSCL);
	void begin(int pinSDA, int pinSCL);
	void begin();
	boolean sensorFound();

	uint16_t getManufactorID();
	uint16_t getDeviceID();
	serialID_data getSerialID();

	uint16_t heaterEnable();
	uint16_t heaterDisable();

	clima_data update();

};



