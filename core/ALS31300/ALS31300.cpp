#include "ALS31300.h"

ALS31300::ALS31300(I2C *i2c) {
    i2c_ = i2c;
	setup("full");
}

void ALS31300::setup(string mode) {
    write(CUSTOMER_ACCESS_REG,CUSTOMER_ACCESS_CODE);
	setLoopMode(mode);
}

void ALS31300::setLoopMode(string loop_mode) {
	if(loop_mode == "single") {
		loop_mode_ = LOOP_MODE::SINGLE_LOOP;
	} else if(loop_mode == "fast") {
		loop_mode_ = LOOP_MODE::FAST_LOOP;
	} else if(loop_mode == "full") {
		loop_mode_ = LOOP_MODE::FULL_LOOP;
	}

	i2c_->start();
	i2c_->write((ALS31300_ADDRESS << 1) & 0xFE); // Slave Address with write bit set => 0
	i2c_->write(0x27);

	i2c_->stop();
	wait_us(2);
	i2c_->start();
	// Slave Address with read bit set => 1
	i2c_->write((ALS31300_ADDRESS << 1) | 0x01);

	uint32_t value0x27 = i2c_->read(1) << 24;
	value0x27 += i2c_->read(1) << 16;
	value0x27 += i2c_->read(1) << 8;
	value0x27 += i2c_->read(0);
	i2c_->stop();

	// I2C loop mode is in bits 2 and 3 so mask them out
	// and set them to the full loop mode
	value0x27 = (value0x27 & 0xFFFFFFF3) | (0x2 << 2);

	// Write the new values to the register the I2C loop mode is in
	write(0x27, value0x27);


	// setting BW
	i2c_->start();
	i2c_->write((ALS31300_ADDRESS << 1) & 0xFE); // Slave Address with write bit set => 0
	i2c_->write(0x02);

	i2c_->stop();
	wait_us(2);
	i2c_->start();
	// Slave Address with read bit set => 1
	i2c_->write((ALS31300_ADDRESS << 1) | 0x01);

	uint32_t value0x02 = i2c_->read(1) << 24;
	value0x02 += i2c_->read(1) << 16;
	value0x02 += i2c_->read(1) << 8;
	value0x02 += i2c_->read(0);
	i2c_->stop();

	// I2C loop mode is in bits 2 and 3 so mask them out
	// and set them to the full loop mode
	value0x02 = (value0x02 & 0xFE7FFFFF);

	// Write the new values to the register the I2C loop mode is in
	write(0x02, value0x02);

	wait_us(160);
}

bool ALS31300::write(uint8_t reg, uint32_t data){
	i2c_->start();
	i2c_->write((ALS31300_ADDRESS << 1) & 0xFE);
	i2c_->write(reg);
	i2c_->write((char)(data >> 24));
	i2c_->write((char)(data >> 16));
	i2c_->write((char)(data >> 8));
	i2c_->write((char)(data));

	i2c_->stop();
	wait_us(20);
	return true;
}

bool set = false;
vector<vector<float>> ALS31300::read(){
	vector<vector<float>> values(2);
	for (int i = 0; i < 2; ++i) {
		values.at(i) = vector<float>(3);
	}

	if(!set) {
		i2c_->start();
		i2c_->write((ALS31300_ADDRESS << 1) & 0xFE); // Slave Address with write bit set => 0
		i2c_->write(0x28);

		// Datasheet minimum time between between stop/start = 1.3us
		i2c_->stop();
		wait_us(2);
		i2c_->start();

		// Slave Address with read bit set => 1
		i2c_->write((ALS31300_ADDRESS << 1) | 0x01);
		set = true;
	}
	int x = i2c_->read(1) << 4;
	int y = i2c_->read(1) << 4;
	int z = i2c_->read(1) << 4;
	i2c_->read(1);

	// Read the next 4 bytes which are the contents of register 0x29
	i2c_->read(1);
	x |= i2c_->read(1) & 0x0F;
	char d = i2c_->read(1);
	y |= (d >> 4) & 0x0F;
	z |= d & 0x0F;
	i2c_->read(1);    // Temperature not used
//	i2c_->stop();

	// Sign extend the 12th bit for x, y and z.
	x = SignExtendBitfield((uint32_t)x, 12);
	y = SignExtendBitfield((uint32_t)y, 12);
	z = SignExtendBitfield((uint32_t)z, 12);

	values.at(0).at(0) = x;
	values.at(0).at(1) = y;
	values.at(0).at(2) = z;

	// Sensitivity of 500 gauss = 4.0 lsb/g
	// Sensitivity of 1000 gauss = 2.0 lsb/g <<<
	// Sensitivity of 2000 gauss = 1.0 lsb/g
	float rx = (float)x / 2.0;
	float ry = (float)y / 4.0;
	float rz = (float)z / 4.0;

	float angleXY = atan2f(ry, rx) * 180.0 / M_PI;
	float angleXZ = atan2f(rz, rx) * 180.0 / M_PI;
	float angleYZ = atan2f(rz, ry) * 180.0 / M_PI;

	values.at(1).at(0) = angleXY;
	values.at(1).at(1) = angleXZ;
	values.at(1).at(2) = angleYZ;

	return values;
}




void ALS31300::readALS31300ADC_FastLoop(int busAddress){
	uint32_t value0x27;
	value0x27 = (value0x27 & 0xFFFFFFF3) | (0x0 << 2);
	i2c_->start();
	i2c_->write(0x28);
	int x;
	int y;
	int z;

    for (int count = 0; count < 8; ++count) {
        // Start the read and request 4 bytes
        // which is the contents of register 0x28
//        Wire.requestFrom(busAddress, 4);

        // Read the first 4 bytes which are the contents of register 0x28
        // and sign extend the 8th bit
        x = SignExtendBitfield(i2c_->read(1), 8);
        y = SignExtendBitfield(i2c_->read(1), 8);
        z = SignExtendBitfield(i2c_->read(1), 8);
	    i2c_->read(1);    // Temperature and flags not used

        // Convert the X, Y and Z values into radians
        float rx = (float)x / 256.0 * M_TWOPI;
        float ry = (float)y / 256.0 * M_TWOPI;
        float rz = (float)z / 256.0 * M_TWOPI;

        // Use a four quadrant Arc Tan to convert 2
        // axis to an angle (which is in radians) then
        // convert the angle from radians to degrees
        // for display.
        float angleXY = atan2f(ry, rx) * 180.0 / M_PI;
        float angleXZ = atan2f(rz, rx) * 180.0 / M_PI;
        float angleYZ = atan2f(rz, ry) * 180.0 / M_PI;

    }
}


// Sign extend a right justified value
//
long ALS31300::SignExtendBitfield(uint32_t data, int width) {
    long x = (long)data;
    long mask = 1L << (width - 1);

    if (width < 32) {
        x = x & ((1 << width) - 1); // make sure the upper bits are zero
    }

    return (long)((x ^ mask) - mask);
}

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

std::string ALS31300::hexStr(unsigned char *data, int len) {
    std::string s((unsigned)len * 2, ' ');
    for (int i = 0; i < len; ++i) {
        s[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}