/*
 * CSpiritSpi.cpp
 *
 *  Created on: 11 lis 2015
 *      Author: marcin
 */

#include "CSpiritSpi.h"

uint32_t CSpiritSpi::m_spiTimeout = 1000;
void CSpiritSpi::init(SpiStruct* pinsStruct, GPIO_TypeDef* gpioCS, uint16_t pinCS) {
	/* SPI Config */
	m_spiStruct = pinsStruct;
	m_hspi.Instance = m_spiStruct->Instance;
	/* SPI baudrate is set to 8 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 32/4 = 8 MHz)
	 to verify these constraints:
	 - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
	 Since the provided driver doesn't use read capability from LCD, only constraint
	 on write baudrate is considered.
	 - SD card SPI interface max baudrate is 25MHz for write/read
	 - PCLK2 max frequency is 32 MHz
	 */
	m_hspi.Init.BaudRatePrescaler = m_spiStruct->BaudRatePrescaler;	//SPI_BAUDRATEPRESCALER_4
	m_hspi.Init.Direction = m_spiStruct->Direction;					//SPI_DIRECTION_2LINES
	m_hspi.Init.CLKPhase = m_spiStruct->CLKPhase;					//SPI_PHASE_1EDGE
	m_hspi.Init.CLKPolarity = m_spiStruct->CLKPolarity;				//SPI_POLARITY_LOW
	m_hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	m_hspi.Init.CRCPolynomial = 7;
	m_hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	m_hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	m_hspi.Init.NSS = SPI_NSS_SOFT;
	m_hspi.Init.TIMode = SPI_TIMODE_DISABLED;
	m_hspi.Init.Mode = SPI_MODE_MASTER;
	/* pins init */
	m_gpioCS = gpioCS;
	m_pinCS = pinCS;
	pinsInit(&m_hspi);
	HAL_SPI_Init(&m_hspi);
}

void CSpiritSpi::pinsInit(SPI_HandleTypeDef *hspi) {
	GPIO_InitTypeDef GPIO_InitStruct;

    /* Configure SPI pin: CS */
    GPIO_InitStruct.Pin = m_pinCS;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(m_gpioCS, &GPIO_InitStruct);

	/*** Configure the SPI peripheral ***/

	/*** Configure the GPIOs ***/
	uint32_t alternateFun;
	if(hspi->Instance == SPI1){
		alternateFun = GPIO_AF5_SPI1;
	} else if(hspi->Instance == SPI2){
		alternateFun = GPIO_AF5_SPI2;
	}
	/* Configure SPI SCK */
	GPIO_InitStruct.Pin = m_spiStruct->sckPin.pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = alternateFun;
	HAL_GPIO_Init(m_spiStruct->sckPin.gpio, &GPIO_InitStruct);

	//MOSI
	GPIO_InitStruct.Pin = m_spiStruct->mosiPin.pin;
	GPIO_InitStruct.Alternate = alternateFun;
	HAL_GPIO_Init(m_spiStruct->mosiPin.gpio, &GPIO_InitStruct);

	//MISO
	GPIO_InitStruct.Pin = m_spiStruct->misoPin.pin;
	GPIO_InitStruct.Alternate = alternateFun;
	HAL_GPIO_Init(m_spiStruct->misoPin.gpio, &GPIO_InitStruct);

	/* Enable SPI clock */
	if(hspi->Instance == SPI1){
		__SPI1_CLK_ENABLE();
	} else if(hspi->Instance == SPI2){
		__SPI2_CLK_ENABLE();
	}
}

void CSpiritSpi::csLow(){
	HAL_GPIO_WritePin(m_gpioCS, m_pinCS, GPIO_PIN_RESET);
}

void CSpiritSpi::csHigh(){
	HAL_GPIO_WritePin(m_gpioCS, m_pinCS, GPIO_PIN_SET);
}

void CSpiritSpi::error(void) {
	/* De-initialize the SPI communication BUS */
	HAL_SPI_DeInit(&m_hspi);
	while(1){}
//	/* Re-Initiaize the SPI communication BUS */
//	init(m_spiStruct);
}

void CSpiritSpi::wait() {
	while (((m_hspi.Instance)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0
			|| ((m_hspi.Instance)->SR & SPI_SR_BSY))
		;
}

void CSpiritSpi::write(uint8_t value){
	  HAL_StatusTypeDef status = HAL_OK;
	  while (__HAL_SPI_GET_FLAG(&m_hspi, SPI_FLAG_TXE) == RESET);
	  status = HAL_SPI_Transmit(&m_hspi, (uint8_t*) &value, 1, m_spiTimeout);
	  /* Check the communication status */
	  if (status != HAL_OK)
	  {
		  /* Execute user timeout callback */
		  error();
	  }
}
uint16_t CSpiritSpi::readRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer){
	  uint16_t tmpstatus = 0x00;
	  uint8_t aHeader[2] = {0};
	  uint8_t dummy = 0xFF;
	  HAL_StatusTypeDef statusSPI;
	  /* Built the aHeader bytes */
	  aHeader[0] = READ_HEADER;
	  aHeader[1] = cRegAddress;
	  __disable_irq();
	  csLow();
	  wait();
	  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
	  statusSPI = HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *)&aHeader[0], (uint8_t *)&(tmpstatus), 1, m_spiTimeout);
	  tmpstatus = tmpstatus << 8;
	  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
	  statusSPI = HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *)&aHeader[1], (uint8_t *)&tmpstatus, 1, m_spiTimeout);
	  for (int index = 0; index < cNbBytes; index++)
	  {
	    HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *)&dummy, (uint8_t *)&(pcBuffer)[index], 1, m_spiTimeout);
	  }
	  /* To be sure to don't rise the Chip Select before the end of last sending */
	  while (__HAL_SPI_GET_FLAG(&m_hspi, SPI_FLAG_TXE) == RESET);
	  csHigh();
	  __enable_irq();
	  return tmpstatus;
}

