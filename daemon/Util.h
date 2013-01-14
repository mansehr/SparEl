/*
 * Util.h
 *
 *  Created on: Jan 3, 2013
 *      Author: andreas
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <vector>
#include <string>
#include <sstream>

//#define LOCAL_TEST 1

namespace mansehr {

class Util {
public:
	Util();
	virtual ~Util();

	static std::vector<std::string> split(const std::string &s, char delim);

protected:
	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
};

} /* namespace mansehr */
#endif /* UTIL_H_ */
