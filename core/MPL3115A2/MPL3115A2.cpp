#include "MPL3115A2.h"
#include "mbed.h"

#include "stdarg.h" // For debugOut use of the ... parameter and va_list

MPL3115A2::MPL3115A2(I2C *i2c) : _i2c(i2c)
{
}

// By default I set the sensor to altimeter mode. I inserted a 1ms pause 
// between each call to allow logic capture if needed. This give a small
// gap between captures on the bus to make working with the data easier.
void MPL3115A2::init()
{
    setModeStandby();
    wait_ms(1);
    setModeAltimeter();
    wait_ms(1);
    setOversampleRate(7);
    wait_ms(1);
    enableEventFlags();
    wait_ms(1);
    setModeActive();
    wait_ms(1);
}

// This method wait for a specified amount of time for one of the data
// ready flags to be set. You need to pass in the correct data ready
// mask for this to work. See page 22 of the datasheet.
int MPL3115A2::dataReady(const char mask) {
    int attempts = 0;
 
    while ((i2cRead(STATUS) & mask) == 0)
    {
        attempts++;
        
        if(attempts > MAX_DATA_READY_ATTEMPTS) 
            return 0; // Failed
            
        wait_ms(1);
    }
    
    return 1; // Success
}

Altitude* MPL3115A2::readAltitude(Altitude* a) {
    // Force the sensor to take a new reading.
    toggleOneShot();
    
    // Wait for the data to be ready.
    if (!pressureDataReady())
    {
        // We leave the altitude object as is if we encounter an error.
        return a;
    }
        
    // Get the new data from the sensor.
    _i2c->start();                              // Start
    if (_i2c->write(MPL3115A2_ADDRESS) != 1) {    // A write to device
//        debugOut("MPL3115A2::readAltitude: Sensor failed to respond to write request at address 0x%X\r\n", MPL3115A2_ADDRESS);
    }
    if (_i2c->write(OUT_P_MSB) != 1) {           // Register to read
//        debugOut("MPL3115A2::readAltitude: Sensor at address 0x%X did not acknowledge register 0x%X\r\n", MPL3115A2_ADDRESS, OUT_P_MSB);
    }

    // Write the data directly into our Altitude object. This object
    // takes care of converting the compressed data from the sensor, and
    // provides functions to get the data in various units. And it also
    // has a print function to output the data as a string.
    _i2c->read(MPL3115A2_ADDRESS, (*a), Altitude::size);
    a->setAltitude();

    return a;
}

// See readAltitude for comments about this function.
Pressure* MPL3115A2::readPressure(Pressure* p)
{
    toggleOneShot();
    
    if (!pressureDataReady())
    {
//        debugOut("MPL3115A2::readPressure: Sensor failed to generate a pressure reading in a reasonable time.\r\n");
        return p;
    }

    _i2c->start(); 
    if (_i2c->write(MPL3115A2_ADDRESS) != 1)
//        debugOut("MPL3115A2::readPressure: Sensor failed to respond to write request at address 0x%X\r\n", MPL3115A2_ADDRESS);
    
    if (_i2c->write(OUT_P_MSB) != 1) 
//        debugOut("MPL3115A2::readPressure: Sensor at address 0x%X did not acknowledge register 0x%X\r\n", MPL3115A2_ADDRESS, OUT_P_MSB);
    
    _i2c->read(MPL3115A2_ADDRESS, (*p), Pressure::size);
    p->setPressure();

    return p;
}

// See readAltitude for comments about this function.
Temperature* MPL3115A2::readTemperature(Temperature* t)
{
    toggleOneShot();

    if (!temperatureDataReady())
    {
//        debugOut("MPL3115A2::readTemperature: Sensor failed to generate a temperature reading in a reasonable time.\r\n");
        return t;
    }

    _i2c->start();  
    if (_i2c->write(MPL3115A2_ADDRESS) != 1) {
//        debugOut("MPL3115A2::readTemperature: Sensor failed to respond to write request at address 0x%X\r\n", MPL3115A2_ADDRESS);
    }
    if (_i2c->write(OUT_T_MSB) != 1) {
//        debugOut("MPL3115A2::readTemperature: Sensor at address 0x%X did not acknowledge register 0x%X\r\n", MPL3115A2_ADDRESS, OUT_P_MSB);
    }

    _i2c->read(MPL3115A2_ADDRESS, (*t), Temperature::size);
    t->setTemperature();
    
    return t;
}

