/*
 * FT5336.cpp
 *
 *  Created on: 26 sty 2016
 *      Author: Marcin
 */

#include "FT5336.h"
#include "reg.h"

void FT5336::init(uint16_t address){
	HAL_Delay(200);
	m_i2cDriver.init();
	m_deviceAddr = address;
}

uint16_t FT5336::readID() {
	volatile uint8_t ucReadId = 0;
	uint8_t nbReadAttempts = 0;
	uint8_t bFoundDevice = 0; /* Device not found by default */
	/* At maximum 4 attempts to read ID : exit at first finding of the searched device ID */
	for (nbReadAttempts = 0; ((nbReadAttempts < 3) && !(bFoundDevice)); nbReadAttempts++) {
		/* Read register FT5336_CHIP_ID_REG as DeviceID detection */
		ucReadId = m_i2cDriver.read(m_deviceAddr, FT5336_CHIP_ID_REG);
		/* Found the searched device ID ? */
		if (ucReadId == FT5336_ID_VALUE) {
			/* Set device as found */
			bFoundDevice = 1;
		}
	}
	/* Return the device ID value */
	return ucReadId;
}

void FT5336::start(){
	disableIT();
}

void FT5336::disableIT(){
	uint8_t regVal = 0;
	regVal = (FT5336_G_MODE_INTERRUPT_POLLING & (FT5336_G_MODE_INTERRUPT_MASK >> FT5336_G_MODE_INTERRUPT_SHIFT)) << FT5336_G_MODE_INTERRUPT_SHIFT;
	m_i2cDriver.write(m_deviceAddr, FT5336_GMODE_REG, regVal);
}

void FT5336::enableIT() {
	uint8_t regValue = 0;
	regValue = (FT5336_G_MODE_INTERRUPT_TRIGGER & (FT5336_G_MODE_INTERRUPT_MASK >> FT5336_G_MODE_INTERRUPT_SHIFT)) << FT5336_G_MODE_INTERRUPT_SHIFT;
	/* Set interrupt trigger mode in FT5336_GMODE_REG */
	m_i2cDriver.write(m_deviceAddr, FT5336_GMODE_REG, regValue);
}

uint8_t FT5336::detectTouch() {
	volatile uint8_t nbTouch = 0;
	/* Read register FT5336_TD_STAT_REG to check number of touches detection */
	nbTouch = m_i2cDriver.read(m_deviceAddr, FT5336_TD_STAT_REG);
	nbTouch &= FT5336_TD_STAT_MASK;
	if (nbTouch > m_maxDetectableTouch) {
		/* If invalid number of touch detected, set it to zero */
		nbTouch = 0;
	}
	/* Update ft5336 driver internal global : current number of active touches */
	m_ft5336TypeDef.currActiveTouchNb = nbTouch;
	/* Reset current active touch index on which to work on */
	m_ft5336TypeDef.currActiveTouchIdx = 0;
	return (nbTouch);
}


