/*
 * CLed.cpp
 *
 *  Created on: 11 cze 2015
 *      Author: Marcin
 */

#include "CLed.h"

void Led::init(GPIO_TypeDef* gpio, uint16_t pin){
	m_gpio = gpio;
	m_pin = pin;

	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Speed = GPIO_SPEED_LOW;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Pin = m_pin;
	HAL_GPIO_Init(m_gpio, &GPIO_Init);
}

void Led::setOn(){
	HAL_GPIO_WritePin(m_gpio, m_pin, GPIO_PIN_SET);
}

void Led::setOff(){
	HAL_GPIO_WritePin(m_gpio, m_pin, GPIO_PIN_RESET);
}

void Led::setToggle(){
	HAL_GPIO_TogglePin(m_gpio, m_pin);
}

bool Led::isOn(){
	return HAL_GPIO_ReadPin(m_gpio, m_pin) == GPIO_PIN_SET;
}
