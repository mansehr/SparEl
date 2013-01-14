/*
 * SocketException.h
 *
 *  Created on: Jan 2, 2013
 *      Author: andreas
 */

#ifndef SOCKETEXCEPTION_H_
#define SOCKETEXCEPTION_H_

#include <string>

using namespace std;

class SocketException {
public:
	SocketException(string s) :
			exString(s) {
	}
	;
	~SocketException() {
	}
	;

	string description() {
		return exString;
	}

private:

	string exString;

};

#endif /* SOCKETEXCEPTION_H_ */
