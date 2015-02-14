/*
 * SisbarcUSART.cpp
 *
 *  Created on: 04/02/2015
 *      Author: cams7
 */
#include "Sisbarc.h"

#include "SisbarcProtocol.h"
#include "SisbarcEEPROM.h"

#include "ArduinoUSART.h"
#include "ArduinoEEPROM.h"

#include "util/Iterator.h"

//#include <Arduino.h>
#include "_Serial.h"

#include <stdlib.h>
#include <stdio.h>

namespace SISBARC {

SisbarcClass::SisbarcClass() :
		_serialData((uint8_t*) malloc(SisbarcProtocol::TOTAL_BYTES_PROTOCOL)), _serialDataIndex(
				0x00), _threadIntervals(
				(uint16_t*) malloc(ArduinoEEPROM::THREAD_INTERVAL_MAX + 1)), _totalThreadIntervals(
				0x00) {
	printf("New Sisbarc\n");
}

SisbarcClass::~SisbarcClass() {
	free(_serialData);
	free(_threadIntervals);

	printf("Delete Sisbarc\n");
}

//Recebe protocolo
ArduinoStatus *SisbarcClass::receive(uint8_t const message[]) {
	return SisbarcProtocol::decode(message);
}

void SisbarcClass::receiveDataBySerial(ArduinoStatus* const arduino) {
	if (!ThreadController::getThreads()->isEmpty()) {
		Iterator<Thread>* i = ThreadController::getThreads()->iterator();
		while (i->hasNext()) {
			// Object exists? Is enabled? Timeout exceeded?
			Thread* thread = i->next();
			bool executeBySerial = thread->run(arduino);

			if (executeBySerial) {
				int32_t interval = getThreadInterval((ArduinoPin*) arduino, -1);

				if (interval > 0) {
					thread->setInterval(interval);
					thread->setEnabled(true);
					printf("interval thread now is %i\n", interval);
				} else if (interval == 0) {
					thread->setEnabled(false);
					printf("thread now is not enabled\n");
				} else
					printf("interval not found in EEPROM\n");

				break;
			}
		}
	}
}

void SisbarcClass::serialEventRun(void) {
	uint8_t data = (uint8_t) Serial.read(); //lê os dados da porta serial - Maximo 64 bytes

	if (data & 0x80) { //Last bit
		*(_serialData) = data;
		_serialDataIndex = 0x01;
	} else if (_serialDataIndex > 0x00) {
		*(_serialData + _serialDataIndex) = data;
		_serialDataIndex++;

		if (_serialDataIndex == SisbarcProtocol::TOTAL_BYTES_PROTOCOL) {
			ArduinoStatus* arduino = receive(_serialData);
			if (arduino != NULL) {
				receiveDataBySerial(arduino);
				delete arduino;
			}
			_serialDataIndex = 0x00;
		}
	}
}

void SisbarcClass::serialWrite(uint8_t* const data) {
	if (data == NULL)
		return;

	Serial.write(data, SisbarcProtocol::TOTAL_BYTES_PROTOCOL);
	free(data);
}

void SisbarcClass::send(ArduinoStatus* const arduino) {
	if (arduino == NULL)
		return;

	serialWrite(SisbarcProtocol::getProtocol(arduino));
}

void SisbarcClass::sendPinDigital(status const statusValue, uint8_t const pin,
		bool const pinValue) {
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

void SisbarcClass::sendPinPWM(status const statusValue, uint8_t const pin,
		uint8_t const pinValue) {
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

void SisbarcClass::sendPinAnalog(status const statusValue, uint8_t const pin,
		uint16_t const pinValue) {
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

int32_t SisbarcClass::getThreadInterval(ArduinoPin* const pin,
		int32_t interval) {
	if (_totalThreadIntervals > 0x00) {
		EEPROMData* data = SisbarcEEPROM::read(pin);

		if (data != NULL) {
			uint8_t index = data->getThreadInterval();
			if (_totalThreadIntervals > index)
				interval = _threadIntervals[index];

			delete data;
		}
	}
	return interval;
}

void SisbarcClass::addThreadInterval(uint8_t const index,
		uint16_t const interval) {
	if (index <= ArduinoEEPROM::THREAD_INTERVAL_MAX) {
		_threadIntervals[index] = interval;
		_totalThreadIntervals++;
	}
}

void SisbarcClass::onRun(ArduinoPin* const pin,
		bool (*callback)(ArduinoStatus*), int32_t interval) {

	interval = getThreadInterval(pin, interval);

	Thread* thread = new Thread(callback, interval);
	if (interval > 0)
		thread->setEnabled(true);
	else
		thread->setEnabled(false);

	ThreadController::add(thread);
}

void SisbarcClass::onRun(pin_type const pinType, uint8_t const pin,
		bool (*callback)(ArduinoStatus*), int32_t interval) {
	ArduinoPin* arduinoPin = new ArduinoPin(pinType, pin);
	onRun(arduinoPin, callback, interval);
	delete arduinoPin;
}

void SisbarcClass::run(void) {
	ThreadController::run();

	if (Serial.available() > 0) //verifica se existe comunicação com a porta serial
		serialEventRun();
}

SisbarcClass Sisbarc;

} /* namespace SISBARC */
