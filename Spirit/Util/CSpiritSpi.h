/*
 * CSpiritSpi.h
 *
 *  Created on: 11 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITSPI_H_
#define CSPIRITSPI_H_

#include "stm32f7xx_hal.h"

/* SPIRIT1_Spi_config_Headers */
#define HEADER_WRITE_MASK     0x00                                /*!< Write mask for header byte*/
#define HEADER_READ_MASK      0x01                                /*!< Read mask for header byte*/
#define HEADER_ADDRESS_MASK   0x00                                /*!< Address mask for header byte*/
#define HEADER_COMMAND_MASK   0x80                                /*!< Command mask for header byte*/

#define LINEAR_FIFO_ADDRESS 0xFF                                  /*!< Linear FIFO address*/

#define BUILT_HEADER(add_comm, w_r) (add_comm | w_r)                             /*!< macro to build the header byte*/
#define WRITE_HEADER        BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_WRITE_MASK) /*!< macro to build the write
                                                                                                         header byte*/
#define READ_HEADER         BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_READ_MASK)  /*!< macro to build the read
                                                                                                         header byte*/
#define COMMAND_HEADER      BUILT_HEADER(HEADER_COMMAND_MASK, HEADER_WRITE_MASK) /*!< macro to build the command
                                                                                                         header byte*/

class CSpiritSpi {
public:
	struct SPin{
		GPIO_TypeDef* gpio;
		uint16_t pin;
	};
	struct SpiStruct{
		SPin sckPin;
		SPin mosiPin;
		SPin misoPin;
		SPI_TypeDef* Instance;
		uint32_t BaudRatePrescaler;
		uint32_t Direction;
		uint32_t CLKPhase;
		uint32_t CLKPolarity;
	};
	void init(SpiStruct* pinsStruct, GPIO_TypeDef* gpioCS, uint16_t pinCS);
	uint16_t readRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
	void write(uint8_t value);
	uint16_t writeRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
	uint16_t commandStrobes(uint8_t commandCode);
	uint16_t writeFifo(uint8_t cNbBytes, uint8_t* pcBuffer);
	uint16_t readFifo(uint8_t cNbBytes, uint8_t* pcBuffer);
private:
	static uint32_t m_spiTimeout; /*<! Value of Timeout when SPI communication fails */
	GPIO_TypeDef* m_gpioCS;
	uint16_t m_pinCS;
	void pinsInit(SPI_HandleTypeDef *hspi);
	void csLow(void);
	void csHigh(void);
	void error(void);
	void wait(void);
	SPI_HandleTypeDef m_hspi;
	SpiStruct* m_spiStruct;
};

#endif /* CSPIRITSPI_H_ */
