/*
 * SisbarcUSART.h
 *
 *  Created on: 04/02/2015
 *      Author: cams7
 */

#ifndef SISBARC_H_
#define SISBARC_H_

#include <inttypes.h>

#include "ThreadController.h"
#include "ArduinoStatus.h"

namespace SISBARC {

class SisbarcClass: public ThreadController {
private:
	uint8_t* _serialData;
	uint8_t _serialDataIndex;

	uint16_t* _threadIntervals;
	uint8_t _totalThreadIntervals;

	ArduinoStatus *receive(uint8_t const message[]);

	void receiveDataBySerial(ArduinoStatus* const);

	void serialEventRun(void);

	void serialWrite(uint8_t* const);

	int32_t getThreadInterval(ArduinoPin* const, int32_t);

	void onRun(ArduinoPin* const, bool (*callback)(ArduinoStatus*), int32_t);

public:
	SisbarcClass();
	virtual ~SisbarcClass();

	void send(ArduinoStatus* const);

	void sendPinDigital(status const, uint8_t const, bool const);
	void sendPinPWM(status const, uint8_t const, uint8_t const);
	void sendPinAnalog(status const, uint8_t const, uint16_t const);

	void addThreadInterval(uint8_t const, uint16_t const);

	// Callback set
	void onRun(pin_type const, uint8_t const, bool (*callback)(ArduinoStatus*),
			int32_t interval = 0x00000000);

	void run(void);
};

extern SisbarcClass Sisbarc;

} /* namespace SISBARC */

#endif /* SISBARC_H_ */