void FT5336::getXY(uint16_t* x, uint16_t* y) {
	volatile uint8_t ucReadData = 0;
	static uint16_t coord;
	uint8_t regAddressXLow = 0;
	uint8_t regAddressXHigh = 0;
	uint8_t regAddressYLow = 0;
	uint8_t regAddressYHigh = 0;

	if (m_ft5336TypeDef.currActiveTouchIdx
			< m_ft5336TypeDef.currActiveTouchNb) {
		switch (m_ft5336TypeDef.currActiveTouchIdx) {
		case 0:
			regAddressXLow = FT5336_P1_XL_REG;
			regAddressXHigh = FT5336_P1_XH_REG;
			regAddressYLow = FT5336_P1_YL_REG;
			regAddressYHigh = FT5336_P1_YH_REG;
			break;

		case 1:
			regAddressXLow = FT5336_P2_XL_REG;
			regAddressXHigh = FT5336_P2_XH_REG;
			regAddressYLow = FT5336_P2_YL_REG;
			regAddressYHigh = FT5336_P2_YH_REG;
			break;

		case 2:
			regAddressXLow = FT5336_P3_XL_REG;
			regAddressXHigh = FT5336_P3_XH_REG;
			regAddressYLow = FT5336_P3_YL_REG;
			regAddressYHigh = FT5336_P3_YH_REG;
			break;

		case 3:
			regAddressXLow = FT5336_P4_XL_REG;
			regAddressXHigh = FT5336_P4_XH_REG;
			regAddressYLow = FT5336_P4_YL_REG;
			regAddressYHigh = FT5336_P4_YH_REG;
			break;

		case 4:
			regAddressXLow = FT5336_P5_XL_REG;
			regAddressXHigh = FT5336_P5_XH_REG;
			regAddressYLow = FT5336_P5_YL_REG;
			regAddressYHigh = FT5336_P5_YH_REG;
			break;

		case 5:
			regAddressXLow = FT5336_P6_XL_REG;
			regAddressXHigh = FT5336_P6_XH_REG;
			regAddressYLow = FT5336_P6_YL_REG;
			regAddressYHigh = FT5336_P6_YH_REG;
			break;

		case 6:
			regAddressXLow = FT5336_P7_XL_REG;
			regAddressXHigh = FT5336_P7_XH_REG;
			regAddressYLow = FT5336_P7_YL_REG;
			regAddressYHigh = FT5336_P7_YH_REG;
			break;

		case 7:
			regAddressXLow = FT5336_P8_XL_REG;
			regAddressXHigh = FT5336_P8_XH_REG;
			regAddressYLow = FT5336_P8_YL_REG;
			regAddressYHigh = FT5336_P8_YH_REG;
			break;

		case 8:
			regAddressXLow = FT5336_P9_XL_REG;
			regAddressXHigh = FT5336_P9_XH_REG;
			regAddressYLow = FT5336_P9_YL_REG;
			regAddressYHigh = FT5336_P9_YH_REG;
			break;

		case 9:
			regAddressXLow = FT5336_P10_XL_REG;
			regAddressXHigh = FT5336_P10_XH_REG;
			regAddressYLow = FT5336_P10_YL_REG;
			regAddressYHigh = FT5336_P10_YH_REG;
			break;

		default:
			break;

		} /* end switch(ft5336_handle.currActiveTouchIdx) */

		/* Read low part of X position */
		ucReadData = m_i2cDriver.read(m_deviceAddr, regAddressXLow);
		coord = (ucReadData & FT5336_TOUCH_POS_LSB_MASK)
				>> FT5336_TOUCH_POS_LSB_SHIFT;

		/* Read high part of X position */
		ucReadData = m_i2cDriver.read(m_deviceAddr, regAddressXHigh);
		coord |= ((ucReadData & FT5336_TOUCH_POS_MSB_MASK)
				>> FT5336_TOUCH_POS_MSB_SHIFT) << 8;

		/* Send back ready X position to caller */
		*x = coord;

		/* Read low part of Y position */
		ucReadData = m_i2cDriver.read(m_deviceAddr, regAddressYLow);
		coord = (ucReadData & FT5336_TOUCH_POS_LSB_MASK)
				>> FT5336_TOUCH_POS_LSB_SHIFT;

		/* Read high part of Y position */
		ucReadData = m_i2cDriver.read(m_deviceAddr, regAddressYHigh);
		coord |= ((ucReadData & FT5336_TOUCH_POS_MSB_MASK)
				>> FT5336_TOUCH_POS_MSB_SHIFT) << 8;

		/* Send back ready Y position to caller */
		*y = coord;

		m_ft5336TypeDef.currActiveTouchIdx++; /* next call will work on next touch */

	} /* of if(ft5336_handle.currActiveTouchIdx < ft5336_handle.currActiveTouchNb) */
}

