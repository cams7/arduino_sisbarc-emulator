/*
 * Arduino.h
 *
 *  Created on: 21/01/2015
 *      Author: cams7
 */

#ifndef ARDUINOSTATUS_H_
#define ARDUINOSTATUS_H_

#include <inttypes.h>

namespace SISBARC {

typedef uint8_t transmitter;
typedef uint8_t status;
typedef uint8_t event;
typedef uint8_t pin_type;

class ArduinoStatus {

private:
	transmitter transmitterValue;
	status statusValue;
	event eventValue;
	pin_type pinType;

	uint8_t pin;

public:
	static const transmitter ARDUINO; //Mensagem enviada do Arduino
	static const transmitter OTHER_DEVICE; //Mensagem enviada do PC

	static const status SEND; //Mensagem de envio que nao exige uma resposta
	static const status SEND_RESPONSE; //Mensagem de envio que exige uma resposta
	static const status RESPONSE; //Mensagem de resposta
	static const status RESPONSE_RESPONSE; //Mensagem de resposta que exige outra resposta

	static const event EXECUTE;
	static const event WRITE;
	static const event READ;
	static const event MESSAGE;

	static const pin_type DIGITAL; //Porta Digital
	static const pin_type ANALOG; //Porta Analogica

	static const uint8_t TRANSMITTER_MAX;
	static const uint8_t STATUS_MAX;
	static const uint8_t EVENT_MAX;
	static const uint8_t PIN_TYPE_MAX;

	static const uint8_t DIGITAL_PIN_MAX;        //Numero maximo da porta e 63
	static const uint8_t ANALOG_PIN_MAX;        //Numero maximo da porta e 15

	static const uint8_t PINS_DIGITAL_SIZE;
	static const uint8_t PINS_DIGITAL[];

	static const uint8_t PINS_DIGITAL_PWM_SIZE;
	static const uint8_t PINS_DIGITAL_PWM[];

	static const uint8_t PINS_ANALOG_SIZE;
	static const uint8_t PINS_ANALOG[];

	ArduinoStatus();
	ArduinoStatus(status statusValue, event eventValue, pin_type pinType,
			uint8_t pin);

	virtual ~ArduinoStatus();

	transmitter getTransmitterValue(void);
	void setTransmitterValue(transmitter transmitterValue);

	status getStatusValue(void);
	void setStatusValue(status statusValue);

	event getEventValue(void);
	void setEventValue(event eventValue);

	pin_type getPinType(void);
	void setPinType(pin_type pinType);

	uint8_t getPin(void);
	void setPin(uint8_t pin);

};

} /* namespace SISBARC */

#endif /* ARDUINOSTATUS_H_ */
