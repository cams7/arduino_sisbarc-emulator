/*
 * ArduinoPin.h
 *
 *  Created on: 13/02/2015
 *      Author: cams7
 */

#ifndef ARDUINOPIN_H_
#define ARDUINOPIN_H_

#include <inttypes.h>

namespace SISBARC {

typedef uint8_t pin_type;

class ArduinoPin {
private:
	pin_type _pinType;
	uint8_t _pin;

public:
	static const pin_type DIGITAL; //Porta Digital
	static const pin_type ANALOG; //Porta Analogica

	static const uint8_t PIN_TYPE_MAX;

	static const uint8_t DIGITAL_PIN_MAX;        //Numero maximo da porta e 63
	static const uint8_t ANALOG_PIN_MAX;        //Numero maximo da porta e 15

	static const uint8_t PINS_DIGITAL_SIZE;
	static const uint8_t PINS_DIGITAL[];

	static const uint8_t PINS_DIGITAL_PWM_SIZE;
	static const uint8_t PINS_DIGITAL_PWM[];

	static const uint8_t PINS_ANALOG_SIZE;
	static const uint8_t PINS_ANALOG[];

	ArduinoPin();
	ArduinoPin(pin_type pinType, uint8_t pin);

	virtual ~ArduinoPin();

	pin_type getPinType(void);
	void setPinType(pin_type pinType);

	uint8_t getPin(void);
	void setPin(uint8_t pin);
};
}

#endif /* ARDUINOPIN_H_ */
