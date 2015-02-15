/*
 * EEPROMData1.cpp
 *
 *  Created on: 06/02/2015
 *      Author: cams7
 */

#include "EEPROMData.h"

namespace SISBARC {

const uint8_t EEPROMData::THREAD_INTERVAL_MAX = 0x07; //7
const uint8_t EEPROMData::DIGITAL_ACTION_EVENT_MAX = 0x1F; //31
const uint8_t EEPROMData::ANALOG_ACTION_EVENT_MAX = 0x7F; //127

EEPROMData::EEPROMData() :
		_threadInterval(0x00), _actionEvent(0x00) {
}

EEPROMData::EEPROMData(uint8_t threadInterval, uint8_t actionEvent) :
		_threadInterval(threadInterval), _actionEvent(actionEvent) {
}

EEPROMData::~EEPROMData() {
}

uint8_t EEPROMData::getThreadInterval(void) {
	return _threadInterval;
}
void EEPROMData::setThreadInterval(uint8_t threadInterval) {
	this->_threadInterval = threadInterval;
}

uint8_t EEPROMData::getActionEvent(void) {
	return _actionEvent;
}
void EEPROMData::setActionEvent(uint8_t actionEvent) {
	this->_actionEvent = actionEvent;
}

} /* namespace SISBARC */
