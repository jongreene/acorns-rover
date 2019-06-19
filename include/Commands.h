//
// Created by uberslam on 4/4/19.
//

#ifndef COMMUNICATOR_COMMANDS_H
#define COMMUNICATOR_COMMANDS_H

#include <iostream>
#include <map>
#include <vector>
#include "em_timer.h"
#include "em_chip.h"
#include "ALS31300.h"
#include <sstream>


class Commands {
public:
	explicit Commands(void *);

	int hallCommand(const std::vector<std::string> &parameters);

	int Drive(const std::vector<std::string> &parameters);
	int Stop();
	int Stop(const std::vector<std::string> &parameters);

	bool findCommand(std::string&);
	int executeCommand(std::vector<std::string>);
	void print(std::string);

	~Commands() = default;
private:
	void hallSample();
	vector<vector<float>> average();
	void* platform_;
	ALS31300* hall_;
	void initHallSensor();
	void initMotorDriver();

	typedef int (Commands::*callables)(const std::vector<std::string>&);
	std::map<std::string, callables> commands;
};

#endif //COMMUNICATOR_COMMANDS_H
