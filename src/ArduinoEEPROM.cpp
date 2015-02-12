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
		pin_type pinType, uint8_t pin, uint8_t threadTime, uint8_t actionEvent) :
		ArduinoStatus(statusValue, eventValue, pinType, pin), EEPROMData(
				threadTime, actionEvent) {
}

ArduinoEEPROM::ArduinoEEPROM(status statusValue, event eventValue,
		pin_type pinType, uint8_t pin, EEPROMData* data) :
		ArduinoStatus(statusValue, eventValue, pinType, pin) {
	setThreadTime(data->getThreadTime());
	setActionEvent(data->getActionEvent());
}

ArduinoEEPROM::~ArduinoEEPROM() {
}

ArduinoEEPROMRead::ArduinoEEPROMRead() :
		ArduinoEEPROM() {
	setEventValue(READ);
}

ArduinoEEPROMRead::ArduinoEEPROMRead(status statusValue, pin_type pinType,
		uint8_t pin, uint8_t threadTime, uint8_t actionEvent) :
		ArduinoEEPROM(statusValue, READ, pinType, pin, threadTime, actionEvent) {
}

ArduinoEEPROMRead::ArduinoEEPROMRead(status statusValue, pin_type pinType,
		uint8_t pin, EEPROMData* data) :
		ArduinoEEPROM(statusValue, READ, pinType, pin, data) {

}

ArduinoEEPROMRead::~ArduinoEEPROMRead() {
	// TODO Auto-generated destructor stub
}

ArduinoEEPROMWrite::ArduinoEEPROMWrite() :
		ArduinoEEPROM() {
	setEventValue(WRITE);
}

ArduinoEEPROMWrite::ArduinoEEPROMWrite(status statusValue, pin_type pinType,
		uint8_t pin, uint8_t threadTime, uint8_t actionEvent) :
		ArduinoEEPROM(statusValue, WRITE, pinType, pin, threadTime, actionEvent) {
}

ArduinoEEPROMWrite::ArduinoEEPROMWrite(status statusValue, pin_type pinType,
		uint8_t pin, EEPROMData* data) :
		ArduinoEEPROM(statusValue, WRITE, pinType, pin, data) {

}

ArduinoEEPROMWrite::~ArduinoEEPROMWrite() {
	// TODO Auto-generated destructor stub
}

} /* namespace SISBARC */
