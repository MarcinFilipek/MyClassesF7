/*
 * I2CDriver.h
 *
 *  Created on: 26 sty 2016
 *      Author: Marcin
 */

#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_

#include "stm32f7xx_hal.h"

class I2CDriver {
public:
	void init(void);
	void initPins(void);
	uint8_t read(uint8_t addr, uint8_t reg);
	void write(uint8_t addr, uint8_t reg, uint8_t val);
	I2C_HandleTypeDef* getI2CHandle() { return &m_I2CHandle; }
private:
	/* I2C TIMING Register define when I2C clock source is SYSCLK */
	/* I2C TIMING is calculated from APB1 source clock = 50 MHz */
	/* Due to the big MOFSET capacity for adapting the camera level the rising time is very large (>1us) */
	/* 0x40912732 takes in account the big rising and aims a clock of 100khz */
	/* this value might be adapted when next Rev Birdie board is available */
	static const uint32_t I2C_TIMING = 0x40912732;

	GPIO_TypeDef* m_gpioSCL;
	uint16_t m_pinSCL;
	GPIO_TypeDef* m_gpioSDA;
	uint16_t m_pinSDA;
	I2C_HandleTypeDef m_I2CHandle;

protected:
};

#endif /* I2CDRIVER_H_ */
