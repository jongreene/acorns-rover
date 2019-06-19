#ifndef MPL3115A2_H
#define MPL3115A2_H

#include "mbed.h"

#include "Altitude.h"               // Our classes to handle compressed data from the sensor.
#include "Temperature.h"
#include "Pressure.h"

#define MPL3115A2_ADDRESS   0xC0    // Shifted 7-bit I2C address for sensor

#define READ_ACK 1                  // For mbed I2C Read method.
#define READ_NAK 0 

#define MAX_DATA_READY_ATTEMPTS 512 // How many times we loop waiting for data before giving up.

//      DEFINE             |REGISTER |RESET |RESET    |TYPE |AUTO-INC      |NAME/COMMENT
//                         |         |      |STBY2ACT |     |ADDRESS       |
#define STATUS              0x00 //  | 0x00 | Yes     | R   | 0x01         | Sensor Status Register (Alias for DR_STATUS or F_STATUS)
#define OUT_P_MSB           0x01 //  | 0x00 | Yes     | R   | 0x02 | 0x01  | Pressure Data Out MSB (Bits 12-19 of 20-bit real-time Pressure sample | Root pointer t oPressure and Tempurature FIFO data)
#define OUT_P_CSB           0x02 //  | 0x00 | Yes     | R   | 0x03         | Pressure Data out CSB (Bits 0-3 of 20-bit real-time Pressure sample)
#define OUT_P_LSB           0x03 //  | 0x00 |         |     | 0x           |
#define OUT_T_MSB           0x04 //  | 0x00 |         |     | 0x           |
#define OUT_T_LSB           0x05 //  | 0x00 |         |     | 0x           |
#define DR_STATUS           0x06 //  | 0x00 |         |     | 0x           |
#define OUT_P_DELTA_MSB     0x07 //  | 0x00 |         |     | 0x           |
#define OUT_P_DELTA_CSB     0x08 //  | 0x00 |         |     | 0x           |
#define OUT_P_DELTA_LSB     0x09 //  | 0x00 |         |     | 0x           |
#define OUT_T_DELTA_MSB     0x0A //  | 0x00 |         |     | 0x           |
#define OUT_T_DELTA_LSB     0x0B //  | 0x00 |         |     | 0x           |
#define WHO_AM_I            0x0C //  | 0xC4 |         |     | 0x           |
#define F_STATUS            0x0D //  | 0x00 |         |     | 0x           |
#define F_DATA              0x0E //  | 0x00 |         |     | 0x           |
#define F_SETUP             0x0F //  | 0x00 |         |     | 0x           |
#define TIME_DLY            0x10 //  | 0x00 |         |     | 0x           |
#define SYSMOD              0x11 //  | 0x00 |         |     | 0x           |
#define INT_SOURCE          0x12 //  | 0x00 |         |     | 0x           |
#define PT_DATA_CFG         0x13 //  | 0x00 |         |     | 0x           |
#define BAR_IN_MSB          0x14 //  | 0xC5 |         |     | 0x           |
#define BAR_IN_LSB          0x15 //  | 0xE7 |         |     | 0x           |
#define P_TGT_MSB           0x16 //  | 0x00 |         |     | 0x           |
#define P_TGT_LSB           0x17 //  | 0x00 |         |     | 0x           |
#define T_TGT               0x18 //  | 0x00 |         |     | 0x           |
#define P_WND_MSB           0x19 //  | 0x00 |         |     | 0x           |
#define P_WND_LSB           0x1A //  | 0x00 |         |     | 0x           |
#define T_WND               0x1B //  | 0x00 |         |     | 0x           |
#define P_MIN_MSB           0x1C //  | 0x00 |         |     | 0x           |
#define P_MIN_CSB           0x1D //  | 0x00 |         |     | 0x           |
#define P_MIN_LSB           0x1E //  | 0x00 |         |     | 0x           |
#define T_MIN_MSB           0x1F //  | 0x00 |         |     | 0x           |
#define T_MIN_LSB           0x20 //  | 0x00 |         |     | 0x           |
#define P_MAX_MSB           0x21 //  | 0x00 |         |     | 0x           |
#define P_MAX_CSB           0x22 //  | 0x00 |         |     | 0x           |
#define P_MAX_LSB           0x23 //  | 0x00 |         |     | 0x           |
#define T_MAX_MSB           0x24 //  | 0x00 |         |     | 0x           |
#define T_MAX_LSB           0x25 //  | 0x00 |         |     | 0x           |
#define CTRL_REG1           0x26 //  | 0x00 |         |     | 0x           |
#define CTRL_REG2           0x27 //  | 0x00 |         |     | 0x           |
#define CTRL_REG3           0x28 //  | 0x00 |         |     | 0x           |
#define CTRL_REG4           0x29 //  | 0x00 |         |     | 0x           |
#define CTRL_REG5           0x2A //  | 0x00 |         |     | 0x           |
#define OFF_P               0x2B //  | 0x00 |         |     | 0x           |
#define OFF_T               0x2C //  | 0x00 |         |     | 0x           |
#define OFF_H               0x2D //  | 0x00 |         |     | 0x           |

