/*
 * nrf24l01_types.hpp
 *
 *  Created on: Jul 29, 2025
 *      Author: Oleksiy
 */

#ifndef NIKOLEX_NRF24L01_TYPES_HPP_
#define NIKOLEX_NRF24L01_TYPES_HPP_

#include <stdint.h>


namespace nikolex::nrf24l01 {

	typedef struct Event{
		uint8_t status;
		uint8_t registerAddress;
		uint8_t * payload;
		uint8_t payloadSize;
		void * userData;
	} Event;


	using EventCallback = void (*)(Event&);
	using Delay_us = void(*)(uint32_t);


	typedef struct{
		uint8_t registerAddress;
		uint8_t payloadSize;
		void * userData;
		EventCallback  userCallback;
	} StackItem;


	typedef struct{
		uint8_t mask;
		uint8_t newValue;
	} ModifyData;


	class Device;


	typedef struct {
		bool dataReady;
		bool dataSent;
		bool maxRetransmits;
		uint8_t pipeNumber;
		bool txFifoFull;

		bool rxFull;
		bool rxEmpty;
		bool txEmpty;
		bool txFull;

		Device * device;
	} IRQEvent;


	using IRQCallback_t = void(*)(IRQEvent);
}



#endif /* NIKOLEX_NRF24L01_TYPES_HPP_ */
