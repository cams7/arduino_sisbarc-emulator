/*
 Thread.h - An runnable object

 Thread is responsable for holding the "action" for something,
 also, it responds if it "should" or "should not" run, based on
 the current time;

 For instructions, go to https://github.com/ivanseidel/ArduinoThread

 Created by Ivan Seidel Gomes, March, 2013.
 Released into the public domain.
 */

#ifndef THREAD_H_
#define THREAD_H_

//#include <Arduino.h>
#include <inttypes.h>
#include <stdlib.h>

namespace SISBARC {

/*
 Uncomment this line to enable ThreadName Strings.

 It might be usefull if you are loging thread with Serial,
 or displaying a list of threads...
 */
// #define USE_THREAD_NAMES	1
class Thread {
protected:
	// Desired interval between runs
	long interval;

	// Last runned time in Ms
	long last_run;

	// Scheduled run in Ms (MUST BE CACHED)
	long _cached_next_run;

	/*
	 IMPORTANT! Run after all calls to run()
	 Updates last_run and cache next run.
	 NOTE: This MUST be called if extending
	 this class and implementing run() method
	 */
	void runned(long time = -1);

	// Callback for run() if not implemented
	void (*_onRun)(void);

public:
	//Thread();
	Thread(void (*callback)(void) = NULL, long _interval = 0);

	virtual ~Thread();

	// If the current Thread is enabled or not
	bool enabled;

	// ID of the Thread (initialized from memory adr.)
	long ThreadID;

#ifdef USE_THREAD_NAMES
	// Thread Name (used for better UI).
	String ThreadName;
#endif

	// Set the desired interval for calls, and update _cached_next_run
	virtual void setInterval(long _interval);

	// Return if the Thread should be runned or not
	virtual bool shouldRun(long time = -1);

	// Callback set
	void onRun(void (*callback)(void));

	// Runs Thread
	virtual void run();

};

} /* namespace SISBARC */

#endif /* THREAD_H_ */
