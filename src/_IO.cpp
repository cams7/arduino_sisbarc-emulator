/*
 * _IO.cpp
 *
 *  Created on: 10/02/2015
 *      Author: cams7
 */

#include "_IO.h"

#include <stdlib.h>
#include <math.h>

namespace SISBARC {

const char IO::NEW_LINE = 0x0A; //Nova linha '\n' - 10
const char IO::STRING_END = 0x00; //Final de String '\0' - 0
const char IO::SPACE = 0x20; //Espaço em branco - 32

const uint8_t IO::CODE_ASCII_SIZE = 10;
const uint8_t IO::CODE_ASCII[CODE_ASCII_SIZE] = { 48, 49, 50, 51, 52, 53, 54,
		55, 56, 57 };

int8_t IO::getIndexASCII(const uint8_t code) {
	for (uint8_t i = 0x00; i < CODE_ASCII_SIZE; i++)
		if (CODE_ASCII[i] == code)
			return i;
	return -1;
}

int8_t IO::getCodeASCII(const uint8_t index) {
	return CODE_ASCII[index];
}

uint16_t IO::getValueLine(const char* stringValue, uint8_t totalChars) {
	uint16_t value = 0x0000;

	for (uint8_t i = 0x00; i < totalChars; i++)
		value += getIndexASCII(stringValue[i]) * pow(10, totalChars - i - 1);

	return value;
}

char* IO::getStringLine(uint8_t value, uint8_t totalChars) {

	if (value < 0x000A) //10
		totalChars -= 2;
	else if (value < 0x0064) //100
		totalChars -= 1;

	char *string;
	string = (char*) malloc(totalChars + 1);
	if (string == NULL)
		return NULL;

	uint8_t index = totalChars - 1;

	if (value >= 0) {
		do {
			*(string + index) = getCodeASCII(value % 10);
			value /= 10;
			index--;
		} while (value != 0);
	}
	*(string + totalChars) = STRING_END;

	return string;

}

} /* namespace SISBARC */
