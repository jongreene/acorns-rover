#include "Pressure.h"
#include "mbed.h"

Pressure::Pressure()
{
    _pressure = 0.0;

    _compressed[0] = 0;
    _compressed[1] = 0;
    _compressed[2] = 0;
}

Pressure::Pressure(float a, unitsType units)
{
    setPressure(a, units);
}
    
Pressure::Pressure(const char* compressed)
{
    setPressure(compressed);
}

Pressure::Pressure(const char msb, const char csb, const char lsb)
{
    setPressure(msb, csb, lsb);
}

void Pressure::setPressure()
{
    setPressure(_compressed[0], _compressed[1], _compressed[2]);
}

void Pressure::setPressure(float a, unitsType units)
{
    // TODO:
}
    
void Pressure::setPressure(const char* compressed)
{
    setPressure(compressed[0], compressed[1], compressed[2]);
}

void Pressure::setPressure(const char msb, const char csb, const char lsb)
{
    long pressure_whole = (long)msb<<16 | (long)csb<<8 | (long)lsb;
    pressure_whole >>= 6; // Pressure is an 18 bit number with 2 bits of decimal. Get rid of decimal portion.

    float pressure_decimal = (float)((lsb&0x30)>>4)/4.0; // Turn it into fraction

    _pressure = (float)pressure_whole + pressure_decimal;
}

float Pressure::pressure(unitsType units)
{
    // http://www.asknumbers.com/
    switch (units)
    {
        case PSI:
            return _pressure * 0.000145037738;    
        case INHG:
            return _pressure * 0.00029529983071;
        case MMHG:
            return _pressure * 0.007500615613;
    }
    
    return _pressure;
}

const char* Pressure::print(unitsType units)
{
    sprintf(_printBuffer, "%.0f", pressure(units));
    return _printBuffer;
}
