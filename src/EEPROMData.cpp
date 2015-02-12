/*
 * EEPROMData1.cpp
 *
 *  Created on: 06/02/2015
 *      Author: cams7
 */

#include "EEPROMData.h"

namespace SISBARC {

const uint8_t EEPROMData::THREAD_TIME_MAX = 0x07; //7
const uint8_t EEPROMData::DIGITAL_ACTION_EVENT_MAX = 0x1F; //31
const uint8_t EEPROMData::ANALOG_ACTION_EVENT_MAX = 0x7F; //127

EEPROMData::EEPROMData() :
		threadTime(0x00), actionEvent(0x00) {
}

EEPROMData::EEPROMData(uint8_t threadTime, uint8_t actionEvent) :
		threadTime(threadTime), actionEvent(actionEvent) {
}

EEPROMData::~EEPROMData() {
}

uint8_t EEPROMData::getThreadTime(void) {
	return threadTime;
}
void EEPROMData::setThreadTime(uint8_t threadTime) {
	this->threadTime = threadTime;
}

uint8_t EEPROMData::getActionEvent(void) {
	return actionEvent;
}
void EEPROMData::setActionEvent(uint8_t actionEvent) {
	this->actionEvent = actionEvent;
}

} /* namespace SISBARC */
