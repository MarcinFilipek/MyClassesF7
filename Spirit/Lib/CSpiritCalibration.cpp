/*
 * CSpiritCalibration.cpp
 *
 *  Created on: 19 lis 2015
 *      Author: marcin
 */

#include "CSpiritCalibration.h"

void CSpiritCalibration::init(CSpiritInterface* driverSpi){
	m_driverSpi = driverSpi;
}

/* Enables or Disables the RCO calibration. */
void CSpiritCalibration::setRco(bool wlacz) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
	/* Build new value for the register */
	if (wlacz) {
		tempRegValue |= PROTOCOL2_RCO_CALIBRATION_MASK;
	} else {
		tempRegValue &= ~PROTOCOL2_RCO_CALIBRATION_MASK;
	}
	/* Writes register to enable or disable the RCO calibration */
	m_driverSpi->writeRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
}

/* nables or Disables the VCO calibration. */
void CSpiritCalibration::setVco(bool wlacz) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
	/* Build new value for the register */
	if (wlacz)
		tempRegValue |= PROTOCOL2_VCO_CALIBRATION_MASK;
	else
		tempRegValue &= ~PROTOCOL2_VCO_CALIBRATION_MASK;
	/* Writes register to enable or disable the VCO calibration */
	m_driverSpi->writeRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
}

/* Sets the RCO calibration words. */
void CSpiritCalibration::setRcoCalWords(uint8_t cRwt, uint8_t cRfb) {
	uint8_t tempRegValue[2];
	/* Build the value of RWT and the MSbits of the RFB word */
	tempRegValue[0] = (cRwt << 4) | (cRfb >> 1);
	/* Reads the register value to update the LSbit of RFB */
	m_driverSpi->readRegisters(RCO_VCO_CALIBR_IN1_BASE, 1, &tempRegValue[1]);
	/* Build new value for the register */
	tempRegValue[1] = (tempRegValue[1] & 0x7F) | (cRfb << 7);
	/* Writes the new value for RCO calibration words */
	m_driverSpi->writeRegisters(RCO_VCO_CALIBR_IN2_BASE, 2, tempRegValue);
}

/* Returns the RCO calibration words. */
void CSpiritCalibration::getRcoCalWords(uint8_t* pcRwt, uint8_t* pcRfb) {
	uint8_t tempRegValue[2];
	/* Reads the registers values */
	m_driverSpi->readRegisters(RCO_VCO_CALIBR_OUT1_BASE, 2, tempRegValue);
	/* Build the RWT value */
	(*pcRwt) = tempRegValue[0] >> 4;
	/* Build the RFB value */
	(*pcRfb) = (tempRegValue[0] & 0x0F) << 1 | (tempRegValue[1] >> 7);
}

/* Returns the VCO calibration data from internal VCO calibrator. */
uint8_t CSpiritCalibration::getVcoCalData(void) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(RCO_VCO_CALIBR_OUT0_BASE, 1, &tempRegValue);
	/* Build and returns the VCO calibration data value */
	return (tempRegValue & 0x7F);
}

/* Sets the VCO calibration data to be used in TX mode. */
void CSpiritCalibration::setVcoCalDataTx(uint8_t cVcoCalData) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(RCO_VCO_CALIBR_IN1_BASE, 1, &tempRegValue);
	/* Build the value to be written */
	tempRegValue &= 0x80;
	tempRegValue |= cVcoCalData;
	/* Writes the new value of calibration data in TX */
	m_driverSpi->writeRegisters(RCO_VCO_CALIBR_IN1_BASE, 1, &tempRegValue);
}

/* Returns the actual VCO calibration data used in TX mode. */
uint8_t CSpiritCalibration::getVcoCalDataTx(void)
{
  uint8_t tempRegValue;
  /* Reads the register containing the calibration data word used in TX mode */
  m_driverSpi->readRegisters(RCO_VCO_CALIBR_IN1_BASE, 1, &tempRegValue);
  /* Mask the VCO_CALIBR_TX field and returns the value */
  return (tempRegValue & 0x7F);
}

