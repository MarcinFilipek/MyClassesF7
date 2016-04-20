/*
 * FT5336.h
 *
 *  Created on: 26 sty 2016
 *      Author: Marcin
 */

#ifndef FT5336_H_
#define FT5336_H_

#include "../I2C/I2CDriver.h"

class FT5336 {
public:
	typedef struct {
		/* field holding the current number of simultaneous active touches */
		uint8_t currActiveTouchNb;
		/* field holding the touch index currently managed */
		uint8_t currActiveTouchIdx;
	} FT5336_TypeDef;
	static const uint8_t m_maxDetectableTouch = 5;

	void init(uint16_t address);
	uint16_t readID(void);
	void start(void);
	void enableIT(void);
	uint8_t detectTouch(void);
	void getXY(uint16_t* x, uint16_t *y);
	void getGestureID(uint32_t* gestureId);
	void getTouchInfo(uint32_t touchIdx, uint32_t* pWeight, uint32_t* pArea,
			uint32_t* pEvent);
private:
	uint16_t m_deviceAddr;
	I2CDriver m_i2cDriver;
	FT5336_TypeDef m_ft5336TypeDef;
	void disableIT(void);
protected:
};

#endif /* FT5336_H_ */
