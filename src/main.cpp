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

void callTeste1(void);
void callTeste2(void);
void callTeste3(void);

bool callSerialEventUSART(ArduinoStatus* arduino);
bool callSerialEventEEPROM(ArduinoStatus* arduino);

Thread* thread1 = new Thread();
Thread* thread2 = new Thread();
Thread* thread3 = new Thread();

int main(void) {
	setup();

	for (;;) {
		loop();
		//if (serialEventRun)
		if (Serial.available() /*> 0*/) //verifica se existe comunicação com a porta serial
			serialEventRun();
	}

	delete thread1;
	delete thread2;
	delete thread3;

	return 0;
}

void setup(void) {
	thread1->onRun(callTeste1);
	thread1->setInterval(1000 * 5); // 5 segundos

	thread2->onRun(callTeste2);
	thread2->setInterval(1000 * 3); // 3 segundos

	thread3->onRun(callTeste3);
	thread3->setInterval(1000 * 10); // 10 segundos

	THREAD_CONTROLLER.add(thread1);
	THREAD_CONTROLLER.add(thread2);
	THREAD_CONTROLLER.add(thread3);

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

void callTeste1(void) {
	printf("callTeste1()\n");
}

void callTeste2(void) {
	printf("callTeste2()\n");
}

void callTeste3(void) {
	printf("callTeste3()\n");
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

