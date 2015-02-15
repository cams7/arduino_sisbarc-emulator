/*
 * ArduinoEEPROM.cpp
 *
 *  Created on: 31/01/2015
 *      Author: cams7
 */

#include "ArduinoEEPROM.h"

namespace SISBARC {

ArduinoEEPROM::ArduinoEEPROM() :
		ArduinoStatus(), EEPROMData() {
}

ArduinoEEPROM::ArduinoEEPROM(status statusValue, event eventValue,
		pin_type pinType, uint8_t pin, uint8_t threadInterval,
		uint8_t actionEvent) :
		ArduinoStatus(statusValue, eventValue, pinType, pin), EEPROMData(
				threadInterval, actionEvent) {
}

ArduinoEEPROM::ArduinoEEPROM(status statusValue, event eventValue,
		pin_type pinType, uint8_t pin, EEPROMData* data) :
		ArduinoStatus(statusValue, eventValue, pinType, pin) {
	setThreadInterval(data->getThreadInterval());
	setActionEvent(data->getActionEvent());
}

ArduinoEEPROM::ArduinoEEPROM(status statusValue, event eventValue,
		ArduinoPin* pin, uint8_t threadInterval, uint8_t actionEvent) :
		ArduinoStatus(statusValue, eventValue, pin), EEPROMData(threadInterval,
				actionEvent) {

}
ArduinoEEPROM::ArduinoEEPROM(status statusValue, event eventValue,
		ArduinoPin* pin, EEPROMData* data) :
		ArduinoStatus(statusValue, eventValue, pin) {
	setThreadInterval(data->getThreadInterval());
	setActionEvent(data->getActionEvent());
}

ArduinoEEPROM::~ArduinoEEPROM() {
}

ArduinoEEPROMRead::ArduinoEEPROMRead() :
		ArduinoEEPROM() {
	setEventValue(READ);
}

ArduinoEEPROMRead::ArduinoEEPROMRead(status statusValue, pin_type pinType,
		uint8_t pin, uint8_t threadInterval, uint8_t actionEvent) :
		ArduinoEEPROM(statusValue, READ, pinType, pin, threadInterval,
				actionEvent) {
}

ArduinoEEPROMRead::ArduinoEEPROMRead(status statusValue, pin_type pinType,
		uint8_t pin, EEPROMData* data) :
		ArduinoEEPROM(statusValue, READ, pinType, pin, data) {
}

ArduinoEEPROMRead::ArduinoEEPROMRead(status statusValue, ArduinoPin* pin,
		uint8_t threadInterval, uint8_t actionEvent) :
		ArduinoEEPROM(statusValue, READ, pin, threadInterval, actionEvent) {

}
ArduinoEEPROMRead::ArduinoEEPROMRead(status statusValue, ArduinoPin* pin,
		EEPROMData* data) :
		ArduinoEEPROM(statusValue, READ, pin, data) {
}

ArduinoEEPROMRead::~ArduinoEEPROMRead() {
}

ArduinoEEPROMWrite::ArduinoEEPROMWrite() :
		ArduinoEEPROM() {
	setEventValue(WRITE);
}

ArduinoEEPROMWrite::ArduinoEEPROMWrite(status statusValue, pin_type pinType,
		uint8_t pin, uint8_t threadInterval, uint8_t actionEvent) :
		ArduinoEEPROM(statusValue, WRITE, pinType, pin, threadInterval,
				actionEvent) {
}

ArduinoEEPROMWrite::ArduinoEEPROMWrite(status statusValue, pin_type pinType,
		uint8_t pin, EEPROMData* data) :
		ArduinoEEPROM(statusValue, WRITE, pinType, pin, data) {
}

ArduinoEEPROMWrite::ArduinoEEPROMWrite(status statusValue, ArduinoPin* pin,
		uint8_t threadInterval, uint8_t actionEvent) :
		ArduinoEEPROM(statusValue, WRITE, pin, threadInterval, actionEvent) {
}

ArduinoEEPROMWrite::ArduinoEEPROMWrite(status statusValue, ArduinoPin* pin,
		EEPROMData* data) :
		ArduinoEEPROM(statusValue, WRITE, pin, data) {
}

ArduinoEEPROMWrite::~ArduinoEEPROMWrite() {
}

} /* namespace SISBARC */
