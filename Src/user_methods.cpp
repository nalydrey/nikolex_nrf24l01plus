/*
 * modify_operations.cpp
 *
 *  Created on: Jul 30, 2025
 *      Author: Oleksiy
 */
#include "../Inc/device.hpp"

using namespace nikolex::nrf24l01;

void Device::setPowerDownMode(EventCallback callback, void * userData){
	pinCE->setLow();
	modifyRegister(REGISTER::CONFIG_REGISTER, MASK::POWER_MASK, 0, callback, userData);
}


void Device::setPowerUpMode(EventCallback callback, void * userData){
	addStackItem(REGISTER::CONFIG_REGISTER, callback, userData);
	pinCE->setLow();
	modifyRegister(REGISTER::CONFIG_REGISTER, MASK::POWER_MASK, 0b00000010, [](Event& evt){
		Device* self = (Device *)evt.userData;
		self->delay(2000);
		self->finishOperation();
	}, this);
}


void Device::setRXMode(EventCallback callback, void * userData){
	addStackItem(REGISTER::CONFIG_REGISTER, callback, userData);
	pinCE->setLow();
	modifyRegister(REGISTER::CONFIG_REGISTER, MASK::PRIMARY_MODE_MASK, 0b00000011, [](Event& evt){
		Device* self = (Device *)evt.userData;
		self->delay(200);
		self->pinCE->setHigh();
		self->mode = MODE::RX_MODE;
		self->finishOperation();
	}, this);
}


void Device::setTXMode(EventCallback callback, void * userData){
	addStackItem(REGISTER::CONFIG_REGISTER, callback, userData);
	pinCE->setLow();
	modifyRegister(REGISTER::CONFIG_REGISTER, MASK::PRIMARY_MODE_MASK, 0b00000010, [](Event& evt){
		Device* self = (Device *)evt.userData;
		self->delay(200);
		self->mode = MODE::TX_MODE;
		self->finishOperation();
	}, this);
}


void Device::setDataRate(DATA_RATE dataRate, EventCallback callback, void * userData){
	modifyRegister(REGISTER::RF_SETUP_REGISTER, MASK::DATA_RATE_MASK, dataRate, callback, userData);
}


void Device::setRFchannel(uint8_t ch, EventCallback callback, void * userData){
	channel = ch;
	modifyRegister(REGISTER::RF_CH_REGISTER, MASK::RF_CHANNEL_MASK, ch, callback, userData);
}


void Device::setPowerAmplifier(POWER_AMPLIFIER pa, EventCallback callback, void * userData){
	modifyRegister(REGISTER::RF_SETUP_REGISTER, MASK::PA_MASK, (pa<<1), callback, userData);
}

void Device::setCRClength(CRC_LENGTH crc_len, EventCallback callback, void * userData){
	modifyRegister(REGISTER::CONFIG_REGISTER, MASK::CRC_LENGTH_MASK, (crc_len<<2), callback, userData);
}


void Device::setDynamicPayloadLength(PIPE pipe, bool isEnable, EventCallback callback, void * userData){
	addStackItem(REGISTER::DYNPD_REGISTER, callback, userData);
	modifyRegister(REGISTER::DYNPD_REGISTER, MASK::DPL_PIPE_MASK & pipe, isEnable ? 255 : 0, [](Event& evt){
		Device* self = (Device *)evt.userData;

		self->readRegister(REGISTER::DYNPD_REGISTER, 1, [](Event& evt){
			Device* self = (Device *)evt.userData;

			uint8_t currentValue = evt.payload[0] & MASK::DPL_PIPE_MASK;

			self->modifyRegister(
					REGISTER::FEATURE_REGISTER,
					MASK::DPL_GLOBAL_MASK,
					(currentValue ? 1 : 0) << 2,
					[](Event& evt){
						Device* self = (Device *)evt.userData;

						self->finishOperation();
					}, self);
		}, self);
	}, this);
}



void Device::setAddressWidth(ADDRESS_WIDTH addressWidth, EventCallback callback, void * userData){
	modifyRegister(REGISTER::SETUP_AW_REGISTER, MASK::ADDRESS_WIDTH_MASK, addressWidth, callback, userData);
}


void Device::setRetransmitDelay(RETRANSMIT_DELAY delay, EventCallback callback, void * userData){
	modifyRegister(REGISTER::SETUP_RETR_REGISTER, MASK::RETRANSMIT_DELAY_MASK, delay << 4, callback, userData);
}


