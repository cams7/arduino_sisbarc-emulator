/*
 * CRC.h
 *
 *  Created on: 13/01/2015
 *      Author: cams7
 */

#ifndef CHECKSUM_H_
#define CHECKSUM_H_

#include <inttypes.h>

namespace SISBARC {

typedef uint8_t crc;

class Checksum {
private:
	static const uint8_t POLYNOMIAL;

	static const uint8_t WIDTH;
	static const uint8_t TOPBIT;

	static crc crcNaive(uint8_t const message);
	static crc crcSlow(uint8_t const message[], uint8_t totalBytes);

	static crc getCrcAll(uint32_t const message, uint8_t totalBytes);

public:
	static crc getCrc4Bytes(uint32_t const message);
	static crc getCrc3Bytes(uint32_t const message);
	static crc getCrc2Bytes(uint16_t const message);
	static crc getCrc1Byte(uint8_t const message);
};

} /* namespace SISBARC */

#endif /* CHECKSUM_H_ */
