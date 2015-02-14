/*
 * Serial.h
 *
 *  Created on: 04/02/2015
 *      Author: cams7
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <inttypes.h>

#include "_IO.h"
#include "util/Stack.h"
#include "ArduinoStatus.h"

namespace SISBARC {

class SerialClass: public IO {
private:
	static char const* const FILE_NAME;
	static const uint8_t TOTAL_VALUES_LINE;
	static const uint8_t TOTAL_CHARS;

	static const uint16_t TIMEOUT_MS;

	static bool validateLine(uint16_t& value, uint16_t* linha,
			uint8_t& countSpaces, uint8_t& countLines);

	Stack<ArduinoStatus>* _status;
	long _lastTime;

	uint8_t* _serialData;
	uint8_t _serialDataIndex;

	void readAllLines(void);

	bool isAvailable(void);

public:
	SerialClass();
	virtual ~SerialClass();

	void write(uint8_t* data, const uint8_t size);
	int16_t read(void);

	bool available(void);
};

extern SerialClass Serial;

} /* namespace SISBARC */

#endif /* SERIAL_H_ */
