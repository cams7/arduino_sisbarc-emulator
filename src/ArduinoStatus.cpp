/*
 * Arduino.cpp
 *
 *  Created on: 21/01/2015
 *      Author: cams7
 */

#include "ArduinoStatus.h"

namespace SISBARC {

const transmitter ArduinoStatus::ARDUINO = 0x00;
const transmitter ArduinoStatus::OTHER_DEVICE = 0x01;

const status ArduinoStatus::SEND = 0x00;
const status ArduinoStatus::SEND_RESPONSE = 0x01;
const status ArduinoStatus::RESPONSE = 0x02;
const status ArduinoStatus::RESPONSE_RESPONSE = 0x03;

const event ArduinoStatus::EXECUTE = 0x00;
const event ArduinoStatus::WRITE = 0x01;
const event ArduinoStatus::READ = 0x02;
const event ArduinoStatus::MESSAGE = 0x03;

const pin_type ArduinoStatus::DIGITAL = 0x00;
const pin_type ArduinoStatus::ANALOG = 0x01;

const uint8_t ArduinoStatus::TRANSMITTER_MAX = OTHER_DEVICE;
const uint8_t ArduinoStatus::STATUS_MAX = RESPONSE_RESPONSE;
const uint8_t ArduinoStatus::EVENT_MAX = MESSAGE;
const uint8_t ArduinoStatus::PIN_TYPE_MAX = ANALOG;

const uint8_t ArduinoStatus::DIGITAL_PIN_MAX = 0x3F; //63
const uint8_t ArduinoStatus::ANALOG_PIN_MAX = 0x0F; //15

const uint8_t ArduinoStatus::PINS_DIGITAL_SIZE = 6;
const uint8_t ArduinoStatus::PINS_DIGITAL[PINS_DIGITAL_SIZE] = { 2, 4, 7, 8, 12,
		13 };

const uint8_t ArduinoStatus::PINS_DIGITAL_PWM_SIZE = 6;
const uint8_t ArduinoStatus::PINS_DIGITAL_PWM[PINS_DIGITAL_PWM_SIZE] = { 3, 5,
		6, 9, 10, 11 };

const uint8_t ArduinoStatus::PINS_ANALOG_SIZE = 6;
const uint8_t ArduinoStatus::PINS_ANALOG[PINS_ANALOG_SIZE] =
		{ 0, 1, 2, 3, 4, 5 };

ArduinoStatus::ArduinoStatus() :
		transmitterValue(ARDUINO), statusValue(SEND), eventValue(EXECUTE), pinType(
				DIGITAL), pin(0x00) {
}

ArduinoStatus::ArduinoStatus(status statusValue, event eventValue,
		pin_type pinType, uint8_t pin) :
		transmitterValue(ARDUINO), statusValue(statusValue), eventValue(
				eventValue), pinType(pinType), pin(pin) {
}

ArduinoStatus::~ArduinoStatus() {
	// TODO Auto-generated destructor stub
}

transmitter ArduinoStatus::getTransmitterValue(void) {
	return transmitterValue;
}

void ArduinoStatus::setTransmitterValue(transmitter transmitterValue) {
	this->transmitterValue = transmitterValue;
}

status ArduinoStatus::getStatusValue(void) {
	return statusValue;
}

void ArduinoStatus::setStatusValue(status statusValue) {
	this->statusValue = statusValue;
}

event ArduinoStatus::getEventValue(void) {
	return eventValue;
}
void ArduinoStatus::setEventValue(event eventValue) {
	this->eventValue = eventValue;
}

pin_type ArduinoStatus::getPinType(void) {
	return pinType;
}

void ArduinoStatus::setPinType(pin_type pinType) {
	this->pinType = pinType;
}

uint8_t ArduinoStatus::getPin(void) {
	return pin;
}

void ArduinoStatus::setPin(uint8_t pin) {
	this->pin = pin;
}

} /* namespace SISBARC */
