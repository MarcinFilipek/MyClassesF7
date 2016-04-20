/*
 * I2CDriver.cpp
 *
 *  Created on: 26 sty 2016
 *      Author: Marcin
 */

#include "I2CDriver.h"

void I2CDriver::init(){
	m_I2CHandle.Instance = I2C3;
	m_I2CHandle.Init.Timing = I2C_TIMING;
	m_I2CHandle.Init.OwnAddress1 = 0;
	m_I2CHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	m_I2CHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	m_I2CHandle.Init.OwnAddress2 = 0;
	m_I2CHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	m_I2CHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	m_gpioSCL = GPIOH;
	m_gpioSDA = GPIOH;
	m_pinSCL = GPIO_PIN_7;
	m_pinSDA = GPIO_PIN_8;
	initPins();
	HAL_I2C_Init(&m_I2CHandle);
}

void I2CDriver::initPins(){
	GPIO_InitTypeDef gpioInitStruct;

	gpioInitStruct.Pin = m_pinSCL;
	gpioInitStruct.Mode = GPIO_MODE_AF_OD;
	gpioInitStruct.Pull = GPIO_NOPULL;
	gpioInitStruct.Speed = GPIO_SPEED_FAST;
	gpioInitStruct.Alternate = GPIO_AF4_I2C3;
	HAL_GPIO_Init(m_gpioSCL, &gpioInitStruct);

	gpioInitStruct.Pin = m_pinSDA;
	HAL_GPIO_Init(m_gpioSDA, &gpioInitStruct);

	__HAL_RCC_I2C3_CLK_ENABLE();
	__HAL_RCC_I2C3_FORCE_RESET();
	__HAL_RCC_I2C3_RELEASE_RESET();

    HAL_NVIC_SetPriority(I2C3_EV_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);

    HAL_NVIC_SetPriority(I2C3_ER_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);

}

uint8_t I2CDriver::read(uint8_t addr, uint8_t reg){
	uint8_t readVal = 0;
	HAL_I2C_Mem_Read(&m_I2CHandle, addr, (uint16_t)reg, I2C_MEMADD_SIZE_8BIT, &readVal, 1, 1000);
	return readVal;
}

void I2CDriver::write(uint8_t addr, uint8_t reg, uint8_t val){
	HAL_I2C_Mem_Write(&m_I2CHandle, addr, (uint16_t)reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&val, 1, 1000);
}
