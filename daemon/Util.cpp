/*
 * Util.cpp
 *
 *  Created on: Jan 3, 2013
 *      Author: andreas
 */

#include "Util.h"

namespace mansehr {

Util::Util() {
	// TODO Auto-generated constructor stub

}

Util::~Util() {
	// TODO Auto-generated destructor stub
}

std::vector<std::string> &Util::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	    std::stringstream ss(s);
	    std::string item;
	    while(std::getline(ss, item, delim)) {
	        elems.push_back(item);
	    }
	    return elems;
	}


	std::vector<std::string> Util::split(const std::string &s, char delim) {
	    std::vector<std::string> elems;
	    split(s, delim, elems);
	    return elems;
	}

} /* namespace mansehr */


