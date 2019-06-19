#include "Altitude.h"
#include "mbed.h"

Altitude::Altitude()
{
    _altitude = 0.0;

    _compressed[0] = 0;
    _compressed[1] = 0;
    _compressed[2] = 0;
}

Altitude::Altitude(float a, unitsType units)
{
    setAltitude(a, units);
}
    
Altitude::Altitude(const char* compressed)
{
    setAltitude(compressed);
}

Altitude::Altitude(const char msb, const char csb, const char lsb)
{
    setAltitude(msb, csb, lsb);
}

void Altitude::setAltitude()
{
    setAltitude(_compressed[0], _compressed[1], _compressed[2]);
}

void Altitude::setAltitude(float a, unitsType units)
{
    // TODO:
}
    
void Altitude::setAltitude(const char* compressed)
{
    setAltitude(compressed[0], compressed[1], compressed[2]);
}

void Altitude::setAltitude(const char msb, const char csb, const char lsb)
{
    float tempcsb = (lsb>>4) / 16.0;
    _altitude = (float)((msb << 8) | csb) + tempcsb;
}

float Altitude::altitude(unitsType units)
{
    switch (units)
    {
        case FEET:
            return MetersToFeet(_altitude);
    }
    
    return _altitude;
}

const char* Altitude::print(unitsType units)
{
    sprintf(_printBuffer, "%.0f", altitude(units));
    return _printBuffer;
}
