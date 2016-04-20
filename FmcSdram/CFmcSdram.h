/*
 * CFmcSdram.h
 *
 *  Created on: 4 sty 2016
 *      Author: marcin
 */

#ifndef CFMCSDRAM_H_
#define CFMCSDRAM_H_

#include "stm32f7xx_hal.h"

class CFmcSdram {
public:
	static const uint32_t SDRAM_BANK_ADDR = 0xC0000000;
	static const uint32_t SDRAM_BANK_SIZE = 0x800000;  /* SDRAM device size in MBytes */
	static const uint32_t REFRESH_COUNT = 0x0603;   /* SDRAM refresh counter (100Mhz SD clock) */
	static const uint32_t SDRAM_TIMEOUT = 0xFFFF;
	static const uint16_t SDRAM_MODEREG_BURST_LENGTH_1 = 0x0000;
	static const uint16_t SDRAM_MODEREG_BURST_LENGTH_2 = 0x0001;
	static const uint16_t SDRAM_MODEREG_BURST_LENGTH_4 = 0x0002;
	static const uint16_t SDRAM_MODEREG_BURST_LENGTH_8 = 0x0004;
	static const uint16_t SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL = 0x0000;
	static const uint16_t SDRAM_MODEREG_BURST_TYPE_INTERLEAVED = 0x0008;
	static const uint16_t SDRAM_MODEREG_CAS_LATENCY_2 = 0x0020;
	static const uint16_t SDRAM_MODEREG_CAS_LATENCY_3 = 0x0030;
	static const uint16_t SDRAM_MODEREG_OPERATING_MODE_STANDARD = 0x0000;
	static const uint16_t SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED = 0x0000;
	static const uint16_t SDRAM_MODEREG_WRITEBURST_MODE_SINGLE = 0x0200;
	CFmcSdram();
	void write32(uint32_t* pBuffer, uint32_t writeAddress, uint32_t bufferSize);
	void read32(uint32_t* pBuffer, uint32_t readAddress, uint32_t bufferSize);
private:
	SDRAM_HandleTypeDef      m_hsdram;
	FMC_SDRAM_TimingTypeDef  m_sdramTiming;
	FMC_SDRAM_CommandTypeDef m_command;
	void pinInit(void);
	void initializationSequence(SDRAM_HandleTypeDef* hsdram, FMC_SDRAM_CommandTypeDef* command);
protected:
};

#endif /* CFMCSDRAM_H_ */
