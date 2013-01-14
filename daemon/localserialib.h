#ifndef LOCALSERIALIB_H
#define LOCALSERIALIB_H

//#define DEBUG_LOCALSERIALLIB 1

#include <iostream>

using namespace std;

class localserialib {
private:
	/**
	 * Write string str to the char array buf;
	 */
	int writeBuf(char* buf, string str) {
		unsigned int i = 0;
		for(; i < str.length(); ++i) {
			buf[i] = str[i];
		}
		buf[i] = '\0';
		return i;
	}

public:
	// Constructor of the class
	localserialib() {
#ifdef DEBUG_LOCALSERIALLIB
		cout << "LocalSerialLib: Created " << endl;
#endif
	}

	// Destructor
	~localserialib() {
#ifdef DEBUG_LOCALSERIALLIB
		cout << "LocalSerialLib: Destroyed " << endl;
#endif
	}

	char Open(const char *Device, const unsigned int Bauds) {
#ifdef DEBUG_LOCALSERIALLIB
		cout << "LocalSerialLib: Open, " << Device << ", " << Bauds << endl;
#endif
		return 1;
	}

	// Close the current device
	void Close() {
#ifdef DEBUG_LOCALSERIALLIB
		cout << "LocalSerialLib: Close, " << endl;
#endif
	}

	// Write a char
	char WriteChar(char a) {
#ifdef DEBUG_LOCALSERIALLIB
		cout << "LocalSerialLib: WriteChar, " << a << endl;
#endif
		return a;
	}

	// Read a char (with timeout)
	char ReadChar(char *pByte, const unsigned int TimeOut_ms = 0) {
#ifdef DEBUG_LOCALSERIALLIB
		cout << "LocalSerialLib: ReadChar, " << TimeOut_ms << endl;
#endif
		return 1;
	}

	// Write a string
	char WriteString(const char *String) {
#ifdef DEBUG_LOCALSERIALLIB
		cout << "LocalSerialLib: WriteChar, " << String << endl;
#endif
		return 1;
	}

	static unsigned int timer;

	// Read a string (with timeout)
	int ReadString(char *String, char FinalChar, unsigned int MaxNbBytes,
			const unsigned int TimeOut_ms = 0) {
#ifdef DEBUG_LOCALSERIALLIB
		cout << "LocalSerialLib: ReadString " << String << ", " << FinalChar
				<< ", " << MaxNbBytes << ", " << TimeOut_ms << endl;
#endif
		if ((++timer % 5) == 0) {
			return writeBuf(String, "b:9\n");
		} else if ((++timer % 4) == 0) {
			return writeBuf(String, "d:1232\n");
		} else {
			String[0] = '\0';
			return 0;
		}
	}

	// Write an array of bytes
	char Write(const void *Buffer, const unsigned int NbBytes);

	// Read an array of byte (with timeout)
	int Read(void *Buffer, unsigned int MaxNbBytes,
			const unsigned int TimeOut_ms = 0);

	// _________________________
	// ::: Special operation :::

	// Empty the received buffer
	void FlushReceiver();

	// Return the number of bytes in the received buffer
	int Peek();

};

unsigned int localserialib::timer = 1;

#endif // SERIALIB_H
