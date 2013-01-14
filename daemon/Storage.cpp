/*
 * Storage.cpp
 *
 *  Created on: Jan 9, 2013
 *      Author: andreas
 */

#include "Storage.h"

namespace mansehr {

Storage::Storage() {
	// TODO Auto-generated constructor stub

}

Storage::~Storage() {
	// TODO Auto-generated destructor stub
}

void Storage::storeBlips(unsigned long blip) {
	ofstream blipsFile;
	blipsFile.open(BLIPS_FILENAME, ios::out | ios::app);
	blipsFile << this->getCurrentTimeAsString() << ";" << blip << endl;
	blipsFile.close();

}

void Storage::storeDelta(unsigned long delta) {
	ofstream blipsFile;
	blipsFile.open(DELTA_FILENAME, ios::out | ios::app);
	blipsFile << this->getCurrentTimeAsString() << ";" << this->calculateKWhFromDelta(delta) << endl;
	blipsFile.close();
}

double Storage::calculateKWhFromDelta(unsigned long delta) {
	const long long blips_per_hour = MSEC_IMPS;
	if (delta > 0)
		return blips_per_hour / (double)delta;
	return 0;
}

string Storage::getCurrentTimeAsString() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[32];
	tstruct = *localtime(&now);
	// Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

	return buf;
}

} /* namespace mansehr */
