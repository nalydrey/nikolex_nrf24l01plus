/*
 * Device.cpp
 *
 *  Created on: Jul 2, 2025
 *      Author: Oleksiy
 */

#include "../Inc/device.hpp"

using namespace nikolex::nrf24l01;

Device::Device(ReceiveTransmitter& receive_transmitter, OutputPin& pin_CE, Delay_us delay_us):
	receiveTransmitter(&receive_transmitter),
	pinCE(&pin_CE),
	delay(delay_us)
{
	receiveTransmitter->setDataReadyCb([](void * ctx){
		Device * self = (Device*) ctx;
		self->finishOperation();
	}, this);


	delay(100000);
	pinCE->setLow();
}


void Device::addStackItem(
		uint8_t registerAddress,
		EventCallback callback,
		void * userData,
		uint8_t payloadSize
		){
	StackItem item = {0};
	item.registerAddress = registerAddress;
	item.userCallback = callback;
	item.userData = userData;
	item.payloadSize = payloadSize;
	stack.push(item);
}


void Device::send(uint8_t* data, uint8_t dataSize){
	receiveTransmitter->transmitReceive(buffer, data, dataSize);
}


void Device::pulse(){
	pinCE->setHigh();
//	delay(400);
	delay(30);
	pinCE->setLow();
}





void Device::finishOperation(){
	StackItem* stackItem = stack.pop();

	Event evt;

	evt.status = buffer[0];
	evt.payload = buffer + 1;
	evt.payloadSize = stackItem->payloadSize;
	evt.registerAddress = stackItem->registerAddress;
	evt.userData = stackItem->userData;

	if(stackItem->userCallback) stackItem->userCallback(evt);
}




