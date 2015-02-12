/*
 * Binary.cpp
 *
 *  Created on: 13/01/2015
 *      Author: cams7
 */

#include "Binary.h"
//#include <Arduino.h>
#include <stdlib.h>

namespace SISBARC {

const uint8_t Binary::MAX_1BYTE = 0xFF;        //255
const uint16_t Binary::MAX_2BYTE = 0xFFFF;     //65535
const uint32_t Binary::MAX_3BYTE = 0xFFFFFF;   //6777215
const uint32_t Binary::MAX_4BYTE = 0xFFFFFFFF; //4294967295 - Maximo valor de um 'unsigned int'

//Converte um numero inteiro em array de bytes
uint8_t *Binary::intToBytes(uint32_t value, uint8_t totalBytes) {

	uint8_t *pointer;

	pointer = ((uint8_t*) malloc(totalBytes));
	if (pointer == NULL)
		return NULL;

	const uint8_t BITS = 8;
	const uint8_t BITS_DESLOCADOS = (totalBytes - 1) * BITS;

	uint32_t aux;
	for (uint8_t i = 0; i < totalBytes; i++) {
		aux = (value << (i * BITS));
		aux >>= BITS_DESLOCADOS;
		*(pointer + i) = aux;
	}

	return pointer;
}

//Converte um 'unsigned int 32' em array de bytes
uint8_t *Binary::intTo4Bytes(uint32_t value) {
	uint8_t *pointer;

	const uint8_t TOTAL_BYTES = 4;

	pointer = intToBytes(value, TOTAL_BYTES);
	return pointer;
}

//Converte um 'unsigned int 32' em array de bytes
uint8_t *Binary::intTo3Bytes(uint32_t value) {
	//Valor maximo 3 bytes
	if (value > MAX_3BYTE)
		return NULL;

	uint8_t *pointer;

	const uint8_t TOTAL_BYTES = 3;

	pointer = intToBytes(value, TOTAL_BYTES);
	return pointer;
}

//Converte um 'unsigned int 16' em array de bytes
uint8_t *Binary::intTo2Bytes(uint16_t value) {
	uint8_t *pointer;

	const uint8_t TOTAL_BYTES = 2;

	pointer = intToBytes(value, TOTAL_BYTES);
	return pointer;
}

//Converte um array de bytes em numero inteiro
uint32_t Binary::bytesToInt(uint8_t const values[], uint8_t totalBytes) {
	const uint8_t TOTAL_BITS = 8;

	uint32_t value = 0x00000000;

	uint32_t aux;
	for (uint8_t i = 0; i < totalBytes; i++)
		for (int8_t j = TOTAL_BITS - 1; j >= 0; j--) {
			aux = getValueBit(values[i], j);
			aux <<= (((totalBytes - i - 1) * TOTAL_BITS) + j);
			value |= aux;
		}

	return value;
}

//Converte um array de bytes em um 'unsigned int 32'
uint32_t Binary::bytesToInt32(const uint8_t values[]) {
	return bytesToInt(values, 4);
}

uint16_t Binary::bytesToInt16(const uint8_t values[]) {
	return (uint16_t) bytesToInt(values, 2);
}

//Converte um numero inteiro em uma string de bytes
unsigned char *Binary::intToStringBytes(uint8_t totalBits, uint32_t value) {

	unsigned char *pointer;

	pointer = ((unsigned char*) malloc(totalBits + 1));

	int count = 0;

	if (pointer == NULL)
		return NULL;

	uint32_t aux;

	for (int8_t i = (totalBits - 1); i >= 0; i--) {
		aux = (value >> i);
		if (aux & 0x00000001)
			*(pointer + count) = 1 + '0';
		else
			*(pointer + count) = 0 + '0';

		count++;
	}

	*(pointer + count) = '\0';

	return pointer;
}

//Converte um 'unsigned int 32' em uma string de bytes
unsigned char *Binary::intToString4Bytes(uint32_t value) {
	return intToStringBytes(32, value);
}

//Converte um 'unsigned int 32' em uma string de bytes
unsigned char *Binary::intToString3Bytes(uint32_t value) {
	//Valor maximo 3 bytes
	if (value > MAX_3BYTE)
		return NULL;
	return intToStringBytes(24, value);
}

//Converte um 'unsigned int 16' em uma string de bytes
unsigned char *Binary::intToString2Bytes(uint16_t value) {
	return intToStringBytes(16, value);
}

//Converte um 'unsigned int 8' em uma string de bytes
unsigned char *Binary::intToString1Byte(uint8_t value) {
	return intToStringBytes(8, value);
}

//Retorna o bit pelo indice
uint8_t Binary::getValueBit(uint32_t value, uint8_t index) {
	//const uint8_t TOTAL_BITS = 0x20;	        //32 bits

	//Value = 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 0 0 0 0 0 0 0 0 0
	//Index = 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0

	//Comeca a contagem do 'bit menos significativo' para o 'bit mais significativo', ou seja a contagem e feita da direita para esquerda.

	//uint32_t bitValue = (value << (TOTAL_BITS - index - 1)) >> (TOTAL_BITS - 1);
	value = (value & (0x00000001 << index)) >> index;
	uint8_t bitValue = (uint8_t) value;

	return bitValue;
}

uint8_t Binary::getLastBitByte(uint8_t value) {
	return getValueBit(value, 7);
}

} /* namespace SISBARC */