//! MPL3115A2 I2C Barometric Pressure and Tempurature Sensor Library
//! This class wraps most of the function in the MPL3115A2 sensor leaving out the FIFO and interrupt system.
class MPL3115A2
{
public:
    //! Constructs an MPL3115A2 object and associates an I2C and optional Serial debug object.
    //! @param *i2c The I2C object to use for the sensor.
    //! @param *pc An optional serial debug connection object.
    MPL3115A2(I2C *i2c);

    //! Initializes the sensor, defaulting to Altitude mode. This should be called before using
    //! the sensor for the first time.
    void init();

    //! Queries the value from the WHO_AM_I register (usually equal to 0xC4).
    //! @return The fixed device ID from the sensor.
    char  whoAmI() { return i2cRead(WHO_AM_I); } 
    
    //! Reads Altitude data from the sensor and returns it in the Altitude object passed in. If
    //! no data could be read, the Altitude object is left as is.
    //! @param a A pointer to an Altitude object that will receive the sensor data.
    //! @returns The Altitude pointer that was passed in.
    Altitude* readAltitude(Altitude* a);            
    //! Reads Pressure data from the sensor and returns it in the Pressure object passed in. If
    //! no data could be read, the Pressure object is left as is.
    //! @param a A pointer to a Pressure object that will receive the sensor data.
    //! @returns The Pressure pointer that was passed in.
    Pressure* readPressure(Pressure* p);            
    //! Reads Temperature data from the sensor and returns it in the Temperature object passed in. If
    //! no data could be read, the Temperature object is left as is.
    //! @param a A pointer to an Temperature object that will receive the sensor data.
    //! @returns The Temperature pointer that was passed in.
    Temperature* readTemperature(Temperature* t);
    
    // Use these methods to set the sensor's offsets to increase its accuracy. You can generally
    // find your current altitude with a smart phone or GPS. Same goes for the temperature. For
    // the current pressure where you are, you may need an accurate weather station. Getting the
    // pressure from the web for your area is generally not close enough to help with calibration.
    // You may need to play with the setting to achieve good accuracy. I found the offset steps
    // were not 100% accurate to the datasheet and had to adjust accordingly. 
    
    //! Returns the altitude offset stored in the sensor.
    char offsetAltitude() { return i2cRead(OFF_H); }
    //! Sets the altitude offset stored in the sensor. The allowed offset range is from -128 to 127 meters.
    void setOffsetAltitude(const char offset) { i2cWrite(OFF_H, offset); } 
    //! Returns the pressure offset stored in the sensor.
    char offsetPressure() { return i2cRead(OFF_P); }
    //! Sets the pressure offset stored in the sensor. The allowed offset range is from -128 to 127 where each LSB represents 4 Pa.
    void setOffsetPressure(const char offset) { i2cWrite(OFF_P, offset); }
    //! Returns the temperature offset stored in the sensor.
    char offsetTemperature() { return i2cRead(OFF_T); }
    //! Sets the temperature offset stored in the sensor. The allowed offset range is from -128 to 127 where each LSB represents 0.0625ºC.
    void setOffsetTemperature(const char offset) { i2cWrite(OFF_T, offset); } 
    
    //! Puts the sensor into Standby mode. Required when using methods below.
    void  setModeStandby(); 
    //! Activates the sensor to start taking measurements.
    void  setModeActive();  
    
    //! Puts the sensor into barometric mode, be sure to put the sensor in standby mode first.
    void  setModeBarometer();           
    //! Puts the sensor into altimeter mode, be sure to put the sensor in standby mode first.
    void  setModeAltimeter();           
    //! Sets the number of samples from 1 to 128, be sure to put the sensor in standby mode first.
    void  setOversampleRate(char rate); 
    //! Sets all the event flags, be sure to put the sensor in standby mode first.
    void  enableEventFlags();           

private:
    //! The I2C object we use to communicate with the sensor. It is not part
    //! of the class so that it can be shared with other peripherals on the 
    //! bus.
    I2C *_i2c;

    //! This helper function is used to CLEAR bits. The mask should contain 1s in the position
    //! where the bits need to be cleared. One or more bits can be cleared this way.
    void clearRegisterBit(const char regAddr, const char bitMask);
    //! This helper function is used to SET bits. The mask should contain 1s in the position
    //! where the bits need to be set. One or more bits can be set this way.
    void setRegisterBit(const char regAddr, const char bitMask);

    //! Helper functions to check if data is ready in a register for either temperature or pressure.
    //! The mask passed in determines which register bit to look at.
    int dataReady(const char mask);
    //! Blocks for about 1/2 a second while checking the data ready bit. Returns 0 on sucees.
    //! This function works for both altitude and barometric pressure depending on the mode the sensor is in.
    int pressureDataReady() { return dataReady(0x04); }
    //! Blocks for about 1/2 a second while checking the data ready bit. Returns 0 on sucees.
    int temperatureDataReady() { return dataReady(0x02); }
 
    //! Called to force the sensor to take another sample
    void toggleOneShot();                       
    
    //! Helper functions to read one value from the I2C bus using the sensor's address.
    char i2cRead(char regAddr);
    //! Helper functions to write one value from the I2C bus using the sensor's address.
    void i2cWrite(char regAddr, char value);
};

#endif // MPL3115A2_H
