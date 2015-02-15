/*
 * SisbarcEEPROM.cpp
 *
 *  Created on: 31/01/2015
 *      Author: cams7
 */

#include "SisbarcEEPROM.h"

#include <stdlib.h>
#include "util/Binary.h"

#include "_EEPROM.h"

namespace SISBARC {

const uint16_t SisbarcEEPROM::TOTAL_BYTES_EEPROM = 0x0400; //1024
const uint8_t SisbarcEEPROM::TOTAL_BYTES_BY_RECORD = 0x02; //2

const int16_t SisbarcEEPROM::RETURN_ERROR = -1; //0xFFFFFFFF
const uint8_t SisbarcEEPROM::ADDRESS_TOTAL_BYTES = 0x00;

bool SisbarcEEPROM::isPinValid(pin_type pinType, uint8_t pin) {
	//O tipos de pino sao 0/1 = DIGITAL/ANALOG
	if (pinType > ArduinoEEPROM::PIN_TYPE_MAX)
		return false;

	//Os pinos estao entre 0-63
	if (pinType == ArduinoEEPROM::DIGITAL
			&& pin > ArduinoEEPROM::DIGITAL_PIN_MAX)
		return false;
	//Os pinos estao entre 0-15
	else if (pinType == ArduinoEEPROM::ANALOG
			&& pin > ArduinoEEPROM::ANALOG_PIN_MAX)
		return false;

	return true;
}

uint8_t *SisbarcEEPROM::getBytesRecord(uint16_t address) {
	uint8_t *bytes;
	bytes = ((uint8_t*) malloc(TOTAL_BYTES_BY_RECORD));

	*(bytes) = EEPROM::read(address);
	*(bytes + 1) = EEPROM::read(address + 1);

	return bytes;
}

uint16_t SisbarcEEPROM::getRecord(uint16_t address) {
	uint8_t *bytes;
	bytes = getBytesRecord(address);

	uint16_t record = Binary::bytesToInt16(bytes);
	free(bytes);

	return record;
}

uint16_t SisbarcEEPROM::getPinValue(uint16_t address) {

	//if (address >= TOTAL_BYTES_EEPROM)
	//	return RETURN_ERROR;

	uint16_t record = getRecord(address);

	uint16_t mask = 0x0001;

	uint16_t pinType = record;
	pinType <<= 1;
	pinType >>= 15;
	pinType &= mask; //00000000 00000001

	if (pinType == ArduinoEEPROM::DIGITAL) {
		mask = 0x00FF;
		record &= mask; //00000000 11111111
	} else if (pinType == ArduinoEEPROM::ANALOG) {
		mask = 0x03FF;
		record &= mask; //00000011 11111111
	}

	return record;
}

int16_t SisbarcEEPROM::getAddress(pin_type pinType, uint8_t pin) {

	if (!isPinValid(pinType, pin))
		return RETURN_ERROR;

	uint16_t totalBytes = getTotalBytesUsed();

	for (uint16_t address = ADDRESS_TOTAL_BYTES + TOTAL_BYTES_BY_RECORD;
			address < totalBytes; address += TOTAL_BYTES_BY_RECORD) {

		uint16_t record = getRecord(address);

		uint16_t mask = 0x0001;
		uint16_t recordPinType = record;
		recordPinType <<= 1;
		recordPinType >>= 15;
		recordPinType &= mask; //00000000 00000001

		if (recordPinType != pinType)
			continue;

		uint16_t recordPin = record;
		recordPin <<= 2;

		if (pinType == ArduinoEEPROM::DIGITAL) {
			//00000000 00111111
			mask = 0x003F;
			recordPin >>= 10;
			recordPin &= mask;

		} else if (pinType == ArduinoEEPROM::ANALOG) {
			//00000000 00001111
			mask = 0x000F;
			recordPin >>= 12;
			recordPin &= mask;
		}

		if (recordPin == pin)
			return address;

	}

	return RETURN_ERROR;
}

uint16_t SisbarcEEPROM::getTotalBytesUsed() {
	uint16_t record = getRecord(ADDRESS_TOTAL_BYTES);
	return record;
}

void SisbarcEEPROM::setTotalBytesUsed(uint16_t totalBytesUsed) {
	uint8_t *bytes;
	bytes = Binary::intTo2Bytes(totalBytesUsed);

	EEPROM::write(ADDRESS_TOTAL_BYTES, *(bytes));
	EEPROM::write(ADDRESS_TOTAL_BYTES + 1, *(bytes + 1));

	free(bytes);
}

EEPROMData *SisbarcEEPROM::read(uint16_t address) {
	//O valor do registro não pode ser 0
	uint16_t record = getRecord(address);
	//if (record == 0x0000)
	//	return NULL;

	uint16_t mask = 0x0001;
	uint16_t pinType = record;
	pinType <<= 1;
	pinType >>= 15;
	pinType &= mask; //00000000 00000001

	//printf("pinType: %u\n", pinType);

	uint16_t pin = record;
	pin <<= 2;

	uint16_t pinValue = getPinValue(address);
	uint16_t threadTime = pinValue;
	uint16_t actionEvent = pinValue;

	if (pinType == ArduinoEEPROM::DIGITAL) {
		mask = 0x003F;
		pin >>= 10;
		pin &= mask; //00000000 00111111

		mask = 0x0007;
		threadTime >>= 5;
		threadTime &= mask; //00000000 00000111

		mask = 0x001F;
		actionEvent &= mask; //00000000 00011111
	} else if (pinType == ArduinoEEPROM::ANALOG) {
		mask = 0x000F;
		pin >>= 12;
		pin &= mask; //00000000 00001111

		mask = 0x0007;
		threadTime >>= 7;
		threadTime &= mask; //00000000 00000111

		mask = 0x007F;
		actionEvent &= mask; //00000000 01111111
	}

	//printf("pin: %u\n", pin);

	EEPROMData* data = new EEPROMData((uint8_t) threadTime,
			(uint8_t) actionEvent);

	return data;
}

EEPROMData *SisbarcEEPROM::read(pin_type pinType, uint8_t pin) {
	int16_t address = getAddress(pinType, pin);

	if (address == RETURN_ERROR)
		return NULL;

	return read(address);

}

EEPROMData *SisbarcEEPROM::read(ArduinoPin* pin) {
	if (pin == NULL)
		return NULL;

	return read(pin->getPinType(), pin->getPin());
}

int16_t SisbarcEEPROM::write(ArduinoEEPROMWrite* arduino) {
	if (!isPinValid(arduino->getPinType(), arduino->getPin()))
		return RETURN_ERROR;

	int16_t address = getAddress(arduino->getPinType(), arduino->getPin());
	bool isNewRecord = false;

	uint16_t record = 0x8000; //10000000 00000000

	if (address == RETURN_ERROR) {
		address = getTotalBytesUsed();

		if (((uint16_t) address) == TOTAL_BYTES_EEPROM)
			return RETURN_ERROR;

		isNewRecord = true;

		record = 0x0000; //00000000 00000000
	}

	//0/1 pin_type pin threadTime actionEvent
	//DIGITAL
	//1 0 111111 111 11111 / update digital 63 7 31
	//ANALOG
	//1 1 1111 111 1111111 / update analog 15 7 127

	//01000000 00000000
	uint16_t mask = 0x4000;
	uint16_t aux = arduino->getPinType();
	aux <<= 14;
	record |= (aux & mask);

	//printf("1) record: %s\n", Binary::intToString2Bytes(record));

	if (arduino->getPinType() == ArduinoEEPROM::DIGITAL) {
		//00111111 00000000
		mask = 0x3F00;
		aux = arduino->getPin();
		aux <<= 8;
		record |= (aux & mask);

		//00000000 11100000
		mask = 0x00E0;
		aux = arduino->getThreadInterval();
		aux <<= 5;
		record |= (aux & mask);

		//00000000 00011111
		mask = 0x001F;
		aux = arduino->getActionEvent();
		record |= (aux & mask);

	} else if (arduino->getPinType() == ArduinoEEPROM::ANALOG) {
		//00111100 00000000
		mask = 0x3C00;
		aux = arduino->getPin();
		aux <<= 10;
		record |= (aux & mask);

		//00000011 10000000
		mask = 0x0380;
		aux = arduino->getThreadInterval();
		aux <<= 7;
		record |= (aux & mask);

		//00000000 01111111
		mask = 0x007F;
		aux = arduino->getActionEvent();
		record |= (aux & mask);
	}

	uint8_t *bytes;
	bytes = Binary::intTo2Bytes(record);

	EEPROM::write(address, *(bytes));
	EEPROM::write(address + 1, *(bytes + 1));

	free(bytes);

	int16_t returnValue = 0x0001; //00000000 00000001

	if (isNewRecord) {
		setTotalBytesUsed(address + TOTAL_BYTES_BY_RECORD);
		returnValue = 0x0000; //00000000 00000000
	}

	return returnValue;
}

bool SisbarcEEPROM::clean(pin_type pinType, uint8_t pin) {
	if (!isPinValid(pinType, pin))
		return false;

	int16_t addressRemoved = getAddress(pinType, pin);

	if (addressRemoved == RETURN_ERROR)
		return false;

	//printf("addressRemoved: %u\n", addressRemoved);

	uint16_t totalBytes = getTotalBytesUsed();

	//printf("totalBytes: %u\n", totalBytes);

	for (uint16_t address = addressRemoved + TOTAL_BYTES_BY_RECORD;
			address < totalBytes; address += TOTAL_BYTES_BY_RECORD) {

		uint8_t *bytes;
		bytes = getBytesRecord(address);

		EEPROM::write(addressRemoved, *(bytes));
		EEPROM::write(addressRemoved + 1, *(bytes + 1));

		//printf("Removed %u, address %u -> (%u %u)\n", addressRemoved, address, *(bytes), *(bytes + 1));

		free(bytes);

		addressRemoved += TOTAL_BYTES_BY_RECORD;
	}

	EEPROM::write(addressRemoved, 0x00);
	EEPROM::write(addressRemoved + 1, 0x00);

	setTotalBytesUsed(totalBytes - TOTAL_BYTES_BY_RECORD);

	return true;
}
void SisbarcEEPROM::cleanAll() {
	uint16_t totalBytes = getTotalBytesUsed();
	//uint16_t totalBytes = TOTAL_BYTES_EEPROM;

	for (uint16_t address = 0x0000; address < totalBytes; address++)
		EEPROM::write(address, 0x00);

	setTotalBytesUsed(TOTAL_BYTES_BY_RECORD);
}

} /* namespace SISBARC */
