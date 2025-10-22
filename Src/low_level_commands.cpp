/*
 * commands.cpp
 *
 *  Created on: Jul 29, 2025
 *      Author: Oleksiy
 */

#include "../Inc/device.hpp"

using namespace nikolex::nrf24l01;


void Device::readRegister(
		uint8_t registerAddress,
		uint8_t dataLength,
		EventCallback callback,
		void * userData
){
	addStackItem(registerAddress, callback, userData, dataLength);
	commandSequence[0] = COMMAND::R_REGISTER | (registerAddress & 0x1F);
	memset(commandSequence+1, COMMAND::PLUG, dataLength);
	send(commandSequence, dataLength+1);
}


void Device::writeRegister(
		uint8_t registerAddress,
		uint8_t * data,
		uint8_t dataSize,
		EventCallback callback,
		void * userData
){
	addStackItem(registerAddress, callback, userData);
	commandSequence[0] = COMMAND::W_REGISTER | (registerAddress & 0x1F);
	memcpy(commandSequence+1, data, dataSize);
	send(commandSequence, dataSize+1);
}


void Device::modifyRegister(
			uint8_t registerAddress,
			uint8_t mask,
			uint8_t newValue,
			EventCallback callback,
			void * userData
){
	addStackItem(registerAddress, callback, userData);
	modifyData = {mask, newValue};

	readRegister(registerAddress, 1, [](Event& evt){

		Device* self = (Device *)evt.userData;

		uint8_t oldValue = evt.payload[0];
		uint8_t bitMask = self->modifyData.mask;
		uint8_t newValue = self->modifyData.newValue;

		uint8_t result = (oldValue & ~bitMask) | (newValue & bitMask);

		self->writeRegister(evt.registerAddress, &result, 1, [](Event& evt){
			Device* self = (Device *)evt.userData;
			self->finishOperation();
		}, self);

	}, this);
}

void Device::flushTX(EventCallback callback, void * userData){
	addStackItem(0, callback, userData, 1);
	commandSequence[0] = COMMAND::FLUSH_TX;
	send(commandSequence, 2);
}


void Device::flushRX(EventCallback callback, void * userData){
	addStackItem(0, callback, userData, 1);
	commandSequence[0] = COMMAND::FLUSH_RX;
	send(commandSequence, 2);
}


void Device::reuseTXpayload(EventCallback callback, void * userData){
	addStackItem(0, callback, userData);
	commandSequence[0] = COMMAND::REUSE_TX_PL;
	send(commandSequence, 1);
}


void Device::writeACKpayload(
		PIPE pipe,
		uint8_t* payload,
		uint8_t payloadLength,
		EventCallback callback,
		void * userData
){
	addStackItem(0, callback, userData);

	uint8_t pipeNumber = -1;
	uint8_t currentValue = pipe;
	bool pending = true;

	while(pending){
		if((currentValue & 0x01) == 1){
			pending = false;
		}
		currentValue = currentValue >> 1;
		pipeNumber++;
	}

	commandSequence[0] = COMMAND::W_ACK_PAYLOAD | (pipeNumber & 0x07);
	memcpy(commandSequence + 1, payload, payloadLength);
	send(commandSequence, payloadLength + 1);
}


void Device::writeTXpayload(uint8_t* payload, uint8_t payloadLength, EventCallback callback, void * userData){
	addStackItem(0, callback, userData);
	commandSequence[0] = COMMAND::W_TX_PAYLOAD;
	memcpy(commandSequence + 1, payload, payloadLength);
	send(commandSequence, payloadLength + 1);
}


void Device::writeTXpayloadWithNOACK(uint8_t* payload, uint8_t payloadLength, EventCallback callback, void * userData){
	addStackItem(0, callback, userData);
	commandSequence[0] = COMMAND::W_TX_PAYLOAD_NO_ACK;
	memcpy(commandSequence + 1, payload, payloadLength);
	send(commandSequence, payloadLength + 1);
}





void Device::readRxPayload(uint8_t payloadLength,	EventCallback callback,	void * userData)
{
	addStackItem(0, callback, userData, payloadLength);
	commandSequence[0] = COMMAND::R_RX_PAYLOAD;
	memset(commandSequence + 1, COMMAND::PLUG, payloadLength);
	send(commandSequence, payloadLength + 1);
}


void Device::readRxPayloadWidth(EventCallback callback, void * userData){
	addStackItem(0, callback, userData, 1);
	commandSequence[0] = COMMAND::R_RX_PL_WID;
	send(commandSequence, 2);
}

