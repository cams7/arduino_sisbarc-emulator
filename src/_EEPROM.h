/*
 * EEPROM.h
 *
 *  Created on: 30/01/2015
 *      Author: cams7
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <inttypes.h>

#include "_IO.h"

namespace SISBARC {

class EEPROM: public IO {
private:
	static char const* const FILE_NAME;
	static char const* const FILE_TEMP;

	static const uint8_t TOTAL_CHARS;

public:
	static uint8_t read(uint16_t);
	static void write(uint16_t, uint8_t);
};

} /* namespace SISBARC */

#endif /* EEPROM_H_ */
