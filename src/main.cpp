#include "mbed.h"
#include "PlatformInterface.h"
#include "Commands.h"
#include "Connector.h"
#include "em_emu.h"

//// Required to fix link error inside arm mbed-os
// fpv4-sp/softfp/libc.a(lib_a-signalr.o): In function `_getpid_r'
// fpv4-sp/softfp/libc.a(lib_a-signalr.o): In function `_kill_r'
extern "C"{
int _getpid(){return 0;}
int _kill(){return 0;}
}

DigitalOut bcEnablePin(PF7);

int main() {
//    //Disable the board controller serial connection
//    bcEnablePin = 0;
//
//	auto communicator = new PlatformInterface();

	auto connection = new Connector();

	while (1)
	{
		/* On every wakeup enter EM2 again */
		EMU_EnterEM2(true);
	}

	return 0;
}
