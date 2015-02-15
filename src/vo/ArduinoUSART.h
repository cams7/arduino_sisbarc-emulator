/*
 * ArduinoUSART.h
 *
 *  Created on: 31/01/2015
 *      Author: cams7
 */

#ifndef ARDUINOUSART_H_
#define ARDUINOUSART_H_

#include "ArduinoStatus.h"

namespace SISBARC {

class ArduinoUSART: public ArduinoStatus {
private:
	uint16_t _pinValue;

public:
	static const uint8_t DIGITAL_PIN_VALUE_MAX; //Valor maximo da porta e 255
	static const uint16_t ANALOG_PIN_VALUE_MAX; //Valor maximo da porta e 1023

	ArduinoUSART();

	ArduinoUSART(status statusValue, pin_type pinType, uint8_t pin,
			uint16_t pinValue);
	ArduinoUSART(status statusValue, event eventValue, pin_type pinType,
			uint8_t pin, uint16_t pinValue);

	ArduinoUSART(status statusValue, ArduinoPin* pin, uint16_t pinValue);
	ArduinoUSART(status statusValue, event eventValue, ArduinoPin* pin,
			uint16_t pinValue);

	virtual ~ArduinoUSART();

	uint16_t getPinValue(void);
	void setPinValue(uint16_t pinValue);

};

class ArduinoUSARTMessage: public ArduinoUSART {
public:
	ArduinoUSARTMessage();

	ArduinoUSARTMessage(status statusValue, pin_type pinType, uint8_t pin,
			uint16_t codeMessage);
	ArduinoUSARTMessage(status statusValue, ArduinoPin* pin,
			uint16_t codeMessage);

	virtual ~ArduinoUSARTMessage();
};

} /* namespace SISBARC */

#endif /* ARDUINOUSART_H_ */
