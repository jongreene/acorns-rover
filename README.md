<img src="https://i1380.photobucket.com/albums/ah174/nibbleoverbyte/6db4ab84-d0be-4a54-b1a0-f3dc903e4d98_zpspbunmdpz.jpeg" width="250" title="Rover on foam pad"></img>
# Semi-Autonomous Rover
> This highly configurable, bluetooth enabled rover, is controlled by a Giant Gecko STK3700 development board. Currently, the board is configured to drive four independently timed pwm motor-drivers, handle bi-directional UART communication, and sample external sensor data from custom PCBs or a shared i2c bus.

## Initial setup
### Cloning
> When cloning this repo...
#### Cmake build system and it's dependencies
> At runtime, the cmake build system will automatically download a local copy of ARM GNU toolchain as well as a Mbed OS. The toolchain will be configured to be used for compilation. The user-specified version of Mbed OS will be checked out. Once all of the resources have been downloaded, all relavent patches are applied to third-party resources are applied.

## Commands
> Through the use of a highly modularized C/C++ framework, defining new callable functions takes little to no understanding of the underlying services. New routines can be implemented in just a few steps, making driver development fast and simple. While this rover is very capable, the underlying framework that drives it is the true bread and butter of this project.

### Callable command execution
...

### Communication
...

#### UART handling
...



## Device Drivers
...

