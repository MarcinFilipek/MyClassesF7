/*
 * CSpiritInterface.cpp
 *
 *  Created on: 15 lis 2015
 *      Author: marcin
 */

#include "CSpiritInterface.h"

uint16_t CSpiritInterface::POR_TIME = 0x1E00;
CSpiritSpi CSpiritInterface::m_spiDriver;

void CSpiritInterface::initSpiSpirit(){
	CSpiritSpi::SpiStruct spiSpirit_Struct;
	spiSpirit_Struct.Instance = SPI2;
	spiSpirit_Struct.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	spiSpirit_Struct.Direction = SPI_DIRECTION_2LINES;
	spiSpirit_Struct.CLKPhase = SPI_PHASE_1EDGE;
	spiSpirit_Struct.CLKPolarity = SPI_POLARITY_LOW;
	spiSpirit_Struct.sckPin.gpio = GPIOI;
	spiSpirit_Struct.sckPin.pin = GPIO_PIN_1;
	spiSpirit_Struct.misoPin.gpio = GPIOB;
	spiSpirit_Struct.misoPin.pin = GPIO_PIN_14;
	spiSpirit_Struct.mosiPin.gpio = GPIOB;
	spiSpirit_Struct.mosiPin.pin = GPIO_PIN_15;
	m_spiDriver.init(&spiSpirit_Struct, GPIOF, GPIO_PIN_10);

	m_spiritTypes.init(&m_spiDriver);
}
void CSpiritInterface::pinShutdownInit(GPIO_TypeDef* gpio, uint16_t pin){
	m_gpioSDN = gpio;
	m_pinSDN = pin;
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = m_pinSDN;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(m_gpioSDN, &GPIO_InitStruct);
}

void CSpiritInterface::pinInterruptInit(GPIO_TypeDef* gpio, uint16_t pin){
	m_gpioEXTI = gpio;
	m_pinEXTI = pin;
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = m_pinEXTI; //GPIO_PIN_7
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    HAL_GPIO_Init(m_gpioEXTI, &GPIO_InitStruct);
}

void CSpiritInterface::interruptCmd(bool wlacz){
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0x04, 0x04);
	if(wlacz){
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	} else {
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	}
}

void CSpiritInterface::enterShutdown(){
	HAL_GPIO_WritePin(m_gpioSDN, m_pinSDN, GPIO_PIN_SET);
}

void CSpiritInterface::exitShutdown(){
	HAL_GPIO_WritePin(m_gpioSDN, m_pinSDN, GPIO_PIN_RESET);
	  /* Delay to allow the circuit POR, about 700 us */
	for (volatile uint32_t Index = 0; Index < POR_TIME; Index++);
}

void CSpiritInterface::readRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer){
	int16_t tempstatus;
	tempstatus = m_spiDriver.readRegisters(cRegAddress, cNbBytes, pcBuffer);
	CSpiritTypes::SpiritStatus* wskStatus = (CSpiritTypes::SpiritStatus*)&tempstatus;
	m_spiritTypes.getSpiritStatus() = *wskStatus;
}

void CSpiritInterface::writeRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer){
	int16_t tempstatus;
	tempstatus = m_spiDriver.writeRegisters(cRegAddress, cNbBytes, pcBuffer);
	CSpiritTypes::SpiritStatus* wskStatus = (CSpiritTypes::SpiritStatus*)&tempstatus;
	m_spiritTypes.getSpiritStatus() = *wskStatus;
}

void CSpiritInterface::commandStrobes(uint8_t commandCode){
	int16_t tempstatus;
	tempstatus = m_spiDriver.commandStrobes(commandCode);
	CSpiritTypes::SpiritStatus* wskStatus = (CSpiritTypes::SpiritStatus*)&tempstatus;
	m_spiritTypes.getSpiritStatus() = *wskStatus;
}

void CSpiritInterface::writeFifo(uint8_t cNbBytes, uint8_t* pcBuffer){
	int16_t tempstatus;
	tempstatus = m_spiDriver.writeFifo(cNbBytes, pcBuffer);
	CSpiritTypes::SpiritStatus* wskStatus = (CSpiritTypes::SpiritStatus*)&tempstatus;
	m_spiritTypes.getSpiritStatus() = *wskStatus;
}

void CSpiritInterface::readFifo(uint8_t cNbBytes, uint8_t* pcBuffer){
	int16_t tempstatus;
	tempstatus = m_spiDriver.readFifo(cNbBytes, pcBuffer);
	CSpiritTypes::SpiritStatus* wskStatus = (CSpiritTypes::SpiritStatus*)&tempstatus;
	m_spiritTypes.getSpiritStatus() = *wskStatus;
}
