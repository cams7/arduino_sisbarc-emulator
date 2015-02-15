/*
 * arduino_time.h
 *
 *  Created on: 10/02/2015
 *      Author: cams7
 */

#ifndef ARDUINO_TIME_H_
#define ARDUINO_TIME_H_

#define _POSIX_C_SOURCE 200809L

#include <inttypes.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

inline unsigned long int millis(void) {
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	// Seconds
	time_t s = spec.tv_sec;
	// Milliseconds
	long ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds

	//printf("Current time: %"PRIdMAX".%03ld seconds since the Epoch\n", (intmax_t) s, ms);
	return ((s * 1000) + ms);
}

#endif /* ARDUINO_TIME_H_ */
