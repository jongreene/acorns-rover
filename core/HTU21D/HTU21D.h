#ifndef HTU21D_H
#define HTU21D_H

#include "mbed.h"

#define HTU21D_ADDRESS 0x40  //Unshifted 7-bit I2C address for the sensor

#define ERROR_I2C_TIMEOUT 	998
#define ERROR_BAD_CRC		999

#define TRIGGER_TEMP_MEASURE_HOLD  0xE3
#define TRIGGER_HUMD_MEASURE_HOLD  0xE5
#define TRIGGER_TEMP_MEASURE_NOHOLD  0xF3 //243
#define TRIGGER_HUMD_MEASURE_NOHOLD  0xF5 //245
#define WRITE_USER_REG  0xE6
#define READ_USER_REG  0xE7
#define SOFT_RESET  0xFE

#define USER_REGISTER_RESOLUTION_MASK 0x81
#define USER_REGISTER_RESOLUTION_RH12_TEMP14 0x00
#define USER_REGISTER_RESOLUTION_RH8_TEMP12 0x01
#define USER_REGISTER_RESOLUTION_RH10_TEMP13 0x80
#define USER_REGISTER_RESOLUTION_RH11_TEMP11 0x81

#define USER_REGISTER_END_OF_BATTERY 0x40
#define USER_REGISTER_HEATER_ENABLED 0x04
#define USER_REGISTER_DISABLE_OTP_RELOAD 0x02

/**
 * Honeywell HTU21D digital humidity and temperature sensor.
 */
class HTU21D {

public:

    /**
     * Constructor.
     *
     * @param sda mbed pin to use for SDA line of I2C interface.
     * @param scl mbed pin to use for SCL line of I2C interface.
     */
    HTU21D(I2C *i2c);

    //Samples the temperature, input void, outputs an int in celcius.
    float sample_ctemp(void);

    //Samples the temperature, input void, outputs an int in fahrenheit.
    float sample_ftemp(void);

    //Samples the temperature, input void, outputs an int in kelvin.
    float sample_ktemp(void);

    //Samples the humidity, input void, outputs and int.
    int sample_humid(void);



private:
    I2C* i2c_;

    /**
     * Write to EEPROM or RAM on the device.
     *
     * @param EepromOrRam 0x80 -> Writing to EEPROM
     * @param address Address to write to.
     * @param data Data to write.
     */
    void write(int EepromOrRam, int address, int data);

    /**
     * Read EEPROM or RAM on the device.
     *
     * @param EepromOrRam 0x81 -> Reading from EEPROM
     * @param address Address to read from.
     * @return The contents of the memory address.
     */
    int read(int EepromOrRam, int address);

};

#endif /* HTU21D_H */