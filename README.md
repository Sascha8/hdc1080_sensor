# HDC1080 sensor library 

# Description

An Arduino library for esp8266,esp32 for the HDC1080 temperature/humidity sensor.

## Documentation 
lib has currently just the basic feature of HDC1080
 * getManufactorID()  - returns a factory-programmed identification value (0x5449)
 * getDeviceID()      - returns a factory-programmed identification value (0x1050)
 * getSerialID()      - read 41 bit value which contains production date and some unknown data. 
 * update()           - triggers a measure of temperature and humidity. Takes ~ 20ms and returns a struct

heater related stuff and more error handling follows in future.

Written by Sascha Ott (sascha.ott@gmx.de)

MIT license, check license.txt for more information
All text above must be included in any redistribution

### Dependencies
 * none
