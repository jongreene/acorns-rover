//
// Created by Jonathon Greene on 6/18/18.
//
#include "PlatformInterface.h"

using namespace std;

static auto bad_json_=R"({"nak","payload":{"error: badly formatted json"}})";
static auto command_not_found_=R"({"notification":"payload":{"error: command not found"}})";
static auto command_running_=R"({"notification":"payload":{"error: command is already running"}})";
static auto command_stopped_=R"({"notification":"payload":{"error: command is not running"}})";
static auto queue_overflowed_=R"({"notification":"payload":{"error: queue overflowed"}})";

PlatformInterface::PlatformInterface() {
    workQueue_= new EventQueue(32 * EVENTS_EVENT_SIZE);

    serial_buffer_ = new UARTHandler(PB9, PB10, 9600);
	commands_object_ = new Commands(this);
    workerThread_.start(callback(workQueue_, &EventQueue::dispatch_forever));
    commandProcessingThread.start(callback(this, &PlatformInterface::dispatcher));
}

const vector<string> ParseInput(string &input) {
	vector<string> command_object;
	size_t pos = 0;
	std::string token;
	while ((pos = input.find(',')) != std::string::npos) {
		token = input.substr(0, pos);
		command_object.push_back(token);
		input.erase(0, pos + 1);
	}
	command_object.push_back(input);
	return command_object;
}

void PlatformInterface::dispatcher() {
	bool run = true;
	std::string command;
	while(run) {
		string raw_command = serial_buffer_->ReadLine();
		if (raw_command == "ERROR") {
			sendResponse(RESPONSE::NACK);
		}
		else {
			sendResponse(RESPONSE::ACK, raw_command);
			vector<string> command_string = ParseInput(raw_command);
			if((Commands*)commands_object_ ->findCommand(command_string.at(0))) {
				workQueue_->call(callback(this, &PlatformInterface::CommandsCallback), command_string);
			}
			else {
				sendResponse(RESPONSE::COMMAND_NOT_FOUND);
			}
		}
	}
}

void PlatformInterface::sendResponse(RESPONSE response, const string &message) {
    string response_string;
    switch(response) {
        case RESPONSE::NACK :
	        response_string = bad_json_;
            break;

        case RESPONSE::ACK  :
        	response_string = R"({"ack","payload":{"data_received":"{)";
		    response_string += message;
		    response_string += R"(}"}})";
            break;

        case RESPONSE::COMMAND_NOT_FOUND :
	        response_string = command_not_found_;
            break;

        case RESPONSE::COMMAND_STOPPED :
	        response_string = command_stopped_;
            break;

        case RESPONSE::COMMAND_RUNNING :
	        response_string = command_running_;
            break;

        case RESPONSE::QUEUE_OVERFLOW :
	        response_string = queue_overflowed_;
            break;
    }
	emit(response_string);
}


void PlatformInterface::emit(std::string &device_response) {
	//todo: mutex if doing multithread
	serial_buffer_->SendLine(device_response + '\n');
}

void PlatformInterface::CommandsCallback(vector<string> &command) {
	(Commands*)commands_object_ ->executeCommand(command);
}


