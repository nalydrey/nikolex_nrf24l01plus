/*
 * nrf24l01.hpp
 *
 *  Created on: Jul 2, 2025
 *      Author: Oleksiy
 */

#ifndef NIKOLEX_NRF24L01_DEVICEHPP_
#define NIKOLEX_NRF24L01_DEVICEHPP_

#include <stdint.h>
#include <cstring>


#include "../../utils/stack.hpp"
#include "../../interfaces/output_pin.hpp"
#include "../../interfaces/receive_transmitter.hpp"
#include "commands.hpp"
#include "defines.hpp"
#include "masks.hpp"
#include "registers.hpp"
#include "types.hpp"

namespace nikolex::nrf24l01 {

class Device {

private:

	MODE mode;
	uint8_t channel;
	uint8_t buffer[33];
	uint8_t commandSequence[33];
	ReceiveTransmitter* receiveTransmitter;
	Stack<StackItem, 5> stack;
	ModifyData modifyData;
	uint8_t* tempAddr;
	uint8_t tempRegisterAddress;
	OutputPin* pinCE;
	Delay_us delay;
	IRQCallback_t IRQCallback = nullptr;


	void send(uint8_t*, uint8_t);
	void addStackItem(
			uint8_t registerAddress,
			EventCallback callback,
			void * userData,
			uint8_t payloadSize = 0
	);
	void pulse();
	void finishOperation();
	void writeFullAddress(
			uint8_t registerAddress,
			uint8_t* address,
			EventCallback callback = nullptr,
			void * userData  = nullptr
	);


public:

	Device(ReceiveTransmitter& receive_transmitter, OutputPin& pin_CE, Delay_us delay_us);
	friend PIPE operator|(PIPE left, PIPE right){return PIPE(uint8_t(left) | uint8_t(right));}


	void readRegister(
			uint8_t registerAddress,
			uint8_t dataLength,
			EventCallback callback = nullptr,
			void * userData  = nullptr
	);

	void writeRegister(
			uint8_t registerAddress,
			uint8_t * data,
			uint8_t dataSize,
			EventCallback callback = nullptr,
			void * userData  = nullptr
	);

	void modifyRegister(
			uint8_t registerAddress,
			uint8_t mask,
			uint8_t newValue,
			EventCallback callback = nullptr,
			void * userData  = nullptr
	);

	void writeACKpayload(
			PIPE pipe,
			uint8_t* payload,
			uint8_t payloadLength,
			EventCallback callback = nullptr,
			void * userData  = nullptr
	);

	void writeTXpayload(
			uint8_t* payload,
			uint8_t payloadLength,
			EventCallback callback = nullptr,
			void * userData  = nullptr
	);

	void writeTXpayloadWithNOACK(
			uint8_t* payload,
			uint8_t payloadLength,
			EventCallback callback = nullptr,
			void * userData  = nullptr
	);

	void readRxPayload(uint8_t payloadLength, EventCallback callback = nullptr, void * userData  = nullptr);
	void readRxPayloadWidth(EventCallback callback = nullptr, void * userData  = nullptr);
	void flushTX(EventCallback callback = nullptr, void * userData  = nullptr);
	void flushRX(EventCallback callback = nullptr, void * userData  = nullptr);
	void reuseTXpayload(EventCallback callback = nullptr, void * userData  = nullptr);



	void readRxPayloadDynamically(EventCallback callback = nullptr, void * userData  = nullptr);
	void setPowerDownMode(EventCallback callback = nullptr, void * userData  = nullptr);
	void setPowerUpMode(EventCallback callback = nullptr, void * userData  = nullptr);
	void setRXMode(EventCallback callback = nullptr, void * userData  = nullptr);
	void setTXMode(EventCallback callback = nullptr, void * userData  = nullptr);
	void setDataRate(DATA_RATE dataRate, EventCallback callback = nullptr, void * userData  = nullptr);
	void setRFchannel(uint8_t ch, EventCallback callback = nullptr, void * userData  = nullptr);
	void setPowerAmplifier(POWER_AMPLIFIER pa, EventCallback callback = nullptr, void * userData  = nullptr);
	void setCRClength(CRC_LENGTH crc_len,  EventCallback callback = nullptr, void * userData  = nullptr);
	void setDynamicPayloadLength(PIPE pipe, bool isEnable, EventCallback callback = nullptr, void * userData  = nullptr);
	void setAddressWidth(ADDRESS_WIDTH addressWidth, EventCallback callback = nullptr, void * userData  = nullptr);
	void setRetransmitDelay(RETRANSMIT_DELAY delay, EventCallback callback = nullptr, void * userData  = nullptr);
	void setRetransmitCounter(uint8_t counter, EventCallback callback = nullptr, void * userData  = nullptr);
	void setACKpayload(bool isEnable , EventCallback callback = nullptr, void * userData  = nullptr);
	void setTxAddress( uint8_t* address, EventCallback callback = nullptr, void * userData  = nullptr);
	void setPipe0Address( uint8_t* address, EventCallback callback = nullptr, void * userData  = nullptr);
	void setPipe1Address( uint8_t* address, EventCallback callback = nullptr, void * userData  = nullptr);
	void setPipe2Address(uint8_t lastByte, EventCallback callback = nullptr, void * userData  = nullptr);
	void setPipe3Address(uint8_t lastByte, EventCallback callback = nullptr, void * userData  = nullptr);
	void setPipe4Address(uint8_t lastByte, EventCallback callback = nullptr, void * userData  = nullptr);
	void setPipe5Address(uint8_t lastByte, EventCallback callback = nullptr, void * userData  = nullptr);
	void setIRQCallback(IRQCallback_t callback){IRQCallback = callback;};
	void sendMessage(EventCallback callback = nullptr, void * userData  = nullptr);
	MODE getMode(){return mode;}
	void IRQHandler();
};


}


#endif /* NIKOLEX_NRF24L01_DEVICE_HPP_ */
