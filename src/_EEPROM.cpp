/*
 * EEPROM.cpp
 *
 *  Created on: 30/01/2015
 *      Author: cams7
 */

#include "_EEPROM.h"

#include <stdlib.h>
#include <stdio.h>

namespace SISBARC {

char const* const EEPROM::FILE_NAME = "/home/cams7/desenv/github/cpp/temp/eeprom.txt";
char const* const EEPROM::FILE_TEMP =
		"/home/cams7/desenv/github/cpp/temp/eeprom_temp.txt";

const uint8_t EEPROM::TOTAL_CHARS = 0x03;

uint8_t EEPROM::read(uint16_t address) {
	FILE* file = fopen(FILE_NAME, "r");

	uint16_t countLines = 0x0000;
	uint8_t countChars = 0x00;

	char line[TOTAL_CHARS];

	char ch = getc(file);
	while (ch != EOF) {
		if (ch != NEW_LINE) {
			line[countChars] = ch;
			countChars++;
		} else if (countLines != address) {
			countChars = 0x00;
			countLines++;
		} else
			break;

		ch = getc(file);
	}

	fclose(file);

	if (countLines == address)
		return (uint8_t) getValueLine(line, countChars);

	return 0x00;
}

void EEPROM::write(uint16_t address, uint8_t value) {
	FILE* file = fopen(FILE_NAME, "r");
	FILE* fileTemp = fopen(FILE_TEMP, "w");

	//rewind(file);

	uint16_t countLines = 0x0000;

	char ch = getc(file);
	while (ch != EOF) {
		if (address == countLines) {
			char* string;
			string = getStringLine(value, TOTAL_CHARS);

			uint8_t countChars = 0x00;
			while (string[countChars] != STRING_END) {
				putc(string[countChars], fileTemp);
				countChars++;
			}
			free(string);

			putc(NEW_LINE, fileTemp);

			while (ch != NEW_LINE)
				ch = getc(file);

			countLines++;

		} else {
			//copy all lines in file replica.txt
			putc(ch, fileTemp);

			if (ch == NEW_LINE)
				countLines++;

		}
		ch = getc(file);
	}

	fclose(fileTemp);
	fclose(file);

	remove(FILE_NAME);
	rename(FILE_TEMP, FILE_NAME);
}

} /* namespace SISBARC */
