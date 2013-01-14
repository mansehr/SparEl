/*
 * QueryThread.cpp
 *
 *  Created on: Dec 22, 2012
 *      Author: andreas
 */

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "RequestThread.hpp"

void requestThread(bool* running) {
	/* initialize random seed: */
	srand ( time(NULL) );

	cout << "RequestThread: " << running<< endl;
	while ((*running) == true) {
		int randomNumber = rand() % 1000 + 1;
		cout << "RequestThread: " << randomNumber << endl;
		usleep(randomNumber*1000);
	}
}




