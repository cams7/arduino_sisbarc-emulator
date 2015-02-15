/*
 * SisbarcUSART.h
 *
 *  Created on: 04/02/2015
 *      Author: cams7
 */

#ifndef SISBARC_H_
#define SISBARC_H_

#include "ThreadController.h"
#include "vo/ArduinoStatus.h"

namespace SISBARC {

class SisbarcClass: public ThreadController {
private:
	uint8_t* _serialData;
	uint8_t _serialDataIndex;

	uint16_t* _threadIntervals;
	uint8_t _totalThreadIntervals;

	ArduinoStatus *receive(uint8_t const message[]);

	void receiveDataBySerial(ArduinoStatus* const);

	void serialWrite(uint8_t* const);

	int32_t getThreadInterval(ArduinoPin* const, int32_t);

	void onRun(ArduinoPin* const, bool (*callback)(ArduinoStatus*), int32_t);

	void (*_serialWrite)(uint8_t* const);

public:
	SisbarcClass();
	virtual ~SisbarcClass();

	void serialEventRun(uint8_t const);

	void send(ArduinoStatus* const);

	void sendPinDigital(status const, uint8_t const, bool const);
	void sendPinPWM(status const, uint8_t const, uint8_t const);
	void sendPinAnalog(status const, uint8_t const, uint16_t const);

	void addThreadInterval(uint8_t const, uint16_t const);

	// Callback set
	void onRun(pin_type const, uint8_t const, bool (*callback)(ArduinoStatus*),
			int32_t interval = 0x00000000);

	void run(void);

	void onRunSerialWrite(void (*serialWrite)(uint8_t* const));
};

//extern SisbarcClass Sisbarc;

} /* namespace SISBARC */

#endif /* SISBARC_H_ */
