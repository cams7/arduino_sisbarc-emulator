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
		ArduinoStatus(), _pinValue(0x0000) {
	setEventValue(EXECUTE);
}

ArduinoUSART::ArduinoUSART(status statusValue, pin_type pinType, uint8_t pin,
		uint16_t pinValue) :
		ArduinoStatus(statusValue, EXECUTE, pinType, pin), _pinValue(pinValue) {
}

ArduinoUSART::ArduinoUSART(status statusValue, event eventValue,
		pin_type pinType, uint8_t pin, uint16_t pinValue) :
		ArduinoStatus(statusValue, eventValue, pinType, pin), _pinValue(
				pinValue) {
}

ArduinoUSART::ArduinoUSART(status statusValue, ArduinoPin* pin,
		uint16_t pinValue) :
		ArduinoStatus(statusValue, EXECUTE, pin), _pinValue(pinValue) {

}
ArduinoUSART::ArduinoUSART(status statusValue, event eventValue,
		ArduinoPin* pin, uint16_t pinValue) :
		ArduinoStatus(statusValue, eventValue, pin), _pinValue(pinValue) {

}

ArduinoUSART::~ArduinoUSART() {
}

uint16_t ArduinoUSART::getPinValue(void) {
	return _pinValue;
}

void ArduinoUSART::setPinValue(uint16_t pinValue) {
	this->_pinValue = pinValue;
}

ArduinoUSARTMessage::ArduinoUSARTMessage() :
		ArduinoUSART() {
	setEventValue(MESSAGE);
}

ArduinoUSARTMessage::ArduinoUSARTMessage(status statusValue, pin_type pinType,
		uint8_t pin, uint16_t codeMessage) :
		ArduinoUSART(statusValue, MESSAGE, pinType, pin, codeMessage) {
}

ArduinoUSARTMessage::ArduinoUSARTMessage(status statusValue, ArduinoPin* pin,
		uint16_t codeMessage) :
		ArduinoUSART(statusValue, MESSAGE, pin, codeMessage) {
}

ArduinoUSARTMessage::~ArduinoUSARTMessage() {
}

} /* namespace SISBARC */
