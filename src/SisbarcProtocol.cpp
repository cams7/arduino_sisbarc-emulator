/*
 * ArduinoProtocol.cpp
 *
 *  Created on: 14/01/2015
 *      Author: cams7
 */

#include "SisbarcProtocol.h"

#include <stdlib.h>

#include "util/Binary.h"
#include "util/Checksum.h"

#include "vo/ArduinoEEPROM.h"
#include "vo/ArduinoUSART.h"

namespace SISBARC {

const uint32_t SisbarcProtocol::EMPTY_BITS = 0x00000000;

const uint8_t SisbarcProtocol::TOTAL_BITS_PROTOCOL = 0x20; //Total de BITs do protocolo - 32 bits

const uint8_t SisbarcProtocol::TOTAL_BITS_INDEX = 0x04; //Total de BITs reservado para o INDICE - 4 bits
const uint8_t SisbarcProtocol::TOTAL_BITS_CHECKSUM = 0x08; //Total de BITs reservado para o CRC - 8 bits
const uint8_t SisbarcProtocol::TOTAL_BITS_DATA = TOTAL_BITS_PROTOCOL
		- TOTAL_BITS_INDEX - TOTAL_BITS_CHECKSUM; //Total de BITs reservado para os DADOs - 20 bits

const uint8_t SisbarcProtocol::TOTAL_BITS_DIGITAL_PIN = 0x06; //Total de BITs reservado para o PINO DIGITAL - 6 bits
const uint8_t SisbarcProtocol::TOTAL_BITS_ANALOG_PIN = 0x04; //Total de BITs reservado para o PINO ANALOGICO - 4 bits

const uint8_t SisbarcProtocol::TOTAL_BITS_DIGITAL_PIN_VALUE = 0x08; //Total de BITs reservado para o VALOR do PINO DIGITAL - 8 bits
const uint8_t SisbarcProtocol::TOTAL_BITS_ANALOG_PIN_VALUE = 0x0A; //Total de BITs reservado para o VALOR do PINO ANALOGICO - 10 bits

const uint8_t SisbarcProtocol::TOTAL_BITS_THREAD_TIME = 0x03; //Total de BITs reservado para o 'THREAD TIME' - 3 bits

const uint8_t SisbarcProtocol::TOTAL_BITS_DIGITAL_ACTION_EVENT = 0x05; //Total de BITs reservado para o 'ACTION EVENT' do PINO DIGITAL - 5 bits
const uint8_t SisbarcProtocol::TOTAL_BITS_ANALOG_ACTION_EVENT = 0x07; //Total de BITs reservado para o 'ACTION EVENT' do PINO ANALOGICO - 7 bits

const uint8_t SisbarcProtocol::TOTAL_BYTES_PROTOCOL = 0x04; //Total de BYTEs do protocolo - 4 bytes

//cria o protocolo
uint32_t SisbarcProtocol::encode(ArduinoStatus* arduino) {
	//EXECUTE/MESSAGE - DIGITAL
	//0000 0 00 00 0 000000 00000000 00000000
	//0000  _ Ordem bytes                                     4bits
	//0/1   _ ARDUINO/PC                                      1bit
	//0-3   _ SEND/SEND_RESPONSE/RESPONSE/RESPONSE_RESPONSE   2bit
	//0-3   _ EXECUTE/WRITE/READ/MESSAGE                      2bit
	//0/1   _ DIGITAL/ANALOG                                  1bit
	//0-63  _ PIN                                             6bits
	//0-255 _ VALOR PIN | CODE MESSAGE                        8bits
	//0-255 _ CRC                                             8bits

	//EXECUTE/MESSAGE - ANALOG
	//0000 0 00 11 1 0000 0000000000 00000000
	//0000   _ Ordem bytes                                    4bits
	//0/1    _ ARDUINO/PC                                     1bit
	//0-3    _ SEND/SEND_RESPONSE/RESPONSE/RESPONSE_RESPONSE  2bit
	//0-3    _ EXECUTE/WRITE/READ/MESSAGE                     2bit
	//0/1    _ DIGITAL/ANALOG                                 1bit
	//0-15   _ PIN                                            4bits
	//0-1023 _ VALOR PIN | CODE MESSAGE                      10bits
	//0-255  _ CRC                                            8bits

	//WRITE/READ - DIGITAL
	//0000 0 00 01 0 000000 000 00000 00000000
	//0000  _ Ordem bytes                                     4bits
	//0/1   _ ARDUINO/PC                                      1bit
	//0-3   _ SEND/SEND_RESPONSE/RESPONSE/RESPONSE_RESPONSE   2bit
	//0-3   _ EXECUTE/WRITE/READ/MESSAGE                      2bit
	//0/1   _ DIGITAL/ANALOG                                  1bit
	//0-63  _ PIN                                             6bits
	//0-7   _ THREAD TIME                                     3bits
	//0-31  _ ACTION EVENT                                    5bits
	//0-255 _ CRC                                             8bits

	//WRITE/READ - ANALOG
	//0000 0 00 10 1 0000 000 0000000 00000000
	//0000  _ Ordem bytes                                     4bits
	//0/1   _ ARDUINO/PC                                      1bit
	//0-3   _ SEND/SEND_RESPONSE/RESPONSE/RESPONSE_RESPONSE   2bit
	//0-3   _ EXECUTE/WRITE/READ/MESSAGE                      2bit
	//0/1   _ DIGITAL/ANALOG                                  1bit
	//0-15  _ PIN                                             4bits
	//0-7   _ THREAD TIME                                     3bits
	//0-127 _ ACTION EVENT                                    7bits
	//0-255 _ CRC                                             8bits

	if (arduino == NULL)
		return EMPTY_BITS;

	//Os status sao 0/1/2/3 = SEND/ SEND_RESPONSE/ RESPONSE/ RESPONSE_RESPONSE
	if (arduino->getStatusValue() > ArduinoStatus::STATUS_MAX)
		return EMPTY_BITS;

	//Os eventos sao 0/1/2/3 = EXECUTE/WRITE/READ/MESSAGE
	if (arduino->getEventValue() > ArduinoStatus::EVENT_MAX)
		return EMPTY_BITS;

	//O tipos de pino sao 0/1 = DIGITAL/ANALOG
	if (arduino->getPinType() > ArduinoStatus::PIN_TYPE_MAX)
		return EMPTY_BITS;

	//Os pinos estao entre 0-63
	if (arduino->getPinType() == ArduinoStatus::DIGITAL
			&& arduino->getPin() > ArduinoStatus::DIGITAL_PIN_MAX)
		return EMPTY_BITS;
	//Os pinos estao entre 0-15
	else if (arduino->getPinType() == ArduinoStatus::ANALOG
			&& arduino->getPin() > ArduinoStatus::ANALOG_PIN_MAX)
		return EMPTY_BITS;

	if (arduino->getEventValue() == ArduinoStatus::EXECUTE
			|| arduino->getEventValue() == ArduinoStatus::MESSAGE) {

		//Os valores do pino estao entre 0-255
		if (((ArduinoUSART*) arduino)->getPinType() == ArduinoUSART::DIGITAL
				&& ((ArduinoUSART*) arduino)->getPinValue()
						> ArduinoUSART::DIGITAL_PIN_VALUE_MAX)
			return EMPTY_BITS;
		//Os valores do pino estao entre 0-1023
		else if (((ArduinoUSART*) arduino)->getPinType() == ArduinoUSART::ANALOG
				&& ((ArduinoUSART*) arduino)->getPinValue()
						> ArduinoUSART::ANALOG_PIN_VALUE_MAX)
			return EMPTY_BITS;
	} else if (arduino->getEventValue() == ArduinoStatus::WRITE
			|| arduino->getEventValue() == ArduinoStatus::READ) {

		//Os valores da 'thread time' estao entre 0-7
		if (((ArduinoEEPROM*) arduino)->getThreadInterval()
				> ArduinoEEPROM::THREAD_INTERVAL_MAX)
			return EMPTY_BITS;

		//Os valores do 'action event' estao entre 0-31
		if (((ArduinoEEPROM*) arduino)->getPinType() == ArduinoEEPROM::DIGITAL
				&& ((ArduinoEEPROM*) arduino)->getActionEvent()
						> ArduinoEEPROM::DIGITAL_ACTION_EVENT_MAX)
			return EMPTY_BITS;
		//Os valores do 'action event' estao entre 0-127
		else if (((ArduinoEEPROM*) arduino)->getPinType()
				== ArduinoEEPROM::ANALOG
				&& ((ArduinoEEPROM*) arduino)->getActionEvent()
						> ArduinoEEPROM::ANALOG_ACTION_EVENT_MAX)
			return EMPTY_BITS;
	}

	uint32_t protocol = EMPTY_BITS;

	uint32_t mask = 0x00080000;
	uint32_t aux = arduino->getTransmitterValue();
	aux <<= (TOTAL_BITS_DATA - 1);
	protocol |= (aux & mask); //00000000 00001000 00000000 00000000

	mask = 0x00060000;
	aux = arduino->getStatusValue();
	aux <<= (TOTAL_BITS_DATA - 3);
	protocol |= (aux & mask); //00000000 00000110 00000000 00000000

	mask = 0x00018000;
	aux = arduino->getEventValue();
	aux <<= (TOTAL_BITS_DATA - 5);
	protocol |= (aux & mask); //00000000 00000001 10000000 00000000

	mask = 0x00004000;
	aux = arduino->getPinType();
	aux <<= (TOTAL_BITS_DATA - 6);
	protocol |= (aux & mask); //00000000 00000000 01000000 00000000

	//printf("protocol: %s\n", Binary::intToString4Bytes(protocol));

	if (arduino->getPinType() == ArduinoStatus::DIGITAL) {
		mask = 0x00003F00;
		aux = arduino->getPin();
		aux <<= TOTAL_BITS_DIGITAL_PIN_VALUE;
		protocol |= (aux & mask); //00000000 00000000 00111111 00000000

		if (arduino->getEventValue() == ArduinoStatus::EXECUTE
				|| arduino->getEventValue() == ArduinoStatus::MESSAGE) {
			mask = 0x000000FF;
			aux = ((ArduinoUSART*) arduino)->getPinValue();
			protocol |= (aux & mask); //00000000 00000000 00000000 11111111
		} else if (arduino->getEventValue() == ArduinoStatus::WRITE
				|| arduino->getEventValue() == ArduinoStatus::READ) {
			mask = 0x000000E0;
			aux = ((ArduinoEEPROM*) arduino)->getThreadInterval();
			aux <<= TOTAL_BITS_DIGITAL_ACTION_EVENT;
			protocol |= (aux & mask); //00000000 00000000 00000000 11100000

			mask = 0x0000001F;
			aux = ((ArduinoEEPROM*) arduino)->getActionEvent();
			protocol |= (aux & mask); //00000000 00000000 00000000 00011111
		}
	} else if (arduino->getPinType() == ArduinoStatus::ANALOG) {
		mask = 0x00003C00;
		aux = arduino->getPin();
		aux <<= TOTAL_BITS_ANALOG_PIN_VALUE;
		protocol |= (aux & mask); //00000000 00000000 00111100 00000000

		if (arduino->getEventValue() == ArduinoStatus::EXECUTE
				|| arduino->getEventValue() == ArduinoStatus::MESSAGE) {
			mask = 0x000003FF;
			aux = ((ArduinoUSART*) arduino)->getPinValue();
			protocol |= (aux & mask); //00000000 00000000 00000011 11111111
		} else if (arduino->getEventValue() == ArduinoStatus::WRITE
				|| arduino->getEventValue() == ArduinoStatus::READ) {
			mask = 0x00000380;
			aux = ((ArduinoEEPROM*) arduino)->getThreadInterval();
			aux <<= TOTAL_BITS_ANALOG_ACTION_EVENT;
			protocol |= (aux & mask); //00000000 00000000 00000011 10000000

			mask = 0x0000007F;
			aux = ((ArduinoEEPROM*) arduino)->getActionEvent();
			protocol |= (aux & mask); //00000000 00000000 00000000 01111111
		}
	}

	//printf("protocol: %s\n", Binary::intToString4Bytes(protocol));

	crc checksum = Checksum::getCrc3Bytes(protocol);

	protocol <<= TOTAL_BITS_CHECKSUM;

	aux = checksum;
	protocol |= (aux & 0x000000FF); //00000000 00000000 00000000 11111111

	uint8_t *pointer;
	pointer = ((uint8_t*) malloc(TOTAL_BYTES_PROTOCOL));

	if (pointer == NULL)
		return EMPTY_BITS;

	for (uint8_t i = 0x00; i < TOTAL_BYTES_PROTOCOL; i++) { //00001111 11111111 11111111 11111111 -> 11111111 01111111 01111111 01111111
		aux = protocol;
		aux <<= (TOTAL_BITS_INDEX + (i * 7));
		aux >>= (TOTAL_BITS_PROTOCOL - 7);
		if (i == 0x00) {
			aux |= 0x00000080;
			aux = aux & 0x000000FF; //00000000 00000000 00000000 11111111
		} else
			aux = aux & 0x0000007F; //00000000 00000000 00000000 01111111

		*(pointer + i) = (uint8_t) aux;
	}

	protocol = Binary::bytesToInt32(pointer);

	free(pointer);

	return protocol;
}

uint8_t *SisbarcProtocol::getProtocol(ArduinoStatus* arduino) {
	if (arduino == NULL)
		return NULL;

	uint32_t protocol = encode(arduino);
	//free(arduino);

	if (protocol == EMPTY_BITS)
		return NULL;

	return Binary::intTo4Bytes(protocol);
}

uint8_t *SisbarcProtocol::getProtocolUSART(status statusValue, event eventValue,
		pin_type pinType, uint8_t pin, uint16_t pinValue) {
	ArduinoStatus* arduino = NULL;

	if (eventValue == ArduinoStatus::EXECUTE)
		arduino = new ArduinoUSART(statusValue, pinType, pin, pinValue);
	else if (eventValue == ArduinoStatus::MESSAGE)
		arduino = new ArduinoUSARTMessage(statusValue, pinType, pin, pinValue);

	if (arduino == NULL)
		return NULL;

	uint8_t *data;
	data = getProtocol(arduino);
	delete arduino;

	return data;
}

uint8_t *SisbarcProtocol::getProtocolEEPROM(status statusValue,
		event eventValue, pin_type pinType, uint8_t pin, uint8_t threadTime,
		uint8_t actionEvent) {
	ArduinoStatus* arduino = NULL;

	if (eventValue == ArduinoStatus::WRITE)
		arduino = new ArduinoEEPROMWrite(statusValue, pinType, pin, threadTime,
				actionEvent);
	else if (eventValue == ArduinoStatus::READ)
		arduino = new ArduinoEEPROMRead(statusValue, pinType, pin, threadTime,
				actionEvent);

	if (arduino == NULL)
		return NULL;

	uint8_t *data;
	data = getProtocol(arduino);
	delete arduino;

	return data;
}

//Decodifica o protocolo
ArduinoStatus *SisbarcProtocol::decode(uint8_t const values[]) {
	uint32_t aux;
	uint32_t protocol = EMPTY_BITS;
	uint32_t mask = 0x0000007F;

	for (uint8_t i = 0; i < TOTAL_BYTES_PROTOCOL; i++) {
		aux = values[i];
		aux <<= (7 * (TOTAL_BYTES_PROTOCOL - i - 1));
		aux &= (mask << (7 * (TOTAL_BYTES_PROTOCOL - i - 1)));
		protocol |= aux;
	}

	mask = 0x000000FF;
	crc checksumProtocol = protocol & mask; // 00000000 00000000 00000000 11111111

	mask = 0x0FFFFF00;
	uint32_t message = (protocol & mask) >> TOTAL_BITS_CHECKSUM; // 00001111 11111111 11111111 00000000
	crc checksum = Checksum::getCrc3Bytes(message);

	//Verifica se os dados do protocolo nao foram corrompidos
	if (checksumProtocol != checksum)
		return NULL;

	//0/1 _ ARDUINO/PC      1bit
	mask = 0x08000000;
	uint32_t transmitterValue = protocol & mask;
	transmitterValue >>= (TOTAL_BITS_PROTOCOL - TOTAL_BITS_INDEX - 1); // 00001000 00000000 00000000 00000000

	//0-3 _ SEND/SEND_RESPONSE/RESPONSE/RESPONSE_RESPONSE   2bit
	mask = 0x06000000;
	uint32_t statusValue = protocol & mask;
	statusValue >>= (TOTAL_BITS_PROTOCOL - TOTAL_BITS_INDEX - 3); // 00000110 00000000 00000000 00000000

	//0-3 _ EXECUTE/WRITE/READ/MESSAGE   2bit
	mask = 0x01800000;
	uint32_t eventValue = protocol & mask;
	eventValue >>= (TOTAL_BITS_PROTOCOL - TOTAL_BITS_INDEX - 5); // 00000001 10000000 00000000 00000000

	//0/1 _ DIGITAL/ANALOG  1bit
	mask = 0x00400000;
	uint32_t pinType = protocol & mask;
	pinType >>= (TOTAL_BITS_PROTOCOL - TOTAL_BITS_INDEX - 6); // 00000000 01000000 00000000 00000000

	ArduinoStatus* arduino = NULL;

	if (eventValue == ArduinoStatus::EXECUTE)
		arduino = new ArduinoUSART();
	else if (eventValue == ArduinoStatus::WRITE)
		arduino = new ArduinoEEPROMWrite();
	else if (eventValue == ArduinoStatus::READ)
		arduino = new ArduinoEEPROMRead();
	else if (eventValue == ArduinoStatus::MESSAGE)
		arduino = new ArduinoUSARTMessage();

	if (arduino == NULL)
		return NULL;

	arduino->setTransmitterValue((uint8_t) transmitterValue);
	arduino->setStatusValue((uint8_t) statusValue);
	//arduino->setEventValue((uint8_t) eventValue);
	arduino->setPinType((uint8_t) pinType);

	if (arduino->getPinType() == ArduinoStatus::DIGITAL) {
		//0-63  _ PIN           6bits
		mask = 0x003F0000;
		uint32_t pin = protocol & mask;
		pin >>= (TOTAL_BITS_PROTOCOL - TOTAL_BITS_INDEX - 6
				- TOTAL_BITS_DIGITAL_PIN); // 00000000 00111111 00000000 00000000
		arduino->setPin((uint8_t) pin);

		if (arduino->getEventValue() == ArduinoStatus::EXECUTE
				|| arduino->getEventValue() == ArduinoStatus::MESSAGE) {
			//0-255 _ VALOR PIN    8bits
			mask = 0x0000FF00;
			uint32_t pinValue = protocol & mask;
			pinValue >>= TOTAL_BITS_CHECKSUM; // 00000000 00000000 11111111 00000000
			((ArduinoUSART*) arduino)->setPinValue((uint16_t) pinValue);
		} else if (arduino->getEventValue() == ArduinoStatus::WRITE
				|| arduino->getEventValue() == ArduinoStatus::READ) {
			//0-7 _ VALOR PIN    3bits
			mask = 0x0000E000;
			uint32_t threadTime = protocol & mask;
			threadTime >>= (TOTAL_BITS_CHECKSUM
					+ TOTAL_BITS_DIGITAL_ACTION_EVENT); // 00000000 00000000 11100000 00000000
			((ArduinoEEPROM*) arduino)->setThreadInterval((uint8_t) threadTime);

			//0-31 _ VALOR PIN    5bits
			mask = 0x00001F00;
			uint32_t actionEvent = protocol & mask;
			actionEvent >>= TOTAL_BITS_CHECKSUM; // 00000000 00000000 00011111 00000000
			((ArduinoEEPROM*) arduino)->setActionEvent((uint8_t) actionEvent);
		}
	} else if (arduino->getPinType() == ArduinoStatus::ANALOG) {
		//0-15  _ PIN           6bits
		mask = 0x003C0000;
		uint32_t pin = protocol & mask;
		pin >>= (TOTAL_BITS_PROTOCOL - TOTAL_BITS_INDEX - 4
				- TOTAL_BITS_DIGITAL_PIN); // 00000000 00111100 00000000 00000000
		arduino->setPin((uint8_t) pin);

		if (arduino->getEventValue() == ArduinoStatus::EXECUTE
				|| arduino->getEventValue() == ArduinoStatus::MESSAGE) {
			//0-1023 _ VALOR PIN    10bits
			mask = 0x0003FF00;
			uint32_t pinValue = protocol & mask;
			pinValue >>= TOTAL_BITS_CHECKSUM; // 00000000 00000011 11111111 00000000
			((ArduinoUSART*) arduino)->setPinValue((uint16_t) pinValue);
		} else if (arduino->getEventValue() == ArduinoStatus::WRITE
				|| arduino->getEventValue() == ArduinoStatus::READ) {
			//0-7 _ VALOR PIN    3bits
			mask = 0x00038000;
			uint32_t threadTime = protocol & mask;
			threadTime >>=
					(TOTAL_BITS_CHECKSUM + TOTAL_BITS_ANALOG_ACTION_EVENT); // 00000000 00000011 10000000 00000000
			((ArduinoEEPROM*) arduino)->setThreadInterval((uint8_t) threadTime);

			//0-31 _ VALOR PIN    5bits
			mask = 0x00007F00;
			uint32_t actionEvent = protocol & mask;
			actionEvent >>= TOTAL_BITS_CHECKSUM; // 00000000 00000000 01111111 00000000
			((ArduinoEEPROM*) arduino)->setActionEvent((uint8_t) actionEvent);
		}
	}

	return arduino;
}

} /* namespace SISBARC */
