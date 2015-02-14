/*
 * main.cpp
 *
 *  Created on: 10/02/2015
 *      Author: cams7
 */

#include <stdio.h>

#include "ArduinoStatus.h"
#include "ArduinoUSART.h"
#include "ArduinoEEPROM.h"

#include "Sisbarc.h"
#include "SisbarcEEPROM.h"

#define PIN_LED_PISCA    13   //Pino 13 Digital

#define PIN_LED_AMARELA  11   //Pino 11 PWM
#define PIN_LED_VERDE    10   //Pino 10 PWM
#define PIN_LED_VERMELHA 9    //Pino 09 PWM

#define PIN_BOTAO_LED_AMARELA  12 //Pino 12 Digital
#define PIN_BOTAO_LED_VERDE    8  //Pino 8 Digital
#define PIN_BOTAO_LED_VERMELHA 7  //Pino 7 Digital

#define PIN_POTENCIOMETRO      0  //Pino 0 Analogico

#define THREAD_INTERVAL_LED_PISCA    10000 // 10 segundos
#define THREAD_INTERVAL_BOTAO        13000  // 13 segundo
#define THREAD_INTERVAL_POENCIOMETRO 15000  // 15 segundos

#define ON_OFF 0 // Acende ou apaga
#define BLINK  1 // Pisca-pisca
#define FADE   2// Acende ao poucos

#define THREAD_INTERVAL_100MILLIS  100  // 1/10 de segundo
#define THREAD_INTERVAL_250MILLIS  250  // 1/4 de segundo
#define THREAD_INTERVAL_500MILLIS  500  // 1/2 de segundo
#define THREAD_INTERVAL_1SECOUND   1000 // 1 segundo
#define THREAD_INTERVAL_2SECOUNDS  2000 // 2 segundos
#define THREAD_INTERVAL_3SECOUNDS  3000 // 3 segundos
#define THREAD_INTERVAL_5SECOUNDS  5000 // 5 segundos
#define NO_THREAD_INTERVAL         0    // Não roda dentro da thread

void setup(void);
void loop(void);

using namespace SISBARC;

bool callLEDPisca(ArduinoStatus* const);

bool callLEDAmarela(ArduinoStatus* const);
bool callLEDVerde(ArduinoStatus* const);
bool callLEDVermelha(ArduinoStatus* const);

bool callBotaoLEDAmarela(ArduinoStatus* const);
bool callBotaoLEDVerde(ArduinoStatus* const);
bool callBotaoLEDVermelha(ArduinoStatus* const);

bool callPOTENCIOMETRO(ArduinoStatus* const);

int main(void) {
	setup();

	for (;;)
		loop();

	return 0;
}

void setup(void) {
	Sisbarc.addThreadInterval(0x00, THREAD_INTERVAL_100MILLIS);
	Sisbarc.addThreadInterval(0x01, THREAD_INTERVAL_250MILLIS);
	Sisbarc.addThreadInterval(0x02, THREAD_INTERVAL_500MILLIS);
	Sisbarc.addThreadInterval(0x03, THREAD_INTERVAL_1SECOUND);
	Sisbarc.addThreadInterval(0x04, THREAD_INTERVAL_2SECOUNDS);
	Sisbarc.addThreadInterval(0x05, THREAD_INTERVAL_3SECOUNDS);
	Sisbarc.addThreadInterval(0x06, THREAD_INTERVAL_5SECOUNDS);
	Sisbarc.addThreadInterval(0x07, NO_THREAD_INTERVAL);

	Sisbarc.onRun(ArduinoStatus::DIGITAL, PIN_LED_PISCA, callLEDPisca,
	THREAD_INTERVAL_LED_PISCA);

	Sisbarc.onRun(ArduinoStatus::DIGITAL, PIN_LED_AMARELA, callLEDAmarela);

	Sisbarc.onRun(ArduinoStatus::DIGITAL, PIN_LED_VERDE, callLEDVerde);

	Sisbarc.onRun(ArduinoStatus::DIGITAL, PIN_LED_VERMELHA, callLEDVermelha);

	Sisbarc.onRun(ArduinoStatus::DIGITAL, PIN_BOTAO_LED_AMARELA,
			callBotaoLEDAmarela, THREAD_INTERVAL_BOTAO);

	Sisbarc.onRun(ArduinoStatus::DIGITAL, PIN_BOTAO_LED_VERDE,
			callBotaoLEDVerde, THREAD_INTERVAL_BOTAO);

	Sisbarc.onRun(ArduinoStatus::DIGITAL, PIN_BOTAO_LED_VERMELHA,
			callBotaoLEDVermelha, THREAD_INTERVAL_BOTAO);

	Sisbarc.onRun(ArduinoStatus::ANALOG, PIN_POTENCIOMETRO, callPOTENCIOMETRO,
	THREAD_INTERVAL_POENCIOMETRO);

}

