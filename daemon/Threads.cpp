/*
 * SerialThread.cpp
 *
 *  Created on: Dec 22, 2012
 *      Author: andreas
 */

#include <unistd.h>
#include "Util.h"
#include "Threads.h"

#ifndef LOCAL_TEST
#include "serialib.h"
#else
#include "localserialib.h"
#endif

namespace mansehr {

int sec = 1;

#define RESTART_THREAD \
	cerr << "SerialThread sleeps for "<< sec << " seconds" << endl; \
	if(sec < 10) ++sec; \
	usleep(sec*1000*1000); \
	continue;

/*
 * The mainThread function will do the following:
 * Connect to arduino
 * 1. Wait for data
 * 2. pop data from serialQue
 * 3. if data send it to arduino
 * 4. loop to 1
 * exit program on break
 */
void serialThread(const bool* running, ThreadsafeQueue* serialworkerQueue) {
	const int BUFFER_SIZE = 64;
#ifndef LOCAL_TEST
	serialib serialPort;                         // Object of the serialib class
#else
	localserialib serialPort;                    // Object of the serialib class
#endif
	int ret;                                           // Used for return values
	char buf[BUFFER_SIZE];

	while ((*running) == true) {
		// Initialize connection
		// TODO: make the port configurable
		if (serialPort.Open(DEFAULT_DEVICE_PORT, 115200) != 1) { // Open serial link at 115200 bauds                                                           // If an error occured...
			cerr << "Error while opening port. Permission problem ?" << endl;
			RESTART_THREAD
		}
		cout << "Serial port opened successfully !" << endl;

		// Write the AT command on the serial port
		if (serialPort.WriteString("AT\n") != 1) { // If the writting operation failed ...
			cerr << "Error while writing data" << endl;
			RESTART_THREAD
		}

		// TODO: when is this variable changed, the program must handle signals in some way
		while ((*running) == true) {
			// Read a maximum of 128 characters with a timeout of 0.1 seconds
			// The final character of the string must be a line feed ('\n')
			ret = serialPort.ReadString(buf, '\n', BUFFER_SIZE, 100);
			if (ret > 0) {
				cout << "String read from serial port : " << buf << endl;
				//string strBuf(buf);
				handleRecievedSerialData(buf);

			}

			string* str = NULL;
			if (serialworkerQueue->PopElement(str)) {
				cout << "Writing data: " << *str << endl;
				if (serialPort.WriteString(str->c_str()) != 1) { // If the writting operation failed ...
					cerr << "Error while writing data" << endl;
					RESTART_THREAD
				}
				delete str;
			}

			/*toggle = 1 - toggle;
			 char data[] = { '0', '\0' };
			 data[0] += toggle;
			 cout << "Writing data: " << data << endl;
			 if (serialPort.WriteString(data) != 1) { // If the writting operation failed ...
			 cerr << "Error while writing data" << endl;
			 RESTART_THREAD
			 }*/
			sec = 1;
#ifdef LOCAL_TEST
			// Sleep a sec in testmode
			RESTART_THREAD
#endif
		}
	}

	// Close the connection with the device
	serialPort.Close();
}

void handleRecievedSerialData(const char* buf) {
	Storage storage;
	// Split string by ':'
	vector < string > strArray = Util::split(buf, ':');
	if (strArray.size() == 2 && (strArray[0]).length() == 1) {
		long value = stol(strArray[1]);
		char func = strArray[0][0];
		// Parse first char
		switch (strArray[0][0]) {
		// Blips
		case 'b':
			cout << "str1 b: " << strArray[1] << endl;
			storage.storeBlips(value);
			break;
			// Delta
		case 'd':
			cout << "str1 d: " << strArray[1] << endl;
			storage.storeDelta(value);
			break;
			// unhandled
		default:
			cerr << "Recieved unhandled function over serial connection" << func
					<< endl;
			break;
		}
	} else {
		cerr << "Recieved unhandled data over serial connection" << buf << endl;
	}
	/*cout << "str0: " << strArray[0] << endl;
	 cout << "str1: " << strArray[1] << endl;*/
}

void timerThread(const bool* running, ThreadsafeQueue* queue,
		const unsigned int queueId) {
	bool toggle = false;
	string* str = NULL;
	while ((*running) == true) {
		toggle = !toggle;
		str = alterSwitch(0, toggle);
		queue->PushElement(str, queueId);
		usleep(2000 * 1000);
	}
}

string* alterSwitch(const unsigned int switchId, const bool on) {

	// The least significant bit tells if the swicth should be turned on (1) or off (0)
	unsigned int value = (switchId << 1) | on;

	return new string(to_string(value));
}

}
