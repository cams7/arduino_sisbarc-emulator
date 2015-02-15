/*
 * ArduinoProtocol.h
 *
 *  Created on: 14/01/2015
 *      Author: cams7
 */

#ifndef SISBARCPROTOCOL_H_
#define SISBARCPROTOCOL_H_

#include <inttypes.h>

#include "vo/ArduinoStatus.h"

namespace SISBARC {

class SisbarcProtocol {
private:
	static const uint32_t EMPTY_BITS;

	static const uint8_t TOTAL_BITS_PROTOCOL;

	static const uint8_t TOTAL_BITS_INDEX;
	static const uint8_t TOTAL_BITS_CHECKSUM;
	static const uint8_t TOTAL_BITS_DATA;

	static const uint8_t TOTAL_BITS_DIGITAL_PIN;
	static const uint8_t TOTAL_BITS_ANALOG_PIN;

	static const uint8_t TOTAL_BITS_DIGITAL_PIN_VALUE;
	static const uint8_t TOTAL_BITS_ANALOG_PIN_VALUE;

	static const uint8_t TOTAL_BITS_THREAD_TIME;

	static const uint8_t TOTAL_BITS_DIGITAL_ACTION_EVENT;
	static const uint8_t TOTAL_BITS_ANALOG_ACTION_EVENT;

	static uint32_t encode(ArduinoStatus* arduino);

public:
	static const uint8_t TOTAL_BYTES_PROTOCOL;

	static uint8_t *getProtocol(ArduinoStatus* arduino);

	static uint8_t *getProtocolUSART(status statusValue, event eventValue,
			pin_type pinType, uint8_t pin, uint16_t pinValue);

	static uint8_t *getProtocolEEPROM(status statusValue, event eventValue,
			pin_type pinType, uint8_t pin, uint8_t threadTime,
			uint8_t actionEvent);

	static ArduinoStatus *decode(uint8_t const message[]);
};

} /* namespace SISBARC */

#endif /* SISBARCPROTOCOL_H_ */
