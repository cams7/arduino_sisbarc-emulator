/*
 * main.cpp
 *
 *  Created on: 10/02/2015
 *      Author: cams7
 */

//#include <inttypes.h>
//#include <stdlib.h>
#include <stdio.h>

#include "thread/Thread.h"
#include "thread/ThreadController.h"

#include "_Serial.h"
#include "SisbarcUSART.h"
//#include <iostream>

#include "ArduinoStatus.h"
#include "ArduinoUSART.h"
#include "ArduinoEEPROM.h"

using namespace SISBARC;
//using namespace std;

void setup(void);
void loop(void);

void serialEventRun(void);

void callTeste5s(void);
void callTeste1s(void);
void callTeste10s(void);
void callTeste30s(void);
void callTeste1m(void);

bool callSerialEventUSART(ArduinoStatus* arduino);
bool callSerialEventEEPROM(ArduinoStatus* arduino);

Thread* thread5s = new Thread();
Thread* thread1s = new Thread();
Thread* thread10s = new Thread();
Thread* thread30s = new Thread();
Thread* thread1m = new Thread();

int main(void) {
	setup();

	for (;;) {
		loop();
		//if (serialEventRun)
		if (Serial.available() /*> 0*/) //verifica se existe comunicação com a porta serial
			serialEventRun();
	}

	delete thread5s;
	delete thread1s;
	delete thread10s;
	delete thread30s;
	delete thread1m;

	return 0;
}

void setup(void) {
	thread5s->onRun(callTeste5s);
	thread5s->setInterval(1000 * 5); // 5 segundos

	thread1s->onRun(callTeste1s);
	thread1s->setInterval(1000); // 1 segundo

	thread10s->onRun(callTeste10s);
	thread10s->setInterval(1000 * 10); // 10 segundos

	thread30s->onRun(callTeste30s);
	thread30s->setInterval(1000 * 30); // 30 segundos

	thread1m->onRun(callTeste1m);
	thread1m->setInterval(1000 * 60); // 1 minuto

	THREAD_CONTROLLER.add(thread5s);
	THREAD_CONTROLLER.add(thread1s);
	THREAD_CONTROLLER.add(thread1m);
	THREAD_CONTROLLER.add(thread30s);
	THREAD_CONTROLLER.add(thread10s);

	SISBARC_USART.onRun(callSerialEventUSART);
	SISBARC_USART.onRun(callSerialEventEEPROM);
}

void loop(void) {
	THREAD_CONTROLLER.run();
}

void serialEventRun(void) {
	uint8_t data = (uint8_t) Serial.read(); //lê os dados da porta serial - Maximo 64 bytes
	SISBARC_USART.receiveDataBySerial(data);
}

void callTeste5s(void) {
	printf("callTeste5s()\n");
}

void callTeste1s(void) {
	printf("callTeste1s()\n");
}

void callTeste10s(void) {
	printf("callTeste10s()\n");
}

void callTeste30s(void) {
	printf("callTeste30s()\n");
}
void callTeste1m(void) {
	printf("callTeste1m()\n");
}

bool callSerialEventUSART(ArduinoStatus* arduino) {
	if (!((arduino->getEventValue() == ArduinoStatus::EXECUTE
			|| arduino->getEventValue() == ArduinoStatus::MESSAGE)))
		return false;

	printf(
			"USART [transmitter = %u, status = %u, event = %u, pinType = %u, pin = %u, pinValue = %u]\n",
			arduino->getTransmitterValue(), arduino->getStatusValue(),
			arduino->getEventValue(), arduino->getPinType(), arduino->getPin(),
			((ArduinoUSART*) arduino)->getPinValue());

	return true;
}

bool callSerialEventEEPROM(ArduinoStatus* arduino) {
	if (!(arduino->getEventValue() == ArduinoStatus::WRITE
			|| arduino->getEventValue() == ArduinoStatus::READ))
		return false;

	printf(
			"EEPROM [transmitter = %u, status = %u, event = %u, pinType = %u, pin = %u, threadTime = %u, actionEvent = %u]\n",
			arduino->getTransmitterValue(), arduino->getStatusValue(),
			arduino->getEventValue(), arduino->getPinType(), arduino->getPin(),
			((ArduinoEEPROM*) arduino)->getThreadTime(),
			((ArduinoEEPROM*) arduino)->getActionEvent());

	return true;
}

