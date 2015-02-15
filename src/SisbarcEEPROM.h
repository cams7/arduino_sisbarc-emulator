/*
 * SisbarcEEPROM.h
 *
 *  Created on: 31/01/2015
 *      Author: cams7
 */

#ifndef SISBARCEEPROM_H_
#define SISBARCEEPROM_H_

#include <inttypes.h>

#include "vo/ArduinoPin.h"
#include "vo/ArduinoEEPROM.h"
#include "vo/EEPROMData.h"

namespace SISBARC {

class SisbarcEEPROM {
private:
	//Caso obtenha um erro, algum dos metodos podem retornar esse valor
	static const int16_t RETURN_ERROR;
	//Endereco onde fica armazenado o TOTAL de bytes gravado na EEPROM
	static const uint8_t ADDRESS_TOTAL_BYTES;
	//PINO valido
	static bool isPinValid(pin_type pinType, uint8_t pin);

	//Busca um arrays de bytes do registro pelo endereco da EEPROM informado
	static uint8_t *getBytesRecord(uint16_t address);
	//Busca o registro pelo endereco da EEPROM informado
	static uint16_t getRecord(uint16_t address);

	//Busca o valor do PINO pelo endereco da EEPROM informado
	static uint16_t getPinValue(uint16_t address);

	//Busca o endereco da EEPROM pelo PINO informado
	static int16_t getAddress(pin_type pinType, uint8_t pin);

	//Busca o total de registros gravados
	static uint16_t getTotalBytesUsed();
	//Atualiza o total de registros gravados
	static void setTotalBytesUsed(uint16_t totalBytesUsed);

	//Ler o registro pelo endereco da EEPROM informado
	static EEPROMData *read(uint16_t address);

public:
	//Numero maximo de bytes gravado na EEPROM
	static const uint16_t TOTAL_BYTES_EEPROM;
	//Total de bytes por registro
	static const uint8_t TOTAL_BYTES_BY_RECORD;


	static EEPROMData *read(ArduinoPin* pin);
	//Ler o registro pelo PINO informado
	static EEPROMData *read(pin_type pinType, uint8_t pin);
	//Grava o registro
	static int16_t write(ArduinoEEPROMWrite* arduino);

	//Limpa o registro pelo PINO informado
	static bool clean(pin_type pinType, uint8_t pin);
	//Limpa todos os registro
	static void cleanAll();
};

} /* namespace SISBARC */

#endif /* SISBARCEEPROM_H_ */
