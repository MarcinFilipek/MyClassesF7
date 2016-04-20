/*
 * CLed.h
 *
 *  Created on: 11 cze 2015
 *      Author: Marcin
 */

#ifndef CLED_H_
#define CLED_H_

#include "stm32f7xx_hal.h"

class Led {
public:
	void init(GPIO_TypeDef* gpio, uint16_t pin);
	void setOn(void);
	void setOff(void);
	void setToggle(void);
	bool isOn(void);
private:
	GPIO_TypeDef* m_gpio;
	uint16_t m_pin;
};

#endif /* CLED_H_ */
