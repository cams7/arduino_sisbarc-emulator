/*
 * ArduinoPin.cpp
 *
 *  Created on: 13/02/2015
 *      Author: cams7
 */

#include "ArduinoPin.h"

namespace SISBARC {

const pin_type ArduinoPin::DIGITAL = 0x00;
const pin_type ArduinoPin::ANALOG = 0x01;

const uint8_t ArduinoPin::PIN_TYPE_MAX = ANALOG;

const uint8_t ArduinoPin::DIGITAL_PIN_MAX = 0x3F; //63
const uint8_t ArduinoPin::ANALOG_PIN_MAX = 0x0F; //15

const uint8_t ArduinoPin::PINS_DIGITAL_SIZE = 6;
const uint8_t ArduinoPin::PINS_DIGITAL[PINS_DIGITAL_SIZE] =
		{ 2, 4, 7, 8, 12, 13 };

const uint8_t ArduinoPin::PINS_DIGITAL_PWM_SIZE = 6;
const uint8_t ArduinoPin::PINS_DIGITAL_PWM[PINS_DIGITAL_PWM_SIZE] = { 3, 5, 6,
		9, 10, 11 };

const uint8_t ArduinoPin::PINS_ANALOG_SIZE = 6;
const uint8_t ArduinoPin::PINS_ANALOG[PINS_ANALOG_SIZE] = { 0, 1, 2, 3, 4, 5 };

ArduinoPin::ArduinoPin() :
		_pinType(DIGITAL), _pin(0x00) {
}

ArduinoPin::ArduinoPin(pin_type pinType, uint8_t pin) :
		_pinType(pinType), _pin(pin) {
}

ArduinoPin::~ArduinoPin() {
}

pin_type ArduinoPin::getPinType(void) {
	return _pinType;
}

void ArduinoPin::setPinType(pin_type pinType) {
	this->_pinType = pinType;
}

uint8_t ArduinoPin::getPin(void) {
	return _pin;
}

void ArduinoPin::setPin(uint8_t pin) {
	this->_pin = pin;
}
}

