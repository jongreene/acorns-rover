#ifndef ALTITUDE_H
#define ALTITUDE_H

#include "mbed.h"

//! Casting a float to a char truncates, therefore negative numbers become positive.
//! This will properly cast a float in the range -128 to 127 to a char.
#define float_to_char(x) (((x)<0)?(-(char)(x)):((char)(x)))

//! Altitude provides a wrapper around altitude data coming from the sensor. The class handles
//! working with compressed data from the sensor and provides convenient functions for retreiving
//! the data in various units (with room to add more if needed).
class Altitude
{
public:

    //! The size of the compressed data buffer from the sensor. Used in an I2C read.
    static const int size = 3;
    
    //! The units we support converting the sensor data to.
    enum unitsType { METERS, FEET };

    Altitude();
    Altitude(float a, unitsType units = FEET);
    Altitude(const char* compressed);
    Altitude(const char msb, const char csb, const char lsb);

    //! Allows using the object directly in an I2C read operation.
    operator char*(void) { return _compressed; }
    //! Same as calling altitude with METERS as the parameter.
    operator float(void) { return _altitude; }

    //! Returns the altitude in the units you specifiy, defaulting to FEET if none specified.
    float altitude(unitsType units = FEET);
    //! Call to decompress the sensor data after an I2C read.
    void setAltitude();
    void setAltitude(const char* compressed);
    void setAltitude(const char msb, const char csb, const char lsb);
    void setAltitude(float a, unitsType units = FEET);

    //! Returns the altitude as a string in the units specified, defaulting to FEET if none specified.
    const char* print(unitsType units = FEET);
    
    //! Converts meters to feet.
    static float MetersToFeet(float meters) { return meters * 3.28084; }
    //! Converts feet to meters.
    static float FeetToMeters(float feet) { return feet / 3.28084; }
    
private:
    float _altitude;
    char  _compressed[3];
    char  _printBuffer[9];
};

#endif // ALTITUDE_H