void FT5336::getGestureID(uint32_t * pGestureId)
{
  volatile uint8_t ucReadData = 0;
  ucReadData = m_i2cDriver.read(m_deviceAddr, FT5336_GEST_ID_REG);
  *pGestureId = ucReadData;
}


void FT5336::getTouchInfo(uint32_t touchIdx, uint32_t * pWeight,
		uint32_t * pArea, uint32_t * pEvent) {
	volatile uint8_t ucReadData = 0;
	uint8_t regAddressXHigh = 0;
	uint8_t regAddressPWeight = 0;
	uint8_t regAddressPMisc = 0;

	if (touchIdx < m_ft5336TypeDef.currActiveTouchNb) {
		switch (touchIdx) {
		case 0:
			regAddressXHigh = FT5336_P1_XH_REG;
			regAddressPWeight = FT5336_P1_WEIGHT_REG;
			regAddressPMisc = FT5336_P1_MISC_REG;
			break;

		case 1:
			regAddressXHigh = FT5336_P2_XH_REG;
			regAddressPWeight = FT5336_P2_WEIGHT_REG;
			regAddressPMisc = FT5336_P2_MISC_REG;
			break;

		case 2:
			regAddressXHigh = FT5336_P3_XH_REG;
			regAddressPWeight = FT5336_P3_WEIGHT_REG;
			regAddressPMisc = FT5336_P3_MISC_REG;
			break;

		case 3:
			regAddressXHigh = FT5336_P4_XH_REG;
			regAddressPWeight = FT5336_P4_WEIGHT_REG;
			regAddressPMisc = FT5336_P4_MISC_REG;
			break;

		case 4:
			regAddressXHigh = FT5336_P5_XH_REG;
			regAddressPWeight = FT5336_P5_WEIGHT_REG;
			regAddressPMisc = FT5336_P5_MISC_REG;
			break;

		case 5:
			regAddressXHigh = FT5336_P6_XH_REG;
			regAddressPWeight = FT5336_P6_WEIGHT_REG;
			regAddressPMisc = FT5336_P6_MISC_REG;
			break;

		case 6:
			regAddressXHigh = FT5336_P7_XH_REG;
			regAddressPWeight = FT5336_P7_WEIGHT_REG;
			regAddressPMisc = FT5336_P7_MISC_REG;
			break;

		case 7:
			regAddressXHigh = FT5336_P8_XH_REG;
			regAddressPWeight = FT5336_P8_WEIGHT_REG;
			regAddressPMisc = FT5336_P8_MISC_REG;
			break;

		case 8:
			regAddressXHigh = FT5336_P9_XH_REG;
			regAddressPWeight = FT5336_P9_WEIGHT_REG;
			regAddressPMisc = FT5336_P9_MISC_REG;
			break;

		case 9:
			regAddressXHigh = FT5336_P10_XH_REG;
			regAddressPWeight = FT5336_P10_WEIGHT_REG;
			regAddressPMisc = FT5336_P10_MISC_REG;
			break;

		default:
			break;

		} /* end switch(touchIdx) */

		/* Read Event Id of touch index */
		ucReadData = m_i2cDriver.read(m_deviceAddr, regAddressXHigh);
		*pEvent = (ucReadData & FT5336_TOUCH_EVT_FLAG_MASK) >> FT5336_TOUCH_EVT_FLAG_SHIFT;

		/* Read weight of touch index */
		ucReadData = m_i2cDriver.read(m_deviceAddr, regAddressPWeight);
		*pWeight = (ucReadData & FT5336_TOUCH_WEIGHT_MASK) >> FT5336_TOUCH_WEIGHT_SHIFT;

		/* Read area of touch index */
		ucReadData = m_i2cDriver.read(m_deviceAddr, regAddressPMisc);
		*pArea = (ucReadData & FT5336_TOUCH_AREA_MASK) >> FT5336_TOUCH_AREA_SHIFT;

	} /* of if(touchIdx < ft5336_handle.currActiveTouchNb) */
}
