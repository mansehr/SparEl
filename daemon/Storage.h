/*
 * Storage.h
 *
 *  Created on: Jan 9, 2013
 *      Author: andreas
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include <string>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include "Util.h"

namespace mansehr {

using namespace std;

#ifdef LOCAL_TEST
#define BLIPS_FILENAME "./blipsperminute.txt"
#define DELTA_FILENAME "./sparElData.txt"
#else
#define BLIPS_FILENAME "/var/log/blipsperminute.txt"
#define DELTA_FILENAME "/var/log/sparElData.txt"
#endif

#define IMPS_KWh 1000
// Milliseconds per hour * 1000 / pulses per Kwh
#define MSEC_IMPS (60*60*1000) //*1000/IMPS_KWh)

class Storage {
public:
	Storage();
	virtual ~Storage();

	void storeBlips(unsigned long blip);
	void storeDelta(unsigned long delta);
private:
	double calculateKWhFromDelta(unsigned long delta);
	string getCurrentTimeAsString();
};

} /* namespace mansehr */
#endif /* STORAGE_H_ */
