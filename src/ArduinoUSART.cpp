/*
 * ArduinoUSART.cpp
 *
 *  Created on: 31/01/2015
 *      Author: cams7
 */

#include "ArduinoUSART.h"

namespace SISBARC {

const uint8_t ArduinoUSART::DIGITAL_PIN_VALUE_MAX = 0xFF; //255
const uint16_t ArduinoUSART::ANALOG_PIN_VALUE_MAX = 0x3FF; //1023

ArduinoUSART::ArduinoUSART() :
		ArduinoStatus(), pinValue(0x0000) {
	setEventValue(EXECUTE);
}

ArduinoUSART::ArduinoUSART(status statusValue, pin_type pinType, uint8_t pin,
		uint16_t pinValue) :
		ArduinoStatus(statusValue, EXECUTE, pinType, pin), pinValue(pinValue) {
}

ArduinoUSART::ArduinoUSART(status statusValue, event eventValue,
		pin_type pinType, uint8_t pin, uint16_t pinValue) :
		ArduinoStatus(statusValue, eventValue, pinType, pin), pinValue(pinValue) {

}

ArduinoUSART::~ArduinoUSART() {
	// TODO Auto-generated destructor stub
}

uint16_t ArduinoUSART::getPinValue(void) {
	return pinValue;
}

void ArduinoUSART::setPinValue(uint16_t pinValue) {
	this->pinValue = pinValue;
}

ArduinoUSARTMessage::ArduinoUSARTMessage() :
		ArduinoUSART() {
	setEventValue(MESSAGE);
}

ArduinoUSARTMessage::ArduinoUSARTMessage(status statusValue, pin_type pinType,
		uint8_t pin, uint16_t codeMessage) :
		ArduinoUSART(statusValue, MESSAGE, pinType, pin, codeMessage) {
}

ArduinoUSARTMessage::~ArduinoUSARTMessage() {
	// TODO Auto-generated destructor stub
}

} /* namespace SISBARC */
