#ifndef SERIAL_BUFFER_INTERFACE_H
#define SERIAL_BUFFER_INTERFACE_H

#include <string>
#include <stack>
#include <array>
#include "mbed.h"

struct Buffer {
	Buffer() : pending_bytes_(0), in_(0), out_(0) {
		data_ = new std::array<char,size_>{};
	}
	~Buffer() {delete[] data_;}

	int pending_bytes_;
	static const int size_ = 256;

	std::array<char,size_>* data_;
	int in_, out_;
};

class UARTHandler {
public:
	UARTHandler(PinName tx, PinName rx, int baud);
	~UARTHandler();

	void SendLine(std::string);
	const std::string ReadLine();

private:
	void RxISR();
	void TxISR();
	Buffer* rx_buffer_;
	Buffer* tx_buffer_;
	RawSerial* serial_;
};


#endif //SERIAL_BUFFER_INTERFACE_H