void Device::setRetransmitCounter(uint8_t counter, EventCallback callback, void * userData){
	uint8_t value = (counter > 15) ? 15 : counter;
	modifyRegister(REGISTER::SETUP_RETR_REGISTER, MASK::RETRANSMIT_COUNT_MASK, value, callback, userData);
}


void Device::setACKpayload(bool isEnable, EventCallback callback, void * userData){
	modifyRegister(REGISTER::FEATURE_REGISTER, MASK::ACK_PAYLOAD_MASK, isEnable << 1, callback, userData);
}


void Device::sendMessage(EventCallback callback, void * userData){
	addStackItem(0, callback, userData);
	pulse();
	finishOperation();
}


void Device::writeFullAddress(uint8_t registerAddress, uint8_t* address,	EventCallback callback,	void * userData){

	addStackItem(registerAddress, callback, userData);

	tempAddr = address;
	tempRegisterAddress = registerAddress;

	readRegister(REGISTER::SETUP_AW_REGISTER, 1, [](Event& evt){
		Device* self = (Device*)evt.userData;
		uint8_t addressWidth = (evt.payload[0] & 0x03) + 2;

		self->writeRegister(self->tempRegisterAddress, self->tempAddr, addressWidth, [](Event& evt){
			Device* self = (Device*)evt.userData;

			self->readRegister(evt.registerAddress, 5, [](Event& evt){
				Device* self = (Device*)evt.userData;
				self->finishOperation();
			}, self);
		}, self);
	}, this);
}

void Device::setTxAddress(uint8_t* address, EventCallback callback, void * userData){
	writeFullAddress(REGISTER::TX_ADDR_REGISTER, address, callback, userData);

}


void Device::setPipe0Address(uint8_t* address, EventCallback callback, void * userData){
	writeFullAddress(REGISTER::RX_ADDR_P0_REGISTER, address, callback, userData);
}


void Device::setPipe1Address(uint8_t* address, EventCallback callback, void * userData){
	writeFullAddress(REGISTER::RX_ADDR_P1_REGISTER, address, callback, userData);
}


void Device::setPipe2Address(uint8_t lastByte, EventCallback callback, void * userData){
	writeRegister(REGISTER::RX_ADDR_P2_REGISTER, &lastByte, 1, callback, userData);
}


void Device::setPipe3Address(uint8_t lastByte, EventCallback callback, void * userData){
	writeRegister(REGISTER::RX_ADDR_P3_REGISTER, &lastByte, 1, callback, userData);
}


void Device::setPipe4Address(uint8_t lastByte, EventCallback callback, void * userData){
	writeRegister(REGISTER::RX_ADDR_P4_REGISTER, &lastByte, 1, callback, userData);
}


void Device::setPipe5Address(uint8_t lastByte, EventCallback callback, void * userData){
	writeRegister(REGISTER::RX_ADDR_P5_REGISTER, &lastByte, 1, callback, userData);
}


void Device::readRxPayloadDynamically(EventCallback callback, void * userData){
	addStackItem(0, callback, userData);

	readRxPayloadWidth([](Event& evt){
		Device* self = (Device*)evt.userData;

		self->readRxPayload(evt.payload[0], [](Event& evt){
			Device* self = (Device*)evt.userData;
			self->stack.getCurrentItem()->payloadSize = evt.payloadSize;
			self->finishOperation();
		}, self);
	}, this);
}


void Device::IRQHandler(){

	readRegister(REGISTER::FIFO_STATUS_REGISTER, 1, [](Event& evt){
		Device* self = (Device *)evt.userData;

		IRQEvent irqEvent = {0};

		irqEvent.device = self;
		irqEvent.dataReady = (evt.status >> 6) & 0x01;
		irqEvent.dataSent = (evt.status >> 5) & 0x01;
		irqEvent.maxRetransmits = (evt.status >> 4) & 0x01;
		irqEvent.pipeNumber = (evt.status >> 1) & 0x07;
		irqEvent.txFifoFull = evt.status & 0x01;

		irqEvent.txFull = (evt.payload[0] >> 5) & 0x01;
		irqEvent.txEmpty = (evt.payload[0] >> 4) & 0x01;
		irqEvent.rxFull = (evt.payload[0] >> 1) & 0x01;
		irqEvent.rxEmpty = evt.payload[0] & 0x01;

		if(self->IRQCallback) self->IRQCallback(irqEvent);

		self->modifyRegister(
				REGISTER::STATUS_REGISTER,
				(irqEvent.dataReady << 6) | (irqEvent.dataSent << 5) | (irqEvent.maxRetransmits << 4),
				255
			);
		}, this);
}


