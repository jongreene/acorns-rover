/*
    MPL3115A2 Barometric Pressure and Tempurature Sensor Library
    By: Michael Lange
    Date: March 31, 2014
    License: This code is public domain.
 
    This class encapsulates a temperature reading from the sensor.
 
 */
 
 
#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "mbed.h"

//! Temperature provides a wrapper around temperature data coming from the sensor. The class handles
//! working with compressed data from the sensor and provides convenient functions for retreiving
//! the data in various units (with room to add more if needed).
class Temperature
{
public:

    //! The size of the compressed data buffer from the sensor. Used in an I2C read.
    static const int size = 2;

    //! The units we support converting the sensor data to.
    enum unitsType { CELSIUS, FAHRENHEIT, KELVIN };

    Temperature();
    Temperature(float a, unitsType units = FAHRENHEIT);
    Temperature(const char* compressed);
    Temperature(const char msb, const char lsb);

    //! Allows using the object directly in an I2C read operation.
    operator char*(void) { return _compressed; }
    //! Same as calling temperature with FAHRENHEIT as the parameter.
    operator float(void) { return _temperature; }

    float temperature(unitsType units = FAHRENHEIT);
    //! Call to decompress the sensor data after an I2C read.
    void setTemperature();
    void setTemperature(const char* compressed);
    void setTemperature(const char msb, const char lsb);
    void setTemperature(float a, unitsType units = FAHRENHEIT);

    //! Returns the temperature as a string in the units specified, defaulting to FAHRENHEIT if none specified.
    const char* print(unitsType units = FAHRENHEIT);
    
private:
    float _temperature;
    char  _compressed[2];
    char  _printBuffer[9];
};

#endif // TEMPERATURE_H