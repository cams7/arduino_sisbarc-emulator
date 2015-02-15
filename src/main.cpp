/*
 * main.cpp
 *
 *  Created on: 10/02/2015
 *      Author: cams7
 */

#include <stdio.h>

#include "vo/ArduinoStatus.h"
#include "vo/ArduinoUSART.h"
#include "vo/ArduinoEEPROM.h"

#include "Sisbarc.h"
#include "SisbarcEEPROM.h"
#include "SisbarcProtocol.h"

#include "_Serial.h"

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
void serialEventRun(void);

using namespace SISBARC;

void serialWrite(uint8_t* const);

bool callLEDPisca(ArduinoStatus* const);

bool callLEDAmarela(ArduinoStatus* const);
bool callLEDVerde(ArduinoStatus* const);
bool callLEDVermelha(ArduinoStatus* const);

bool callBotaoLEDAmarela(ArduinoStatus* const);
bool callBotaoLEDVerde(ArduinoStatus* const);
bool callBotaoLEDVermelha(ArduinoStatus* const);

bool callPOTENCIOMETRO(ArduinoStatus* const);

SisbarcClass* sisbarc = new SisbarcClass();

int main(void) {
	setup();

	for (;;) {
		loop();

		//if (serialEventRun)
		if (Serial.available() > 0) //verifica se existe comunicação com a porta serial
			serialEventRun();
	}

	return 0;
}

void setup(void) {
	sisbarc->onRunSerialWrite(serialWrite);

	sisbarc->addThreadInterval(0x00, THREAD_INTERVAL_100MILLIS);
	sisbarc->addThreadInterval(0x01, THREAD_INTERVAL_250MILLIS);
	sisbarc->addThreadInterval(0x02, THREAD_INTERVAL_500MILLIS);
	sisbarc->addThreadInterval(0x03, THREAD_INTERVAL_1SECOUND);
	sisbarc->addThreadInterval(0x04, THREAD_INTERVAL_2SECOUNDS);
	sisbarc->addThreadInterval(0x05, THREAD_INTERVAL_3SECOUNDS);
	sisbarc->addThreadInterval(0x06, THREAD_INTERVAL_5SECOUNDS);
	sisbarc->addThreadInterval(0x07, NO_THREAD_INTERVAL);

	sisbarc->onRun(ArduinoStatus::DIGITAL, PIN_LED_PISCA, callLEDPisca,
	THREAD_INTERVAL_LED_PISCA);

	sisbarc->onRun(ArduinoStatus::DIGITAL, PIN_LED_AMARELA, callLEDAmarela);

	sisbarc->onRun(ArduinoStatus::DIGITAL, PIN_LED_VERDE, callLEDVerde);

	sisbarc->onRun(ArduinoStatus::DIGITAL, PIN_LED_VERMELHA, callLEDVermelha);

	sisbarc->onRun(ArduinoStatus::DIGITAL, PIN_BOTAO_LED_AMARELA,
			callBotaoLEDAmarela, THREAD_INTERVAL_BOTAO);

	sisbarc->onRun(ArduinoStatus::DIGITAL, PIN_BOTAO_LED_VERDE,
			callBotaoLEDVerde, THREAD_INTERVAL_BOTAO);

	sisbarc->onRun(ArduinoStatus::DIGITAL, PIN_BOTAO_LED_VERMELHA,
			callBotaoLEDVermelha, THREAD_INTERVAL_BOTAO);

	sisbarc->onRun(ArduinoStatus::ANALOG, PIN_POTENCIOMETRO, callPOTENCIOMETRO,
	THREAD_INTERVAL_POENCIOMETRO);

}

void loop(void) {
	sisbarc->run();
}

void serialEventRun(void) {
	uint8_t data = (uint8_t) Serial.read(); //lê os dados da porta serial - Maximo 64 bytes
	sisbarc->serialEventRun(data);
}