uint16_t CSpiritSpi::writeRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer){
	  uint16_t tmpstatus = 0x0000;
	  uint8_t aHeader[2] = {0};
	  HAL_StatusTypeDef statusSPI;
	  /* Built the aHeader bytes */
	  aHeader[0] = WRITE_HEADER;
	  aHeader[1] = cRegAddress;
	  __disable_irq();
	  csLow();
	  wait();
	  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
	  statusSPI = HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *)&aHeader[0], (uint8_t *)&(tmpstatus), 1, m_spiTimeout);
	  tmpstatus = tmpstatus << 8;
	  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
	  statusSPI = HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *)&aHeader[1], (uint8_t *)&tmpstatus, 1, m_spiTimeout);
	  /* Writes the registers according to the number of bytes */
	  for (int index = 0; index < cNbBytes; index++)
	  {
	    write(pcBuffer[index]);
	  }
	  /* To be sure to don't rise the Chip Select before the end of last sending */
	  while (__HAL_SPI_GET_FLAG(&m_hspi, SPI_FLAG_TXE) == RESET);
	  csHigh();
	  __enable_irq();
	  return tmpstatus;
}

uint16_t CSpiritSpi::commandStrobes(uint8_t commandCode){
	  uint16_t tmpstatus = 0x0000;
	  uint8_t aHeader[2] = {0};
	  /* Built the aHeader bytes */
	  aHeader[0] = COMMAND_HEADER;
	  aHeader[1] = commandCode;
	  __disable_irq();
	  csLow();
	  wait();
	  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
	  HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *)&aHeader[0], (uint8_t *)&tmpstatus, 1, m_spiTimeout);
	  tmpstatus = tmpstatus<<8;
	  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
	  HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *)&aHeader[1], (uint8_t *)&tmpstatus, 1, m_spiTimeout);
	  /* To be sure to don't rise the Chip Select before the end of last sending */
	  while (__HAL_SPI_GET_FLAG(&m_hspi, SPI_FLAG_TXE) == RESET);
	  csHigh();
	  __enable_irq();
	  return tmpstatus;

}

/*
 * Write data into TX FIFO
 */
uint16_t CSpiritSpi::writeFifo(uint8_t cNbBytes, uint8_t* pcBuffer) {
	uint16_t tmpstatus = 0x0000;
	uint8_t aHeader[2] = { 0 };
	/* Built the aHeader bytes */
	aHeader[0] = WRITE_HEADER;
	aHeader[1] = LINEAR_FIFO_ADDRESS;
	__disable_irq();
	/* Put the SPI chip select low to start the transaction */
	csLow();
	wait();
	/* Write the aHeader bytes and read the SPIRIT1 status bytes */
	HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *) &aHeader[0], (uint8_t *) &tmpstatus, 1, m_spiTimeout);
	tmpstatus = tmpstatus << 8;
	/* Write the aHeader bytes and read the SPIRIT1 status bytes */
	HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *) &aHeader[1], (uint8_t *) &tmpstatus, 1, m_spiTimeout);
	/* Writes the registers according to the number of bytes */
	for (int index = 0; index < cNbBytes; index++) {
		write(pcBuffer[index]);
	}
	/* To be sure to don't rise the Chip Select before the end of last sending */
	while (__HAL_SPI_GET_FLAG(&m_hspi, SPI_FLAG_TXE) == RESET);
	/* Put the SPI chip select high to end the transaction */
	csHigh();
	__enable_irq();
	return tmpstatus;
}


uint16_t CSpiritSpi::readFifo(uint8_t cNbBytes, uint8_t* pcBuffer)
{
  uint16_t tmpstatus = 0x0000;
  uint8_t aHeader[2];
  uint8_t dummy=0xFF;
  /* Built the aHeader bytes */
  aHeader[0]=READ_HEADER;
  aHeader[1]=LINEAR_FIFO_ADDRESS;
  __disable_irq();
  /* Put the SPI chip select low to start the transaction */
  csLow();
  wait();
  /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *)&aHeader[0], (uint8_t *)&tmpstatus, 1, m_spiTimeout);
  tmpstatus = tmpstatus<<8;
    /* Write the aHeader bytes and read the SPIRIT1 status bytes */
  HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *)&aHeader[1], (uint8_t *)&tmpstatus, 1, m_spiTimeout);
  for (int index = 0; index < cNbBytes; index++)
  {
    HAL_SPI_TransmitReceive(&m_hspi, (uint8_t *)&dummy, (uint8_t *)&pcBuffer[index], 1, m_spiTimeout);
  }
  /* To be sure to don't rise the Chip Select before the end of last sending */
  while(__HAL_SPI_GET_FLAG(&m_hspi, SPI_FLAG_TXE) == RESET);
  /* Put the SPI chip select high to end the transaction */
  csHigh();
  __enable_irq();
  return tmpstatus;
}
