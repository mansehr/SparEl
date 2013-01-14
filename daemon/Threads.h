
#include <iostream>
#include <string>
#include "Util.h"
#include "Storage.h"
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
void serialThread(const bool*, ThreadsafeQueue*);

void handleRecievedSerialData(const char* );
/*
 * Timer thread that controlls different timing events
 */
void timerThread(const bool*, ThreadsafeQueue* , const unsigned int);

/**
 * Create the string to send to arduino with given parameters
 */
string* alterSwitch(const unsigned int switchId, const bool on);
}
