/*
 * ServerSocket.h
 *
 *  Created on: Jan 1, 2013
 *      Author: andreas
 */

#ifndef SERVERSOCKET_H_
#define SERVERSOCKET_H_

#include "Socket.h"

namespace mansehr {

class ServerSocket : protected Socket {
public:
	ServerSocket();
	virtual ~ServerSocket();

	// Server socket initialization
	bool create();
	bool bind(const int port);
	bool listen() const;
	bool accept(Socket&) const;


	// Connect is not used by a ServerSocket
	bool connect(const string host, const int port) =delete;

};

} /* namespace mansehr */
#endif /* SERVERSOCKET_H_ */
