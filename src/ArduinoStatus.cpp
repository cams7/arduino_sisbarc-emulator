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

const uint8_t ArduinoStatus::TRANSMITTER_MAX = OTHER_DEVICE;
const uint8_t ArduinoStatus::STATUS_MAX = RESPONSE_RESPONSE;
const uint8_t ArduinoStatus::EVENT_MAX = MESSAGE;

ArduinoStatus::ArduinoStatus() :
		ArduinoPin(), _transmitterValue(ARDUINO), _statusValue(SEND), _eventValue(
				EXECUTE) {
}

ArduinoStatus::ArduinoStatus(status statusValue, event eventValue,
		pin_type pinType, uint8_t pin) :
		ArduinoPin(pinType, pin), _transmitterValue(ARDUINO), _statusValue(
				statusValue), _eventValue(eventValue) {
}

ArduinoStatus::ArduinoStatus(status statusValue, event eventValue,
		ArduinoPin* pin) :
		_transmitterValue(ARDUINO), _statusValue(statusValue), _eventValue(
				eventValue) {
	setPinType(pin->getPinType());
	setPin(pin->getPin());
}

ArduinoStatus::~ArduinoStatus() {
}

transmitter ArduinoStatus::getTransmitterValue(void) {
	return _transmitterValue;
}

void ArduinoStatus::setTransmitterValue(transmitter transmitterValue) {
	this->_transmitterValue = transmitterValue;
}

status ArduinoStatus::getStatusValue(void) {
	return _statusValue;
}

void ArduinoStatus::setStatusValue(status statusValue) {
	this->_statusValue = statusValue;
}

event ArduinoStatus::getEventValue(void) {
	return _eventValue;
}
void ArduinoStatus::setEventValue(event eventValue) {
	this->_eventValue = eventValue;
}

} /* namespace SISBARC */