void serialWrite(uint8_t* const data) {
	Serial.write(data, SisbarcProtocol::TOTAL_BYTES_PROTOCOL);
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

void writeThenRespond(ArduinoStatus* const arduino) {
	ArduinoEEPROMWrite* arduinoEEPROMWrite = ((ArduinoEEPROMWrite*) arduino);

	int16_t returnValue = SisbarcEEPROM::write(arduinoEEPROMWrite);
	if (returnValue != 0x0000 && returnValue != 0x0001)
		return;

	arduinoEEPROMWrite->setTransmitterValue(ArduinoStatus::ARDUINO);
	arduinoEEPROMWrite->setStatusValue(ArduinoStatus::RESPONSE);
	sisbarc->send(arduinoEEPROMWrite);
	//printf(
	//		"WRITE [transmitter = %u, status = %u, event = %u, pinType = %u, pin = %u, threadInterval = %u, actionEvent = %u]\n",
	//		arduino->getTransmitterValue(), arduino->getStatusValue(),
	//		arduino->getEventValue(), arduino->getPinType(), arduino->getPin(),
	//		arduinoEEPROMWrite->getThreadInterval(),
	//		arduinoEEPROMWrite->getActionEvent());
}

void executeThenRespond(ArduinoStatus* const arduino) {
	arduino->setTransmitterValue(ArduinoStatus::ARDUINO);
	arduino->setStatusValue(ArduinoStatus::RESPONSE);
	sisbarc->send(arduino);
	//printf(
	//		"EXECUTE [transmitter = %u, status = %u, event = %u, pinType = %u, pin = %u, pinValue = %u]\n",
	//		arduino->getTransmitterValue(), arduino->getStatusValue(),
	//		arduino->getEventValue(), arduino->getPinType(), arduino->getPin(),
	//		((ArduinoUSART*) arduino)->getPinValue());
}

bool callLEDPisca(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("callLEDPisca()\n");
	} else {
		//printf("By serial: callLEDPisca()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_LED_PISCA)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::WRITE) {
			printf("LED PISCA - ");
			writeThenRespond(arduino);

			return true;
		}
	}
	return false;
}

bool callLEDAmarela(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("callLEDAmarela()\n");
	} else {
		//printf("By serial: callLEDAmarela()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_LED_AMARELA)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::WRITE) {
			printf("LED AMARELA - ");
			writeThenRespond(arduino);

			return true;
		} else if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("LED AMARELA - ");
			executeThenRespond(arduino);

			return true;
		}
	}
	return false;
}

bool callLEDVerde(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("callLEDVerde()\n");
	} else {
		//printf("By serial: callLEDVerde()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_LED_VERDE)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::WRITE) {
			printf("LED VERDE - ");
			writeThenRespond(arduino);

			return true;
		} else if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("LED VERDE - ");
			executeThenRespond(arduino);

			return true;
		}
	}
	return false;
}

bool callLEDVermelha(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("callLEDVermelha()\n");
	} else {
		//printf("By serial: callLEDVermelha()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_LED_VERMELHA)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::WRITE) {
			printf("LED VERMELHA - ");
			writeThenRespond(arduino);

			return true;
		} else if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("LED VERMELHA - ");
			executeThenRespond(arduino);

			return true;
		}
	}
	return false;
}

bool callBotaoLEDAmarela(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("callBotaoLEDAmarela()\n");
	} else {
		//printf("By serial: callBotaoLEDAmarela()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_BOTAO_LED_AMARELA)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("BOTAO LED AMARELA - ");
			executeThenRespond(arduino);

			return true;
		}
	}
	return false;
}

bool callBotaoLEDVerde(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("callBotaoLEDVerde()\n");
	} else {
		//printf("By serial: callBotaoLEDVerde()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_BOTAO_LED_VERDE)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("BOTAO LED VERDE - ");
			executeThenRespond(arduino);

			return true;
		}
	}
	return false;
}

bool callBotaoLEDVermelha(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("callBotaoLEDVermelha()\n");
	} else {
		//printf("By serial: callBotaoLEDVermelha()\n");

		if (!isCallBySerialToPinDigital(arduino))
			return false;

		if (arduino->getPin() != PIN_BOTAO_LED_VERMELHA)
			return false;

		if (arduino->getEventValue() == ArduinoStatus::EXECUTE) {
			printf("BOTAO LED VERMELHA - ");
			executeThenRespond(arduino);

			return true;
		}
	}
	return false;
}

bool callPOTENCIOMETRO(ArduinoStatus* const arduino) {
	if (arduino == NULL) {
		printf("callPOTENCIOMETRO()\n");
	} else {
		printf("By serial: callPOTENCIOMETRO()\n");
	}
	return false;
}

