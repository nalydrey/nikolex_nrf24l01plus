/*
 * nrf24l01_def.hpp
 *
 *  Created on: Jul 4, 2025
 *      Author: Oleksiy
 */

#ifndef NIKOLEX_NRF24L01_DEF_HPP_
#define NIKOLEX_NRF24L01_DEF_HPP_

namespace nikolex::nrf24l01 {

	enum MODE {
		TX_MODE,
		RX_MODE
	};

	enum CRC_LENGTH{
		ONE_BYTE,
		TWO_BYTE
	};

	enum PIPE {
		PIPE_0 = 1 << 0,
		PIPE_1 = 1 << 1,
		PIPE_2 = 1 << 2,
		PIPE_3 = 1 << 3,
		PIPE_4 = 1 << 4,
		PIPE_5 = 1 << 5,
	};

	enum DATA_RATE{
		DATA_RATE_1Mbps,
		DATA_RATE_2Mbps		= 0b00001000,
		DATA_RATE_250_kbps  = 0b00100000,
	};

	enum POWER_AMPLIFIER {
		PA_minus18dBm,
		PA_minus12dBm,
		PA_minus6dBm,
		PA_0dBm
	};

	enum ADDRESS_WIDTH {
		AW_3BYTES = 1,
		AW_4BYTES,
		AW_5BYTES
	};

	enum RETRANSMIT_DELAY {
		RD_250us,
		RD_500us,
		RD_750us,
		RD_1000us,
		RD_1250us,
		RD_1500us,
		RD_1750us,
		RD_2000us,
		RD_2250us,
		RD_2500us,
		RD_2750us,
		RD_3000us,
		RD_3250us,
		RD_3500us,
		RD_3750us,
		RD_4000us,
	};

}


#endif /* NIKOLEX_NRF24L01_DEF_HPP_ */
