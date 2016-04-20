/*
 * CSpiritGpio.cpp
 *
 *  Created on: 18 lis 2015
 *      Author: Marcin
 */

#include "CSpiritGpio.h"

void CSpiritGpio::init(CSpiritInterface* driverSpi){
	m_driverSpi = driverSpi;
}

/*
 * Initializes the SPIRIT GPIOx according to the specified
 * parameters in the pxGpioInitStruct.
 */
void CSpiritGpio::initGpio(SGpioInit* gpioInitStruct){
	  uint8_t tempRegValue = 0x00;
	  tempRegValue = ((uint8_t)(gpioInitStruct->xSpiritGpioMode) | (uint8_t)(gpioInitStruct->xSpiritGpioIO));
	  m_driverSpi->writeRegisters(gpioInitStruct->xSpiritGpioPin, 1, &tempRegValue);
}
