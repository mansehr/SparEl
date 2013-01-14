
#include <iostream>
#include <string>
#include "ThreadsafeQueue.h"

namespace mansehr {


using namespace std;

#if defined (_WIN32) || defined( _WIN64)
#define         DEFAULT_DEVICE_PORT             "COM1"                               // COM1 for windows
#endif

#ifdef __linux__
#define         DEFAULT_DEVICE_PORT             "/dev/ttyACM0"                         // ttyACM0 for linux
#endif

/*
 * SerialThread that communicates with the arduino device
 */
void serialThread(bool* running, ThreadsafeQueue *serialworkerQueue);

}