void loop(void) {
	Sisbarc.run();
}

bool isCallBySerial(ArduinoStatus* const arduino) {
	if (ArduinoStatus::OTHER_DEVICE != arduino->getTransmitterValue())
		return false;

	if (!(ArduinoStatus::SEND_RESPONSE == arduino->getStatusValue()
			|| ArduinoStatus::RESPONSE_RESPONSE == arduino->getStatusValue()))
		return false;

	return true;
}

bool isCallBySerialToPinDigital(ArduinoStatus* const arduino) {
	if (!isCallBySerial(arduino))
		return false;

	if (arduino->getPinType() != ArduinoStatus::DIGITAL)
		return false;

	return true;
}

bool isCallBySerialToPinAnalog(ArduinoStatus* const arduino) {
	if (!isCallBySerial(arduino))
		return false;

	if (arduino->getPinType() != ArduinoStatus::ANALOG)
		return false;

	return true;
}

void write(ArduinoStatus* const arduino) {
	ArduinoEEPROMWrite* arduinoEEPROMWrite = ((ArduinoEEPROMWrite*) arduino);

	int16_t returnValue = SisbarcEEPROM::write(arduinoEEPROMWrite);
	if (returnValue != 0x0000 && returnValue != 0x0001)
		return;

	arduinoEEPROMWrite->setTransmitterValue(ArduinoStatus::ARDUINO);
	arduinoEEPROMWrite->setStatusValue(ArduinoStatus::RESPONSE);
	Sisbarc.send(arduinoEEPROMWrite);
	printf(
			"WRITE [transmitter = %u, status = %u, event = %u, pinType = %u, pin = %u, threadInterval = %u, actionEvent = %u]\n",
			arduino->getTransmitterValue(), arduino->getStatusValue(),
			arduino->getEventValue(), arduino->getPinType(), arduino->getPin(),
			arduinoEEPROMWrite->getThreadInterval(),
			arduinoEEPROMWrite->getActionEvent());
}

void execute(ArduinoStatus* const arduino) {
	printf(
			"EXECUTE [transmitter = %u, status = %u, event = %u, pinType = %u, pin = %u, pinValue = %u]\n",
			arduino->getTransmitterValue(), arduino->getStatusValue(),
			arduino->getEventValue(), arduino->getPinType(), arduino->getPin(),
			((ArduinoUSART*) arduino)->getPinValue());
}

bool callLEDPisca(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("Define: callLEDPisca()\n");
	} else {
		//printf("By serial: callLEDPisca()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_LED_PISCA)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::WRITE) {
			printf("LED PISCA - ");
			write(arduino);

			return true;
		}
	}
	return false;
}

bool callLEDAmarela(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("Define: callLEDAmarela()\n");
	} else {
		//printf("By serial: callLEDAmarela()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_LED_AMARELA)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::WRITE) {
			printf("LED AMARELA - ");
			write(arduino);

			return true;
		} else if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("LED AMARELA - ");
			execute(arduino);

			return true;
		}
	}
	return false;
}

bool callLEDVerde(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("Define: callLEDVerde()\n");
	} else {
		//printf("By serial: callLEDVerde()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_LED_VERDE)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::WRITE) {
			printf("LED VERDE - ");
			write(arduino);

			return true;
		} else if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("LED VERDE - ");
			execute(arduino);

			return true;
		}
	}
	return false;
}

bool callLEDVermelha(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("Define: callLEDVermelha()\n");
	} else {
		//printf("By serial: callLEDVermelha()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_LED_VERMELHA)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::WRITE) {
			printf("LED VERMELHA - ");
			write(arduino);

			return true;
		} else if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("LED VERMELHA - ");
			execute(arduino);

			return true;
		}
	}
	return false;
}

bool callBotaoLEDAmarela(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("Define: callBotaoLEDAmarela()\n");
	} else {
		//printf("By serial: callBotaoLEDAmarela()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_BOTAO_LED_AMARELA)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("BOTAO LED AMARELA - ");
			execute(arduino);

			return true;
		}
	}
	return false;
}

bool callBotaoLEDVerde(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("Define: callBotaoLEDVerde()\n");
	} else {
		//printf("By serial: callBotaoLEDVerde()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_BOTAO_LED_VERDE)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("BOTAO LED VERDE - ");
			execute(arduino);

			return true;
		}
	}
	return false;
}

bool callBotaoLEDVermelha(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("Define: callBotaoLEDVermelha()\n");
	} else {
		//printf("By serial: callBotaoLEDVermelha()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_BOTAO_LED_VERMELHA)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("BOTAO LED VERMELHA - ");
			execute(arduino);

			return true;
		}
	}
	return false;
}

bool callPOTENCIOMETRO(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("Define: callPOTENCIOMETRO()\n");
	} else {
		printf("By serial: callPOTENCIOMETRO()\n");
	}
	return false;
}