void MPL3115A2::setModeAltimeter()
{
    setRegisterBit(CTRL_REG1, 0x80);    // Set ALT bit
}

void MPL3115A2::setModeBarometer()
{
    clearRegisterBit(CTRL_REG1, 0x80);  // Clear ALT bit
}

void MPL3115A2::setModeStandby()
{
    clearRegisterBit(CTRL_REG1, 0x01);  // Clear SBYB bit for Standby mode
}
void MPL3115A2::setModeActive()
{
    setRegisterBit(CTRL_REG1, 0x01);    // Set SBYB bit for Active mode
}

void MPL3115A2::setOversampleRate(char sampleRate)
{
    if(sampleRate > 7) 
        sampleRate = 7;                 // OS cannot be larger than 0b.0111
    
    sampleRate <<= 3;                   // Align it for the CTRL_REG1 register
  
    char temp = i2cRead(CTRL_REG1);     // Read current settings
    temp &= 0xC7;                       // Clear out old OS bits
    temp |= sampleRate;                 // Mask in new OS bits
    i2cWrite(CTRL_REG1, temp);
}

void MPL3115A2::enableEventFlags()
{
    i2cWrite(PT_DATA_CFG, 0x07); // Enable all three pressure and temp event flags 
}

void MPL3115A2::toggleOneShot(void)
{
    clearRegisterBit(CTRL_REG1, 0x02);  // Clear OST bit
    setRegisterBit(CTRL_REG1, 0x02);    // Set the OST bit.
}

void MPL3115A2::clearRegisterBit(const char regAddr, const char bitMask)
{
    char temp = i2cRead(regAddr);   // Read the current register value
    temp &= ~bitMask;               // Clear the bit from the value
    i2cWrite(regAddr, temp);        // Write register value back
}

void MPL3115A2::setRegisterBit(const char regAddr, const char bitMask)
{
    char temp = i2cRead(regAddr);   // Read the current register value
    temp |= bitMask;                // Set the bit in the value
    i2cWrite(regAddr, temp);        // Write register value back
}

char MPL3115A2::i2cRead(char regAddr)
{
    _i2c->start();                              // Start
    if (_i2c->write(MPL3115A2_ADDRESS)!=1) {     // A write to device
//        debugOut("MPL3115A2::i2cRead: Sensor failed to respond to write request at address 0x%X\r\n", MPL3115A2_ADDRESS);
    }
    if (_i2c->write(regAddr)!=1) {               // Register to read
//        debugOut("MPL3115A2::i2cRead: Sensor at address 0x%X did not acknowledge register 0x%X\r\n", MPL3115A2_ADDRESS, regAddr);
    }
    _i2c->start();                  
    if (_i2c->write(MPL3115A2_ADDRESS|0x01)!=1) { // Read from device
//        debugOut("MPL3115A2::i2cRead: Sensor failed to respond to read request at address 0x%X\r\n", MPL3115A2_ADDRESS);
    }

    char result = _i2c->read(READ_NAK);         // Read the data
    _i2c->stop();
    
    return result;  
}

void MPL3115A2::i2cWrite(char regAddr, char value)
{
    char cmd[2];
    cmd[0] = regAddr;
    cmd[1] = value;
    if (_i2c->write(MPL3115A2_ADDRESS, cmd, 2) != 0) {
//        debugOut("MPL3115A2::i2cWrite: Failed writing value (%d, 0x%X) to register 0x%X\r\n", value, regAddr);
    }
}
