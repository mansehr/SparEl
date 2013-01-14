/*
 * Socket.h
 *
 *  Created on: Jan 1, 2013
 *      Author: andreas
 */

#ifndef SOCKET_H_
#define SOCKET_H_


#include "SocketException.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "string.h" // for memset
#include <string>
#include <netinet/in.h>

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 500;
const int MAXRECV = 500;

namespace mansehr {

using namespace std;

class SeverSocket;

class Socket {
public:
	Socket();
	virtual ~Socket();

	// Client initialization
	bool connect(const string host, const int port);

	// Data Transimission
	bool send(const string) const;
	int recv(string&) const;
	const Socket& operator <<(const string&) const;
	const Socket& operator >>(string&) const;

	void set_non_blocking(const bool);

	bool is_valid() const {
		return m_sock != -1;
	}

protected:
	sockaddr_in m_addr;
	int m_sock;

	friend class ServerSocket;

};

} /* namespace mansehr */
#endif /* SOCKET_H_ */
