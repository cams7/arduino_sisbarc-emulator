/*
 * ThreadController.cpp
 *
 *  Created on: 10/02/2015
 *      Author: cams7
 */

#include "ThreadController.h"

#include "util/Iterator.h"

//#include <Arduino.h>
#include <stdio.h>
#include "util/_arduino_time.h"

namespace SISBARC {

ThreadController::ThreadController(long interval) :
		Thread(), _threads(new List<Thread>) {

	Thread::setInterval(interval);

	printf("New ThreadController\n");
}

ThreadController::~ThreadController() {
	delete _threads;
	printf("Delete ThreadController\n");
}

List<Thread>* ThreadController::getThreads(void) const {
	return _threads;
}

/*
 ThreadController run() (cool stuf)
 */
void ThreadController::run(void) {

	// Run this thread before
	if (Thread::_callback != NULL)
		Thread::_callback(NULL);

	if (!_threads->isEmpty()) {
		long time = millis();

		Iterator<Thread>* i = _threads->iterator();
		while (i->hasNext()) {
			// Object exists? Is enabled? Timeout exceeded?
			Thread* thread = i->next();
			if (thread->shouldRun(time)) {
				thread->run();
				break;
			}
		}
	}

	// ThreadController extends Thread, so we should flag as runned thread
	runned();
}

/*
 List controller (boring part)
 */
void ThreadController::add(Thread* const thread) {
	// Check if the Thread already exists on the array
	if (!_threads->isEmpty()) {
		Iterator<Thread>* i = _threads->iterator();
		while (i->hasNext())
			if (i->next()->getThreadID() == thread->getThreadID())
				return;
	}

	_threads->add(thread);
}

void ThreadController::remove(unsigned long const id) {
	if (!_threads->isEmpty()) {
		Iterator<Thread>* i = _threads->iterator();
		while (i->hasNext()) {
			if (i->next()->getThreadID() == id) {
				i->remove();
				break;
			}
		}
	}
}

void ThreadController::remove(Thread* const thread) {
	remove(thread->getThreadID());
}

void ThreadController::clear(void) {
	if (!_threads->isEmpty()) {
		Iterator<Thread>* i = _threads->iterator();
		while (i->hasNext())
			i->remove();
	}
}

uint16_t ThreadController::size(void) const {
	return _threads->size();
}

Thread* ThreadController::get(uint16_t const& index) const {
	if (!_threads->isEmpty())
		return _threads->get(index);

	return NULL;
}

//ThreadController THREAD_CONTROLLER;

} /* namespace SISBARC */
