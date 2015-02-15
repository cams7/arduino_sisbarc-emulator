/*
 * CRC.cpp
 *
 *  Created on: 13/01/2015
 *      Author: cams7
 */

#include "Checksum.h"

//#include <Arduino.h>
#include <stdlib.h>

#include "Binary.h"

namespace SISBARC {

const uint8_t Checksum::POLYNOMIAL = 0xD8; /* 11011 followed by 0's */
const uint8_t Checksum::WIDTH = (8 * sizeof(crc));
const uint8_t Checksum::TOPBIT = (1 << (WIDTH - 1));

//Gera CRC para 1 byte
crc Checksum::crcNaive(uint8_t const message) {
	crc remainder;

	/*
	 * Initially, the dividend is the remainder.
	 */
	remainder = message;

	/*
	 * For each bit position in the message....
	 */
	for (int8_t i = 8; i > 0; --i) {
		/*
		 * If the uppermost bit is a 1...
		 */
		if (remainder & 0x80) {
			/*
			 * XOR the previous remainder with the divisor.
			 */
			remainder ^= POLYNOMIAL;
		}

		/*
		 * Shift the next bit of the message into the remainder.
		 */
		remainder = (remainder << 1);
	}

	/*
	 * Return only the relevant bits of the remainder as CRC.
	 */
	return (remainder >> 4);

} /* crcNaive() */

//Gera CRC para um array de bytes
crc Checksum::crcSlow(uint8_t const message[], uint8_t totalBytes) {
	crc remainder = 0;

	/*
	 * Perform modulo-2 division, a byte at a time.
	 */
	for (uint8_t i = 0; i < totalBytes; ++i) {
		/*
		 * Bring the next byte into the remainder.
		 */
		remainder ^= (message[i] << (WIDTH - 8));

		/*
		 * Perform modulo-2 division, a bit at a time.
		 */
		for (uint8_t bit = 8; bit > 0; --bit) {
			/*
			 * Try to divide the current data bit.
			 */
			if (remainder & TOPBIT) {
				remainder = (remainder << 1) ^ POLYNOMIAL;
			} else {
				remainder = (remainder << 1);
			}
		}
	}

	/*
	 * The final remainder is the CRC result.
	 */
	return (remainder);

} /* crcSlow() */

//Gera CRC para 1,2,3 e 4 bytes
crc Checksum::getCrcAll(uint32_t const message, uint8_t totalBytes) {
	if (totalBytes < 1 || totalBytes > 4)
		return 0;

	crc checksum;

	if (totalBytes > 1) {
		uint8_t *bytes;
		bytes = NULL;

		switch (totalBytes) {
		case 2:
			bytes = Binary::intTo2Bytes(message);
			break;
		case 3:
			bytes = Binary::intTo3Bytes(message);
			break;
		case 4:
			bytes = Binary::intTo4Bytes(message);
			break;
		default:
			break;
		}

		checksum = crcSlow(bytes, totalBytes);
		free(bytes);
	} else
		checksum = crcNaive((uint8_t) message);

	return checksum;
}

//Gera CRC para 4 bytes
crc Checksum::getCrc4Bytes(uint32_t const message) {
	crc checksum = getCrcAll(message, 4);
	return checksum;
}

//Gera CRC para 3 bytes
crc Checksum::getCrc3Bytes(uint32_t const message) {
	crc checksum = getCrcAll(message, 3);
	return checksum;
}

//Gera CRC para 2 bytes
crc Checksum::getCrc2Bytes(uint16_t const message) {
	crc checksum = getCrcAll(message, 2);
	return checksum;
}

//Gera CRC para 1 byte
crc Checksum::getCrc1Byte(uint8_t const message) {
	crc checksum = getCrcAll(message, 1);
	return checksum;
}

} /* namespace SISBARC */