/* Sets the VCO calibration data to be used in RX mode. */
void CSpiritCalibration::setVcoCalDataRx(uint8_t cVcoCalData) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(RCO_VCO_CALIBR_IN0_BASE, 1, &tempRegValue);
	/* Build the value to be written */
	tempRegValue &= 0x80;
	tempRegValue |= cVcoCalData;
	/* Writes the new value of calibration data in RX */
	m_driverSpi->writeRegisters(RCO_VCO_CALIBR_IN0_BASE, 1, &tempRegValue);
}

/* Returns the actual VCO calibration data used in RX mode. */
uint8_t CSpiritCalibration::getVcoCalDataRx(void)
{
  uint8_t tempRegValue;
  /* Reads the register containing the calibration data word used in TX mode */
  m_driverSpi->readRegisters(RCO_VCO_CALIBR_IN0_BASE, 1, &tempRegValue);
  /* Mask the VCO_CALIBR_RX field and returns the value */
  return (tempRegValue & 0x7F);
}

/* Sets the VCO calibration window. */
void CSpiritCalibration::setVcoWindow(VcoWin xRefWord) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
	/* Build the values to be written */
	tempRegValue &= 0xFC;
	tempRegValue |= xRefWord;
	/* Writes the new value of VCO calibration window */
	m_driverSpi->writeRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
}

/* Returns the VCO calibration window. */
CSpiritCalibration::VcoWin CSpiritCalibration::getVcoWindow(void) {
	uint8_t tempRegValue1, tempRegValue2;
	VcoWin refWord;
	/* Reads the register containing the REFWORD value */
	m_driverSpi->readRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue1);
	/* Reads the Xtal configuration */
	m_driverSpi->readRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue2);
	/* Mask the REFWORD field */
	tempRegValue1 &= 0x03;
	/* Mask the 24_26_MHz_SELECT field */
	tempRegValue2 = ((tempRegValue2 & 0x40) >> 6);
	/* In case of 26 MHz crystal */
	if (tempRegValue2) {
		switch (tempRegValue1) {
		case 0:
			refWord = CALIB_TIME_6_77_US_26MHZ;
			break;
		case 1:
			refWord = CALIB_TIME_13_54_US_26MHZ;
			break;
		case 2:
			refWord = CALIB_TIME_27_08_US_26MHZ;
			break;
		case 3:
			refWord = CALIB_TIME_54_15_US_26MHZ;
			break;
		}
	}

	/* In case of 24 MHz crystal */
	else {
		switch (tempRegValue1) {
		case 0:
			refWord = CALIB_TIME_7_33_US_24MHZ;
			break;
		case 1:
			refWord = CALIB_TIME_14_67_US_24MHZ;
			break;
		case 2:
			refWord = CALIB_TIME_29_33_US_24MHZ;
			break;
		case 3:
			refWord = CALIB_TIME_58_67_US_24MHZ;
			break;
		}
	}
	return refWord;
}

/* Selects a VCO. */
void CSpiritCalibration::selectVco(VcoSel xVco) {
	uint8_t tempRegValue;
	m_driverSpi->readRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
	tempRegValue &= 0xF9;
	if (xVco == VCO_H) {
		tempRegValue |= 0x02;
	} else {
		tempRegValue |= 0x04;
	}
	m_driverSpi->writeRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
}

/* Returns the VCO selected. */
CSpiritCalibration::VcoSel CSpiritCalibration::getVcoSelecttion(void) {
	uint8_t tempRegValue;
	m_driverSpi->readRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
	tempRegValue = (tempRegValue >> 1) & 0x3;
	if (tempRegValue == 0x01) {
		return VCO_H;
	} else {
		return VCO_L;
	}
}
