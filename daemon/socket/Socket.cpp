/*
 * Socket.cpp
 *
 *  Created on: Jan 1, 2013
 *      Author: andreas
 */

#include "Socket.h"
#include <errno.h>
#include <fcntl.h>

namespace mansehr {

Socket::Socket() :
		m_sock(-1) {
	memset(&m_addr, 0, sizeof(m_addr));
}

Socket::~Socket() {
	if (is_valid())
		::close(m_sock);
}



bool Socket::send(const string s) const {
	int status = ::send(m_sock, s.c_str(), s.size(), MSG_NOSIGNAL);
	if (status == -1) {
		return false;
	} else {
		return true;
	}
}

int Socket::recv(string& s) const {
	char buf[MAXRECV + 1];

	s = "";

	memset(buf, 0, MAXRECV + 1);

	int status = ::recv(m_sock, buf, MAXRECV, 0);

	if (status == -1) {
		cerr << "status == -1   errno == " << errno
				<< "  in Socket::recv\n";
		return 0;
	} else if (status == 0) {
		return 0;
	} else {
		s = buf;
		return status;
	}
}

const Socket& Socket::operator <<(const string& s) const {
	if (!send(s)) {
		throw SocketException("Could not write to socket.");
	}

	return *this;

}

const Socket& Socket::operator >>(string& s) const {
	if (!recv(s)) {
		throw SocketException("Could not read from socket.");
	}

	return *this;
}

void Socket::set_non_blocking(const bool b) {

	int opts;

	opts = fcntl(m_sock, F_GETFL);

	if (opts < 0) {
		return;
	}

	if (b)
		opts = (opts | O_NONBLOCK);
	else
		opts = (opts & ~O_NONBLOCK);

	fcntl(m_sock, F_SETFL, opts);

}

bool Socket::connect(const string host, const int port) {
	if (!is_valid())
		return false;

	/*m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(port);
*/
	int status = 0;//inet_pton(AF_INET, host.c_str(), &m_addr.sin_addr);
/*
	if (errno == EAFNOSUPPORT)
		return false;

	status = ::connect(m_sock, (sockaddr *) &m_addr, sizeof(m_addr));
*/
	if (status == 0)
		return true;
	else
		return false;
}

} /* namespace mansehr */
