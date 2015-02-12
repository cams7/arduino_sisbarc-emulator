/*
 * ArduinoEEPROM.h
 *
 *  Created on: 31/01/2015
 *      Author: cams7
 */

#ifndef ARDUINOEEPROM_H_
#define ARDUINOEEPROM_H_

#include <inttypes.h>

#include "ArduinoStatus.h"
#include "EEPROMData.h"

namespace SISBARC {

class ArduinoEEPROM: public ArduinoStatus, public EEPROMData {
public:
	ArduinoEEPROM();
	ArduinoEEPROM(status statusValue, event eventValue, pin_type pinType,
			uint8_t pin, uint8_t threadTime, uint8_t actionEvent);
	ArduinoEEPROM(status statusValue, event eventValue, pin_type pinType,
			uint8_t pin, EEPROMData* data);

	virtual ~ArduinoEEPROM();

};

class ArduinoEEPROMRead: public ArduinoEEPROM {
public:
	ArduinoEEPROMRead();
	ArduinoEEPROMRead(status statusValue, pin_type pinType, uint8_t pin,
			uint8_t threadTime, uint8_t actionEvent);
	ArduinoEEPROMRead(status statusValue, pin_type pinType, uint8_t pin,
			EEPROMData* data);
	virtual ~ArduinoEEPROMRead();
};

class ArduinoEEPROMWrite: public ArduinoEEPROM {
public:
	ArduinoEEPROMWrite();
	ArduinoEEPROMWrite(status statusValue, pin_type pinType, uint8_t pin,
			uint8_t threadTime, uint8_t actionEvent);
	ArduinoEEPROMWrite(status statusValue, pin_type pinType, uint8_t pin,
			EEPROMData* data);
	virtual ~ArduinoEEPROMWrite();
};

} /* namespace SISBARC */

#endif /* ARDUINOEEPROM_H_ */
