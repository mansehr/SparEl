/*
 * SerialThread.cpp
 *
 *  Created on: Dec 22, 2012
 *      Author: andreas
 */

//#define LOCAL_TEST 1

#include <unistd.h>
#ifndef LOCAL_TEST
#include "serialib.h"
#else
#include "localserialib.h"
#endif
#include "Util.h"
#include "SerialThread.h"

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
void serialThread(bool* running, ThreadsafeQueue* serialworkerQueue) {
	const int BUFFER_SIZE = 64;
#ifndef LOCAL_TEST
	serialib serialPort;                         // Object of the serialib class
#else	
	localserialib serialPort;                         // Object of the serialib class
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
		int toggle = 0;

		// TODO: when is this variable changed, the program must handle signals in some way
		while ((*running) == true) {
			// Read a maximum of 128 characters with a timeout of 0.5 seconds
			// The final character of the string must be a line feed ('\n')
			ret = serialPort.ReadString(buf, '\n', BUFFER_SIZE, 500);
			if (ret > 0) {
				cout << "String read from serial port : " << buf << endl;
				vector<string> bufStrings = Util::split(buf, ':');		
			}

			string* str = NULL;
			if(serialworkerQueue->PopElement(str)) {
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

}
