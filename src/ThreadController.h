/*
 ThreadController.h - Controlls a list of Threads with different timings

 Basicaly, what it does is to keep track of current Threads and run when
 necessary.

 ThreadController is an extended class of Thread, because of that,
 it allows you to add a ThreadController inside another ThreadController...

 For instructions, go to https://github.com/ivanseidel/ArduinoThread

 Created by Ivan Seidel Gomes, March, 2013.
 Released into the public domain.
 */

#ifndef THREADCONTROLLER_H_
#define THREADCONTROLLER_H_

#include "Thread.h"
#include "util/List.h"

namespace SISBARC {

class ThreadController: public Thread {
private:
	List<Thread>* _threads;

protected:
	List<Thread>* getThreads(void) const;

public:
	ThreadController(long interval = 0);

	virtual ~ThreadController();

	// run() Method is overrided
	virtual void run(void);

	// Adds a thread in the first available slot (remove first)
	// Returns if the Thread could be added or not
	void add(Thread* const);

	// remove the thread (given the Thread* or ThreadID)
	void remove(unsigned long const);
	void remove(Thread* const);

	// Removes all threads
	void clear(void);

	// Return the quantity of Threads
	uint16_t size(void) const;

	// Return the I Thread on the array
	// Returns NULL if none found
	Thread* get(uint16_t const&) const;
};

//extern ThreadController THREAD_CONTROLLER;

} /* namespace SISBARC */

#endif /* THREADCONTROLLER_H_ */
