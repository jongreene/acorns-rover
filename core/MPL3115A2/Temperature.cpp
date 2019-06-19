#include "Temperature.h"
#include "mbed.h"

Temperature::Temperature()
{
    _temperature = 0.0;

    _compressed[0] = 0;
    _compressed[1] = 0;
}

Temperature::Temperature(float t, unitsType units)
{
    setTemperature(t, units);
}
    
Temperature::Temperature(const char* compressed)
{
    setTemperature(compressed);
}

Temperature::Temperature(const char msb, const char lsb)
{
    setTemperature(msb, lsb);
}

void Temperature::setTemperature()
{
    setTemperature(_compressed[0], _compressed[1]);
}

void Temperature::setTemperature(float t, unitsType units)
{
    // TODO:
}
    
void Temperature::setTemperature(const char* compressed)
{
    setTemperature(compressed[0], compressed[1]);
}

void Temperature::setTemperature(const char msb, const char lsb)
{
    float templsb = (lsb>>4) / 16.0; //temp, fraction of a degree
    _temperature = (float)(msb + templsb);
}

float Temperature::temperature(unitsType units)
{
    switch (units)
    {
        case FAHRENHEIT:
            return (_temperature * 9.0)/ 5.0 + 32.0;
        
        case KELVIN:
            return _temperature + 273.15;
    }
    
    return _temperature;
}

const char* Temperature::print(unitsType units)
{
    sprintf(_printBuffer, "%.1f", temperature(units));
    return _printBuffer;
}
