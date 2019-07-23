//
// Created by uberslam on 6/28/19.
//

#include "Connector.h"
#include "LUARTHandler.h"

char read_buffer[1024];
char write_buffer[1024];


Connector::Connector() {
	connection_ = new connection{false,0,0,nullptr,nullptr,0};
	connection_->write_data_ = &write_buffer[0];
	connection_->read_data_ = &read_buffer[0];

	request_size_ = 32;
	leuartInit();
//	getNBytes(request_size_);

	connect();
}

void Connector::sendBytesCallback(unsigned int channel, bool primary, void *user)
{

}

void Connector::getBytesCallback(unsigned int channel, bool primary, void *user)
{
	auto instance = (Connector *)user;
	sendNBytes(true, instance->connection_->read_data_, instance->request_size_, &sendBytesCallback);
}

void Connector::sendPacket(char buffer[], int bytes)
{
	sendNBytes(true, buffer, bytes, &sendBytesCallback, (void *)this);
}

void Connector::getNBytes(int bytes)
{
	readNBytes(true, connection_->write_data_, bytes, &getBytesCallback, (void *)this);
}

int getRandomInt()
{
	// Seed with a real random value, if available
	std::random_device r;
	// Choose a random mean between 1 and 6
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(1, 6);
	return uniform_dist(e1);
}

void Connector::connect()
{


	memcpy ( connection_->write_data_, "syn_packet", strlen("syn_packet")+1 );

	sendPacket(connection_->write_data_, strlen("syn_packet"));
	getNBytes(request_size_);
//
//	connection_->ack = uniform_dist(e1);
//
//	Send a SYN packet with host_connection.seq
//
//	connection.host_ack = Received host SYN/ACK
//
//	connection.active = true;
}