/*
    MPL3115A2 Barometric Pressure and Tempurature Sensor Library
    By: Michael Lange
    Date: March 31, 2014
    License: This code is public domain.
 
    This class encapsulates a pressure reading from the sensor.
 
 */
 
 
#ifndef PRESSURE_H
#define PRESSURE_H

#include "mbed.h"

//! Pressure provides a wrapper around barometric data coming from the sensor. The class handles
//! working with compressed data from the sensor and provides convenient functions for retreiving
//! the data in various units (with room to add more if needed).
class Pressure
{
public:

    //! The size of the compressed data buffer from the sensor. Used in an I2C read.
    static const int size = 3;

    //! The units we support converting the sensor data to.
    enum unitsType { PASCALS, PSI, INHG, MMHG };

    Pressure();
    Pressure(float a, unitsType units = PASCALS);
    Pressure(const char* compressed);
    Pressure(const char msb, const char csb, const char lsb);

    //! Allows using the object directly in an I2C read operation.
    operator char*(void) { return _compressed; }
    //! Same as calling pressure with PASCALS as the parameter.
    operator float(void) { return _pressure; }

    float pressure(unitsType units = PASCALS);
    //! Call to decompress the sensor data after an I2C read.
    void setPressure();
    void setPressure(const char* compressed);
    void setPressure(const char msb, const char csb, const char lsb);
    void setPressure(float a, unitsType units = PASCALS);

    //! Returns the pressure as a string in the units specified, defaulting to PASCAL if none specified.
    const char* print(unitsType units = PASCALS);
    
private:
    float _pressure;
    char  _compressed[3];
    char  _printBuffer[9];
};

#endif // PRESSURE_H