/*
 * Serial.cpp
 *
 *  Created on: 04/02/2015
 *      Author: cams7
 */

#include "_Serial.h"

#include "util/_arduino_time.h"
#include "util/Binary.h"

#include "ArduinoUSART.h"
#include "ArduinoEEPROM.h"
#include "SisbarcProtocol.h"
#include <stdio.h>

namespace SISBARC {

char const* const SerialClass::FILE_NAME =
		"/home/cams7/desenv/github/cpp/temp/serial_data.txt";

const uint8_t SerialClass::TOTAL_VALUES_LINE = 0x07;

const uint8_t SerialClass::TOTAL_CHARS = 0x04;

const uint16_t SerialClass::TIMEOUT_MS = 100; //1/10 segundos

SerialClass::SerialClass() :
		_status(new Stack<ArduinoStatus>()), _lastTime(0), _serialData(
		NULL), _serialDataIndex(0x00) {
	printf("New SerialClass\n");
}

SerialClass::~SerialClass() {
	delete _status;
	//free(_serialData);

	printf("Delete SerialClass\n");
}

void SerialClass::write(uint8_t* data, const uint8_t size) {
	uint32_t protocol = Binary::bytesToInt32(data);
	printf("serial write: %s\n", Binary::intToString4Bytes(protocol));
}

void SerialClass::readAllLines(void) {

	Stack<uint16_t>* linhas = new Stack<uint16_t>();
	uint16_t* linha = ((uint16_t*) malloc(TOTAL_VALUES_LINE));

	FILE* file = fopen(FILE_NAME, "r");
	char ch = getc(file);

	uint8_t countChars = 0x00;
	uint8_t countSpaces = 0x00;
	uint8_t countLines = 0x0000;

	char* stringValue = NULL;
	uint16_t value;

	bool hasError = false;

	while (ch != EOF) {
		if (ch == NEW_LINE && countChars > 0x00 && countSpaces > 0x00) {
			if (countSpaces < TOTAL_VALUES_LINE - 2) {
				printf(
						"ERROR: Os valores informado na linha '%u' sao menores que '%u'\n",
						countLines + 1, TOTAL_VALUES_LINE - 1);
				hasError = true;
				break;
			}

			if (countSpaces > TOTAL_VALUES_LINE - 1) {
				printf(
						"ERROR: Os valores informado na linha '%u' sao maiores que '%u'\n",
						countLines + 1, TOTAL_VALUES_LINE);
				hasError = true;
				break;
			}

			value = getValueLine(stringValue, countChars);

			if (!validateLine(value, linha, countSpaces, countLines)) {
				hasError = true;
				break;
			}

			linha[countSpaces] = value;

			linhas->push(linha);
			linha = ((uint16_t*) malloc(TOTAL_VALUES_LINE));

			countChars = 0x00;
			countSpaces = 0x00;
			countLines++;

			free(stringValue);
			stringValue = NULL;
		} else if (ch == SPACE && countChars > 0x00) {
			if (stringValue == NULL) {
				printf(
						"ERROR: o espaçamento entre os caracteres da linha '%u' não e valido\n",
						countLines + 1);
				hasError = true;
				break;
			}

			value = getValueLine(stringValue, countChars);

			if (!validateLine(value, linha, countSpaces, countLines)) {
				hasError = true;
				break;
			}

			linha[countSpaces] = value;

			countChars = 0x00;
			countSpaces++;

			free(stringValue);
			stringValue = NULL;
		} else {
			if (stringValue == NULL)
				stringValue = ((char*) malloc(TOTAL_CHARS));

			stringValue[countChars] = ch;

			countChars++;
		}
		ch = getc(file);
	}

	fclose(file);

	if (hasError || linhas->empty()) {
		delete linhas;
		//printf("free( linha )\n");
		free(linha);
		if (stringValue != NULL) {
			//printf("free( stringValue )\n");
			free(stringValue);
		}
		return;
	}

	//Limpa o arquivo
	fclose(fopen(FILE_NAME, "w"));

	countLines = 0x0000;
	while (!linhas->empty()) {
		linha = linhas->top();
		event eventValue = linha[2];

		ArduinoStatus* arduino = NULL;
		if (eventValue == ArduinoStatus::EXECUTE)
			arduino = new ArduinoUSART();
		else if (eventValue == ArduinoStatus::WRITE)
			arduino = new ArduinoEEPROMWrite();
		else if (eventValue == ArduinoStatus::READ)
			arduino = new ArduinoEEPROMRead();
		else if (eventValue == ArduinoStatus::MESSAGE)
			arduino = new ArduinoUSARTMessage();

		if (arduino == NULL) {
			printf("ERROR: Arduino is NULL na linha '%i'\n", countLines + 1);
			hasError = true;
			break;
		}

		arduino->setTransmitterValue(linha[0]);
		arduino->setStatusValue(linha[1]);
		arduino->setPinType(linha[3]);
		arduino->setPin(linha[4]);

		if (eventValue == ArduinoStatus::EXECUTE
				|| eventValue == ArduinoStatus::MESSAGE) {
			((ArduinoUSART*) arduino)->setPinValue(linha[5]);
		} else {
			((ArduinoEEPROM*) arduino)->setThreadTime(linha[5]);
			((ArduinoEEPROM*) arduino)->setActionEvent(linha[6]);
		}

		_status->push(arduino);

		for (uint8_t i = 0x00; i < TOTAL_VALUES_LINE; i++)
			printf("%i ", linha[i]);
		printf("\n");

		linhas->pop();

		countLines++;
	}

	delete linhas;
}

bool SerialClass::validateLine(uint16_t& value, uint16_t* linha, uint8_t& index,
		uint8_t& countLines) {
	//printf("value: %i, countSpaces: %i\n", value, countSpaces);
	if (index == 0x00 && value > ArduinoStatus::TRANSMITTER_MAX) {
		printf("ERROR: 'TRANSMITTER' do Arduino na linha '%u' não e valido\n",
				countLines + 1);
		return false;
	} else if (index == 0x01 && value > ArduinoStatus::STATUS_MAX) {
		printf("ERROR: 'STATUS' do Arduino na linha '%u' não e valido\n",
				countLines + 1);
		return false;
	} else if (index == 0x02 && value > ArduinoStatus::EVENT_MAX) {
		printf("ERROR: 'EVENT' do Arduino na linha '%u' não e valido\n",
				countLines + 1);
		return false;
	} else if (index == 0x03 && value > ArduinoStatus::PIN_TYPE_MAX) {
		printf("ERROR: 'PIN_TYPE' do Arduino na linha '%u' não e valido\n",
				countLines + 1);
		return false;
	} else if (index == 0x04) {
		uint16_t pinType = linha[3];
		//printf("pinType: %i\n", pinType);
		if (pinType == ArduinoStatus::DIGITAL
				&& value > ArduinoStatus::DIGITAL_PIN_MAX) {
			printf(
					"ERROR: O pino 'DIGITAL' do Arduino na linha '%u' não e valido\n",
					countLines + 1);
			return false;
		} else if (pinType == ArduinoStatus::ANALOG
				&& value > ArduinoStatus::ANALOG_PIN_MAX) {
			printf(
					"ERROR: O pino 'ANALOGICO' do Arduino na linha '%u' não e valido\n",
					countLines + 1);
			return false;
		}
	} else if (index == 0x05) {
		uint16_t event = linha[2];
		//printf("event: %i\n", event);

		if ((event == ArduinoStatus::EXECUTE || event == ArduinoStatus::MESSAGE)) {
			uint16_t pinType = linha[3];

			if (pinType == ArduinoStatus::DIGITAL
					&& value > ArduinoUSART::DIGITAL_PIN_VALUE_MAX) {
				printf(
						"ERROR: O valor do pino 'DIGITAL' do Arduino na linha '%u' não e valido\n",
						countLines + 1);
				return false;
			} else if (pinType == ArduinoStatus::ANALOG
					&& value > ArduinoUSART::ANALOG_PIN_VALUE_MAX) {
				printf(
						"ERROR: O valor do pino 'ANALOGICO' do Arduino na linha '%u' não e valido\n",
						countLines + 1);
				return false;
			}

		} else if ((event == ArduinoStatus::WRITE
				|| event == ArduinoStatus::READ)
				&& value > ArduinoEEPROM::THREAD_TIME_MAX) {
			printf(
					"ERROR: 'THREAD_TIME' do Arduino na linha '%u' não e valido\n",
					countLines + 1);
			return false;
		}
	} else if (index == 0x06) {
		uint16_t event = linha[2];

		if (event != ArduinoStatus::WRITE && event != ArduinoStatus::READ) {
			printf("ERROR: 'EVENT' do Arduino na linha '%u' não e valido\n",
					countLines + 1);
			return false;
		}

		uint16_t pinType = linha[3];

		if (pinType == ArduinoStatus::DIGITAL
				&& value > ArduinoEEPROM::DIGITAL_ACTION_EVENT_MAX) {
			printf(
					"ERROR: O 'ACTION_EVENT' do pino 'DIGITAL' do Arduino na linha '%u' não e valido\n",
					countLines + 1);
			return false;
		} else if (pinType == ArduinoStatus::ANALOG
				&& value > ArduinoEEPROM::ANALOG_ACTION_EVENT_MAX) {
			printf(
					"ERROR: O 'ACTION_EVENT' do pino 'ANALOGICO' do Arduino na linha '%u' não e valido\n",
					countLines + 1);
			return false;
		}
	}

	return true;
}

bool SerialClass::available(void) {
	long time = millis();
	if ((time - _lastTime) > TIMEOUT_MS) {
		_lastTime = time;

		readAllLines();

		return !_status->empty();
	}
	return false;
}

int16_t SerialClass::read(void) {
	//printf("read()\n");
	if (!_status->empty()) {
		if (_serialDataIndex == 0x00 && _serialData == NULL) {
			ArduinoStatus* arduino = _status->top();
			_serialData = SisbarcProtocol::getProtocol(arduino);
			_status->pop();
		}

		uint8_t data = _serialData[_serialDataIndex];
		_serialDataIndex++;

		if (_serialDataIndex == 0x04 && _serialData != NULL) {
			free(_serialData);

			_serialData = NULL;
			_serialDataIndex = 0x00;
		}

		return data;
	}

	return -1;
}

SerialClass Serial;

} /* namespace SISBARC */
