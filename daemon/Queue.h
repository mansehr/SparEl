/*
 * Queue.h
 *
 *  Created on: Jan 3, 2013
 *      Author: andreas
 */

#ifndef QUEUE_H_
#define QUEUE_H_

namespace mansehr {

//Copyright (c) 2006 Bruno van Dooren
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

//a thread-safe consumer producer queue that uses no synchronization.
//it is as simple as possible for demonstration purposes.
template<class T> class Queue {
private:
	volatile int m_Read;
	volatile int m_Write;
	static const int Size = 10;
	T m_Data[Size];

public:
	Queue() {
		m_Read = 0;
		m_Write = 0;
	}
	virtual ~Queue() {
		// TODO Auto-generated destructor stub
	}

	//push a new element in the circular queue
	bool PushElement(T &element) {
		int nextElement = (m_Write + 1) % Size;
		if (nextElement != m_Read) {
			m_Data[m_Write] = element;
			m_Write = nextElement;
			return true;
		} else
			return false;
	}

	//remove the next element from the circular queue
	bool PopElement(T &element) {
		if (m_Read == m_Write)
			return false;

		int nextElement = (m_Read + 1) % Size;

		element = m_Data[m_Read];
		m_Read = nextElement;
		return true;
	}

	//look at the next element in the circular queue without removing it
	bool PeekElement(T &element) {
		if (m_Read == m_Write)
			return false;

		element = m_Data[m_Read];
		return true;
	}
};

} /* namespace mansehr */
#endif /* QUEUE_H_ */

