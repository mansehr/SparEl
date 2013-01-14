/*
 * ThreadsafeQueue.h
 *
 *  Created on: Jan 3, 2013
 *      Author: andreas
 */

#ifndef THREADSAFEQUEUE_H_
#define THREADSAFEQUEUE_H_

#include <vector>
#include <tuple>
#include <climits>
#include "Queue.h"

namespace mansehr {

using namespace std;

typedef unsigned long long idcounter_t;

class ThreadsafeQueue {
private:
	class QueueNode {
	public:
		QueueNode() :
				id(0), element(NULL) {

		}
		QueueNode(idcounter_t inId, string* inStr) :
				id(inId), element(inStr) {

		}

		idcounter_t getId() const {
			return id;
		}

		string* getElement() const {
			return element;
		}

		QueueNode& operator=(const QueueNode &rhs) {
			// Only do assignment if RHS is a different object from this.
			if (this != &rhs) {
				this->id = rhs.id;
				this->element = rhs.element;
			}
			return *this;
		}

		volatile QueueNode& operator=(const volatile QueueNode &rhs) {
			// Only do assignment if RHS is a different object from this.
			if (this != &rhs) {
				this->id = rhs.id;
				this->element = rhs.element;
			}
			return *this;
		}

	private:
		idcounter_t id;
		string* element;
	};

	volatile idcounter_t nextid;
	Queue<QueueNode>* queues;
	unsigned int nofQueues;

public:
	ThreadsafeQueue(const unsigned int uiNofQueues) {
		nofQueues = uiNofQueues;
		queues = new Queue<QueueNode> [nofQueues];
		nextid = 0;
	}
	virtual ~ThreadsafeQueue() {
		delete this->queues;
	}

	//push a new element in the circular queue
	bool PushElement(string* &element, unsigned int queueid) {
		if (queueid < nofQueues) {
			QueueNode t(++nextid, element);
			return queues[queueid].PushElement(t);
		}
		return false;
	}

	//remove the next element from the circualr queue
	bool PopElement(string* &element) {
		unsigned int nextQueue = 0;
		idcounter_t lowestId = ULONG_MAX;
		for (unsigned int i = 0; i < nofQueues; ++i) {
			QueueNode t;
			if (queues[i].PeekElement(t)) {
				if (lowestId > t.getId()) {
					nextQueue = i;
					lowestId = t.getId();
				}
			}
		}
		QueueNode t;
		if (queues[nextQueue].PopElement(t)) {
			element = t.getElement();
			return true;
		}
		return false;
	}
};

} /* namespace mansehr */
#endif /* THREADSAFEQUEUE_H_ */
