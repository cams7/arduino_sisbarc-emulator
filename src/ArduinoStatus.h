/*
 * Arduino.h
 *
 *  Created on: 21/01/2015
 *      Author: cams7
 */

#ifndef ARDUINOSTATUS_H_
#define ARDUINOSTATUS_H_

#include <inttypes.h>
#include "ArduinoPin.h"

namespace SISBARC {

typedef uint8_t transmitter;
typedef uint8_t status;
typedef uint8_t event;

class ArduinoStatus: public ArduinoPin {

private:
	transmitter _transmitterValue;
	status _statusValue;
	event _eventValue;

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

	static const uint8_t TRANSMITTER_MAX;
	static const uint8_t STATUS_MAX;
	static const uint8_t EVENT_MAX;

	ArduinoStatus();
	ArduinoStatus(status statusValue, event eventValue, pin_type pinType,
			uint8_t pin);
	ArduinoStatus(status statusValue, event eventValue, ArduinoPin* pin);

	virtual ~ArduinoStatus();

	transmitter getTransmitterValue(void);
	void setTransmitterValue(transmitter transmitterValue);

	status getStatusValue(void);
	void setStatusValue(status statusValue);

	event getEventValue(void);
	void setEventValue(event eventValue);

};

} /* namespace SISBARC */

#endif /* ARDUINOSTATUS_H_ */
