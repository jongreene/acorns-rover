#include "mbed.h"
#include "program_main.h"

//// Required to fix link error inside arm mbed-os
// fpv4-sp/softfp/libc.a(lib_a-signalr.o): In function `_getpid_r'
// fpv4-sp/softfp/libc.a(lib_a-signalr.o): In function `_kill_r'
extern "C"{
int _getpid(){return 0;}
int _kill(){return 0;}
}

DigitalOut bcEnablePin(PF7);

int main() {
    //Disable the board controller serial connection
    bcEnablePin = 0;

	auto communicator = new PlatformInterface();

	return 0;
}
