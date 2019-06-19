//
// Created by Jonathon Greene on 6/18/18.
//
#ifndef PLATFORM_INTERFACE_H
#define PLATFORM_INTERFACE_H

#include "mbed.h"
#include "UARTHandler.h"
#include "Commands.h"

enum class RESPONSE {
    NACK,
    ACK,
    COMMAND_NOT_FOUND,
    COMMAND_STOPPED,
    COMMAND_RUNNING,
    QUEUE_OVERFLOW
};

class PlatformInterface{
public:
	PlatformInterface();
    void emit(std::string &);

private:
	void CommandsCallback(string &raw );
	Commands* commands_object_;
    void dispatcher();
    void sendResponse(RESPONSE response, const std::string &message = "unknown_error");

    Thread workerThread_, commandProcessingThread;
    EventQueue *workQueue_;

    /*!
     * UART variables
     */
    UARTHandler* serial_buffer_;
};

#endif //PLATFORM_INTERFACE_H
