//
// Created by Jonathon Greene on 1/11/19.
//
#ifndef ALS31300_H
#define ALS31300_H

#include "mbed.h"
#include <math.h>
#include <string>
#include <vector>

#define ALS31300_ADDRESS        0x63
#define CUSTOMER_ACCESS_CODE    0x2C413534
#define CUSTOMER_ACCESS_REG     0x35
#define SLEEP_MODE_ACTIVE       0x00
#define SLEEP_MODE_SLEEP        0x01
#define SLEEP_MODE_LOW_POWER    0x02



//    single
//    value0x27 = (value0x27 & 0xFFFFFFF3) | (0x0 << 2);
//    //fast loop
//    value0x27 = (value0x27 & 0xFFFFFFF3) | (0x1 << 2);
//    full loop
//    value0x27 = (value0x27 & 0xFFFFFFF3) | (0x2 << 2);

enum class LOOP_MODE {
    SINGLE_LOOP,
    FAST_LOOP,
    FULL_LOOP
};


class ALS31300 {
public:
    explicit ALS31300(I2C *i2c);
    void setup(std::string);

	std::vector<vector<float>> read();
    bool write(uint8_t reg, uint32_t data);

    void readALS31300ADC_FastLoop(int busAddress);
    void readALS31300ADC_FullLoop(int busAddress);

    //todo: move this back to private after implementing the driver
    long static SignExtendBitfield(uint32_t data, int width);

private:
	mbed::I2C* i2c_;

	LOOP_MODE loop_mode_ = LOOP_MODE::SINGLE_LOOP;

//    uint16_t read(int busAddress, uint8_t address, uint32_t& value);
//    uint16_t write(int busAddress, uint8_t address, uint32_t value);

    void setLoopMode(std::string);

    std::string hexStr(unsigned char *data, int len);
};

#endif //ALS31300_H