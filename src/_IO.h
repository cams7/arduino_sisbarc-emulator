/*
 * _IO.h
 *
 *  Created on: 10/02/2015
 *      Author: cams7
 */

#ifndef IO_H_
#define IO_H_

#include <inttypes.h>

namespace SISBARC {

class IO {
protected:
	static const char NEW_LINE;
	static const char STRING_END;
	static const char SPACE;

	static const uint8_t CODE_ASCII_SIZE;
	static const uint8_t CODE_ASCII[];

	static int8_t getIndexASCII(const uint8_t code);
	static int8_t getCodeASCII(const uint8_t index);

	static uint16_t getValueLine(const char* stringValue, uint8_t totalChars);
	static char* getStringLine(uint8_t value, uint8_t totalChars);

public:

};

} /* namespace SISBARC */

#endif /* IO_H_ */
