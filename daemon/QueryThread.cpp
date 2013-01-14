/*
 * QueryThread.cpp
 *
 *  Created on: Dec 22, 2012
 *      Author: andreas
 */

#include <unistd.h>
#include "QueryThread.hpp"

void queryThread(bool* running) {
	cout << "QueryThread: " << running << endl;
	while ((*running) == true) {
		cout << "QueryThread" << endl;
		usleep(1000*1000);
	}
}




