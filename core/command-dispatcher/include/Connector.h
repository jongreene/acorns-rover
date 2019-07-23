//
// Created by uberslam on 6/28/19.
//

#ifndef SEMI_AUTONOMOUS_ROVER_CONNECTOR_H
#define SEMI_AUTONOMOUS_ROVER_CONNECTOR_H

#include <random>
#include <chrono>
#include <cstring>

struct connection {
	bool active;
	int seq;
	int ack;

	char *write_data_;
	char *read_data_;
	int pending_bytes;

};

class Connector {
public:
	Connector();

	void getNBytes(int bytes);
	void sendPacket(char buffer[], int bytes);

	static void getBytesCallback(unsigned int channel, bool primary, void *user);
	static void sendBytesCallback(unsigned int channel, bool primary, void *user);

	void connect();

	connection *connection_;

	int request_size_;



private:

};


#endif //SEMI_AUTONOMOUS_ROVER_CONNECTOR_H
