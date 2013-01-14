/*
 * CommandMap.h
 *
 *  Created on: Jan 2, 2013
 *      Author: andreas
 */

#ifndef COMMANDMAP_H_
#define COMMANDMAP_H_

#include <map>
#include <string>

namespace mansehr {

using namespace std;

typedef string commandFunction_t(const string&);
typedef map<string, commandFunction_t*> functionmap_t;

class CommandMap {
public:
	CommandMap();
	virtual ~CommandMap();
private:
	static functionmap_t* functionmap;
};

} /* namespace mansehr */
#endif /* COMMANDMAP_H_ */
