/*
 * EEPROMData1.h
 *
 *  Created on: 06/02/2015
 *      Author: cams7
 */

#ifndef EEPROMDATA_H_
#define EEPROMDATA_H_

#include <inttypes.h>

namespace SISBARC {

class EEPROMData {
private:
	uint8_t threadTime;
	uint8_t actionEvent;

public:
	static const uint8_t THREAD_TIME_MAX;

	static const uint8_t DIGITAL_ACTION_EVENT_MAX;
	static const uint8_t ANALOG_ACTION_EVENT_MAX;

	EEPROMData();
	virtual ~EEPROMData();

	EEPROMData(uint8_t threadTime, uint8_t actionEvent);

	uint8_t getThreadTime(void);
	void setThreadTime(uint8_t threadTime);

	uint8_t getActionEvent(void);
	void setActionEvent(uint8_t actionEvent);
};

} /* namespace SISBARC */

#endif /* EEPROMDATA_H_ */
