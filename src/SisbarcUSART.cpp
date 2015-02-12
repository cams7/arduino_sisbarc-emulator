/*
 * SisbarcUSART.cpp
 *
 *  Created on: 04/02/2015
 *      Author: cams7
 */
#include "SisbarcUSART.h"

//#include <Arduino.h>
#include "SisbarcProtocol.h"
#include "SisbarcEEPROM.h"

#include "util/Iterator.h"

#include <stdlib.h>

#include "_Serial.h"
#include "ArduinoUSART.h"
//#include <stdio.h>

namespace SISBARC {

CallbackUSART::CallbackUSART(bool (*callback)(ArduinoStatus*)) :
		onRun(callback) {
	//printf("New CallbackUSART\n");
}

CallbackUSART::~CallbackUSART() {
	//printf("Delete CallbackUSART\n");
}

bool CallbackUSART::run(ArduinoStatus* arduino) {
	if (onRun != NULL)
		return onRun(arduino);

	return false;
}

SisbarcUSART::SisbarcUSART() :
		_calls(new List<CallbackUSART>()), _serialData(NULL), _serialDataIndex(
				0x00) {
	//printf("New SisbarcUSART\n");
}

SisbarcUSART::~SisbarcUSART() {
	delete _calls;
	free(_serialData);

	//printf("Delete SisbarcUSART\n");
}

void SisbarcUSART::onRun(bool (*callback)(ArduinoStatus*)) {
	_calls->add(new CallbackUSART(callback));
}

void SisbarcUSART::run(ArduinoStatus* arduino) {
	if (!_calls->isEmpty()) {
		Iterator<CallbackUSART>* i = _calls->iterator();
		while (i->hasNext()) {
			CallbackUSART* callback = i->next();
			if (callback->run(arduino))
				break;
		}
	}
}

void SisbarcUSART::receiveDataBySerial(uint8_t data) {
	uint8_t lastBit = data & 0x80;

	if (lastBit) {
		if (_serialData == NULL)
			_serialData = ((uint8_t*) malloc(
					SisbarcProtocol::TOTAL_BYTES_PROTOCOL));
		*(_serialData) = data;

		_serialDataIndex = 0x01;
	} else if (_serialDataIndex > 0x00 && _serialData != NULL) {
		*(_serialData + _serialDataIndex) = data;

		if (_serialDataIndex == (SisbarcProtocol::TOTAL_BYTES_PROTOCOL - 1)) {
			ArduinoStatus* arduino = receive(_serialData);
			if (arduino != NULL) {
				run(arduino);
				delete arduino;
			}
		} else
			_serialDataIndex++;
	} else {

	}
}

void SisbarcUSART::serialWrite(uint8_t* data) {
	if (data == NULL)
		return;

	//Serial.write(data, SisbarcProtocol::TOTAL_BYTES_PROTOCOL);
	free(data);

}

void SisbarcUSART::send(ArduinoStatus* arduino) {
	if (arduino == NULL)
		return;

	serialWrite(SisbarcProtocol::getProtocol(arduino));
	//free() executado apos run()
}

void SisbarcUSART::sendPinDigital(status statusValue, uint8_t pin,
		bool pinValue) {
	bool pinOk = false;
	for (int8_t i = 0x00; i < ArduinoUSART::PINS_DIGITAL_SIZE; i++)
		if (pin == ArduinoUSART::PINS_DIGITAL[i]) {
			pinOk = true;
			break;
		}

	if (!pinOk)
		for (int8_t i = 0x00; i < ArduinoUSART::PINS_DIGITAL_PWM_SIZE; i++)
			if (pin == ArduinoUSART::PINS_DIGITAL_PWM[i]) {
				pinOk = true;
				break;
			}

	if (!pinOk)
		return;

	serialWrite(
			SisbarcProtocol::getProtocolUSART(statusValue,
					ArduinoUSART::EXECUTE, ArduinoUSART::DIGITAL, pin,
					(pinValue ? 0x0001 : 0x0000)));
}

void SisbarcUSART::sendPinPWM(status statusValue, uint8_t pin,
		uint8_t pinValue) {
	bool pinOk = false;
	for (int8_t i = 0x00; i < ArduinoUSART::PINS_DIGITAL_PWM_SIZE; i++)
		if (pin == ArduinoUSART::PINS_DIGITAL_PWM[i]) {
			pinOk = true;
			break;
		}

	if (!pinOk)
		return;

	if (pinValue < 0x00)
		return;

	if (pinValue > ArduinoUSART::DIGITAL_PIN_VALUE_MAX)
		return;

	serialWrite(
			SisbarcProtocol::getProtocolUSART(statusValue,
					ArduinoUSART::EXECUTE, ArduinoUSART::DIGITAL, pin,
					pinValue));
}

void SisbarcUSART::sendPinAnalog(status statusValue, uint8_t pin,
		uint16_t pinValue) {
	bool pinOk = false;
	for (int8_t i = 0x00; i < ArduinoUSART::PINS_ANALOG_SIZE; i++)
		if (pin == ArduinoUSART::PINS_ANALOG[i]) {
			pinOk = true;
			break;
		}

	if (!pinOk)
		return;

	if (pinValue < 0x00)
		return;

	if (pinValue > ArduinoUSART::ANALOG_PIN_VALUE_MAX)
		return;

	serialWrite(
			SisbarcProtocol::getProtocolUSART(statusValue,
					ArduinoUSART::EXECUTE, ArduinoUSART::ANALOG, pin,
					pinValue));
}

//Recebe protocolo
ArduinoStatus *SisbarcUSART::receive(uint8_t const message[]) {
	return SisbarcProtocol::decode(message);
}

SisbarcUSART SISBARC_USART;

} /* namespace SISBARC */
