# Semi-Autonomous Rover
This highly configurable, bluetooth enabled rover, is controlled by a Giant Gecko STK3700 development board. Currently, the board is configured to drive four independently timed pwm motor-drivers, handle bi-directional UART communication, and sample external sensor data from custom PCBs or a shared i2c bus.

<img src="https://i1380.photobucket.com/albums/ah174/nibbleoverbyte/6db4ab84-d0be-4a54-b1a0-f3dc903e4d98_zpspbunmdpz.jpeg" width="250" title="Rover on foam pad"></img>
---
## Setup
#### Cloning
> To clone this project, open a terminal and navigate to where you want the repository to reside. Now, enter the command below.
```bash
# clone the repository
$ git clone https://github.com/jongreene/semi-autonomous-rover.git
```
#### Compiling
> Before compiling, you must prepare the project. From the root of the project open a terminal and enter the commands below.
```bash
# create a RELEASE directory
$ mkdir RELEASE

# enter that directory
$ cd RELEASE
```
> The next step is to run the root cmake. You may notice that the first time you run cmake it takes substantially longer. This is due to the cmake build system automatically downloading local copies of the ARM GNU toolchain as well as Mbed OS. The third-party resources are then patched. From the same directory as above, enter the commands below.
```bash
# resolve dependencies and generate build files
$ cmake ..

# compile STK3700 binary
$ make
```
#### Flashing the binary
> I will be using a standalone J-Link JTAG/SWD programmer to flash the resulting binary onto the board. It is possible (and likely easier) to flash the development board using the built in J-Link programmer however I will likely develop my own board and want the methods I use to transfer over. Follow the commands below from a terminal pointed at the build directory.
```bash
# connect to board
$ JLinkExe -device EFM32GG380F1024 -if SWD -speed 4000 -CommanderScript board.jlink

# load the binary onto the STK3700
$ loadbin bin/semi-autonomous-rover.bin, 0x0

# reset and start the board
$ r
$ g
```
> The board should now be executing the binary. This can be tested by sending it a UART command (described below) and verifying you get a response.
---
## Communication
##### UART setup
> The rover, by default, is configured to send and receive data at 9600 baud.
##### Command format
> The newline character `\n` is shown only for clarity but should be generated by your UART client.
```
input: {command_name,param1,param2,...}\n
```
##### Example command
```
input: {drive,40,40,20,20}
```
##### Command responses
There are two types of responses that can be expected when issuing a command. 
* An acknowledgement that a line (\n) of data was received, and whether or not that data was properly formatted and requesting a valid function. 
* A response generated by the function being invoked and can happen as many times as desired, so long as the function is still executing.
##### Command error responses
* bad json: {"nak":"","payload":{"return_value":false,"return_string":"json error: badly formatted json"}}
* command not found: {"notification":"payload":{"return_string":"error: command not found"}}
* command running: {"notification":"payload":{"return_string":"error: command is already running"}}
* command stopped: {"notification":"payload":{"return_string":"error: command is not running"}}
* queue overflowed: {"notification":"payload":{"return_string":"error: queue overflowed"}}
---
## Customization
#### Creating commands
> Through the use of a highly modularized C/C++ framework, defining new callable functions takes little to no understanding of the underlying services. New routines can be implemented in just a few steps, making driver development fast and simple. While this rover is very capable, the underlying framework that drives it is the true bread and butter of this project.
