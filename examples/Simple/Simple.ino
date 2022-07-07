/*
    Name:       Simple.ino
    Created:	05.07.2022 03:18:43
    Author:     sascha.ott@gmx.de
*/

#include <HDC1080_sensor.h>
#include <Wire.h>
#include <SPI.h>


HDC1080_sensor myHDC; // Declares HDC1080 instance. I2C address of HDC1080 is always 0x40 and not changeable.


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Simple HDC1080 demo\n");

  //myHDC.begin();  // Uses default Wire and default I2C pins
  myHDC.begin(2, 0);  // Uses default Wire but special GPIO for I2C
  //myHDC.begin(&Wire2, 2,0);  // Uses Wire2 (on esp32) and special GPIO for I2C
}

void loop() {
  uint16_t manID = myHDC.getManufactorID(); // Fixed value. Should be 0x5449
  Serial.printf("manID= 0x%X\n", manID);

  uint16_t DevID = myHDC.getDeviceID();  // Fixed value. Should be 0x1050
  Serial.printf("devID= 0x%X\n", DevID);

  // Read 41 bit value which contains production date and some unknown data. 
  // Datasheet is wrong here https://e2e.ti.com/support/sensors-group/sensors/f/sensors-forum/560539/hdc1080-serial-id-40-or-41-bits
  // Production date: https://e2e.ti.com/support/sensors-group/sensors/f/sensors-forum/854812/hdc1080-40-bit-serial-number
  serialID_data Serdata = myHDC.getSerialID();
  Serial.printf("Production date (d/m/y): %02d/%02d/%02d\n", Serdata.day, Serdata.month, Serdata.year);
  Serial.printf("Data: 0x%X\n", Serdata.data); // All data that is not production date related.
  Serial.printf("Fulldata: 0x%X\n", Serdata.dataFull); // Complete 41 bit information.

  clima_data  clima = myHDC.update();  // Triggers a measure of temperature and humidity. Takes ~ 20ms and returns a struct
  Serial.printf("Temp= %f C\n", clima.Temp);
  Serial.printf("Hum= %f %%\n\n", clima.Hum);
  delay(1000);
}
