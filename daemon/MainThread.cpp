//============================================================================
// Name        : Simple.cpp
// Author      : Andreas Sehr
// Version     :
// Copyright   : GPL2
// Description : Hello World in C++, Ansi-style
//============================================================================

#define LOCAL_TEST 1

#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include "Util.h"
#include "Threads.h"
#include "ThreadsafeQueue.h"
#include "socket/ServerSocket.h"

using namespace std;
using namespace mansehr;

#define MAIN_THREAD_QUEUE 0
#define TIMER_THREAD_QUEUE 1

typedef string serveroperation_t(const string&);
typedef map<string, serveroperation_t*> operationmap_t;

bool running = true;

Socket* pClientStarting;
int concurrentClients;
long long clients;

ThreadsafeQueue serialworkerQueue(2);

void clientThread();

/**
 * Function returns the first parameter as an int.
 * Check the return value if its zero the cenvertions might be wrong
 */
int getSwitchId(const string &command) {
	vector < string > params = Util::split(command, ' ');
	// Do we have enough information to continue, ie id for the swith we want to turn on
	if (params.size() > 1) {
		// Slot id is parameter 1
		try {
			return stoi(params[1]);
		} catch(exception&) {
			// Ignore error and return zero
		}
	}

	return 0;
}

string onOffFunction(unsigned int switchid, bool turnOn) {
	if (switchid == 0) {
		return "Wrong switch id parameter";
	}

	string* str = alterSwitch(switchid, turnOn);
	serialworkerQueue.PushElement(str, MAIN_THREAD_QUEUE);


	return "Slot " + to_string(switchid) + " turned " + (turnOn ? "on":"off");
}

string onFunction(const string &command) {

	unsigned int switchid = getSwitchId(command);
	return onOffFunction(switchid, true);
}

string offFunction(const string &command) {
	unsigned int switchid = getSwitchId(command);
	return onOffFunction(switchid, false);
}

string statusFunction(const string &command) {
	return "Up and running";
}

int mainThread() {
	ServerSocket socket;

	pClientStarting = NULL;

	socket.create();
	socket.bind(9876);
	socket.listen();

	cout << "Mainthread is listening" << endl;
	while (running) {
		pClientStarting = new Socket;
		if (socket.accept(*pClientStarting)) {
			cout << "Connection" << endl;
			thread clThread(clientThread);
			clThread.detach();
			while (pClientStarting != NULL)
				; // Wait until childthread resets this
			//clientThread(&running, s);
		} else {
			cerr << "Failed to accept connection." << endl;
		}
	}

	return 1;
}

string extractCommand(string str) {
	unsigned int i = 0;
	for (; i < str.length(); ++i) {
		char a = str[i];
		if (a == ' ' || a == '\r' || a == '\n' || a == '\0')
			break;
	}
	if (i < str.length()) {
		str.erase(str.begin() + i, str.end());
	}
	return str;
}

void clientThread() {
	++concurrentClients;
	long long clientId = ++clients;
	Socket* pClient = pClientStarting;
	pClientStarting = NULL;

	operationmap_t operations;
	operations["on"] = onFunction;
	operations["off"] = offFunction;
	operations["status"] = statusFunction;

	cout << "Client " << clientId << " started" << endl;

	do {
		string buf;
		if (!pClient->recv(buf)) {
			break;
		}
		string command = extractCommand(buf);
		cout << "Client " << clientId << ", recieved: " << buf << endl;

		operationmap_t::iterator operation = operations.find(command);
		if (operation != operations.end()) {
			string ret = operation->second(buf) + "\r\n";
			if (!pClient->send(ret)) {
				break;
			}
		} else if (buf == "quit") {
			break;
		} else {
			if (!pClient->send("Operation not found\r\n")) {
				break;
			}
			cerr << "Client " << clientId << ", operation not found: " << buf
					<< endl;
		}
	} while (pClient->is_valid());

	cout << "Client " << clientId << " ended" << endl;

	delete pClient;
	--concurrentClients;
}

int main() {

// Ignorera Sigpipe errors
//signal(SIGPIPE, SIG_IGN);



	cout << "-- MainThread started" << endl;

	thread serThread(serialThread, &running, &serialworkerQueue);
	thread timeThread(timerThread, &running, &serialworkerQueue, TIMER_THREAD_QUEUE);

	mainThread();

	running = false;
	serThread.join();
	timeThread.join();

	cout << "-- MainThread exit" << endl;

	return 0;
} // main
