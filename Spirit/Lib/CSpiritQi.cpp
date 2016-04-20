/*
 * CSpiritQi.cpp
 *
 *  Created on: 25 lis 2015
 *      Author: Marcin
 */

#include "CSpiritQi.h"

void CSpiritQi::init(CSpiritInterface* driverSpi){
	m_driverSpi = driverSpi;
}

/*
 * Enables/Disables the PQI Preamble Quality Indicator check. The running peak PQI is
 * compared to a threshold value and the preamble valid IRQ is asserted as soon as the threshold is passed.
 */
void CSpiritQi::pqiCheck(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the QI register value */
	m_driverSpi->readRegisters(QI_BASE, 1, &tempRegValue);
	/* Enables or disables the PQI Check bit on the QI_BASE register */
	if (xNewState) {
		tempRegValue |= QI_PQI_MASK;
	} else {
		tempRegValue &= ~QI_PQI_MASK;
	}
	/* Writes value on the QI register */
	m_driverSpi->writeRegisters(QI_BASE, 1, &tempRegValue);
}

/*
 * Enables/Disables the Synchronization Quality Indicator check. The running peak SQI is
 * compared to a threshold value and the sync valid IRQ is asserted as soon as the threshold is passed.
 */
void CSpiritQi::sqiCheck(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the QI register value */
	m_driverSpi->readRegisters(QI_BASE, 1, &tempRegValue);
	/* Enables or disables the SQI Check bit on the QI_BASE register */
	if (xNewState) {
		tempRegValue |= QI_SQI_MASK;
	} else {
		tempRegValue &= ~QI_SQI_MASK;
	}
	/* Writes value on the QI register */
	m_driverSpi->writeRegisters(QI_BASE, 1, &tempRegValue);
}

/* Sets the PQI threshold. The preamble quality threshold is 4*PQI_TH (PQI_TH = 0..15). */
void CSpiritQi::setPqiThreshold(PqiThreshold xPqiThr) {
	uint8_t tempRegValue;
	/* Reads the QI register value */
	m_driverSpi->readRegisters(QI_BASE, 1, &tempRegValue);
	/* Build the PQI threshold value to be written */
	tempRegValue &= 0xC3;
	tempRegValue |= ((uint8_t) xPqiThr);
	/* Writes value on the QI register */
	m_driverSpi->writeRegisters(QI_BASE, 1, &tempRegValue);
}

/* Returns the PQI threshold. The preamble quality threshold is 4*PQI_TH (PQI_TH = 0..15). */
CSpiritQi::PqiThreshold CSpiritQi::getPqiThreshold(void)
{
  uint8_t tempRegValue;
  /* Reads the QI register value */
  m_driverSpi->readRegisters(QI_BASE, 1, &tempRegValue);
  /* Rebuild and return the PQI threshold value */
  return (PqiThreshold)(tempRegValue & 0x3C);
}

/*
 * Sets the SQI threshold. The synchronization quality
 * threshold is equal to 8 * SYNC_LEN - 2 * SQI_TH with SQI_TH = 0..3. When SQI_TH is 0 perfect match is required; when
 * SQI_TH = 1, 2, 3 then 1, 2, or 3 bit errors are respectively accepted. It is recommended that the SQI check is always
 * enabled.
 */
void CSpiritQi::setSqiThreshold(SqiThreshold xSqiThr)
{
  uint8_t tempRegValue;
  /* Reads the QI register value */
  m_driverSpi->readRegisters(QI_BASE, 1, &tempRegValue);
  /* Build the SQI threshold value to be written */
  tempRegValue &= 0x3F;
  tempRegValue |= ((uint8_t)xSqiThr);
  /* Writes the new value on the QI register */
  m_driverSpi->readRegisters(QI_BASE, 1, &tempRegValue);
}

/* Returns the SQI threshold. The synchronization quality threshold is equal to 8 * SYNC_LEN - 2 * SQI_TH with SQI_TH = 0..3. */
CSpiritQi::SqiThreshold CSpiritQi::getSqiThreshold(void)
{
  uint8_t tempRegValue;
  /* Reads the QI register value */
  m_driverSpi->readRegisters(QI_BASE, 1, &tempRegValue);
  /* Rebuild and return the SQI threshold value */
  return (SqiThreshold)(tempRegValue & 0xC0);
}

/* Sets the RSSI threshold. */
void CSpiritQi::setRssiThreshold(uint8_t cRssiThr) {
	/* Writes the new value on the RSSI_TH register */
	m_driverSpi->writeRegisters(RSSI_TH_BASE, 1, &cRssiThr);
}
/* Returns the RSSI threshold. */
uint8_t CSpiritQi::getRssiThreshold(void) {
	uint8_t tempRegValue;
	/* Reads the RSSI_TH register value */
	m_driverSpi->readRegisters(RSSI_TH_BASE, 1, &tempRegValue);
	/* Returns RSSI threshold */
	return tempRegValue;
}

/* Computes the RSSI threshold from its dBm value according to the formula: (RSSI[Dbm] + 130)/0.5 */
uint8_t CSpiritQi::computeRssiThreshold(int nDbmValue) {
	/* Check the parameters */
	if (!(nDbmValue >= -130 && nDbmValue <= -2)) {
		while (1){}
	}
	/* Computes the RSSI threshold for register */
	return 2 * (nDbmValue + 130);
}

/* Sets the RSSI threshold from its dBm value according to the formula: (RSSI[Dbm] + 130)/0.5. */
void CSpiritQi::setRssiThresholddBm(int nDbmValue) {
	uint8_t tempRegValue = 2 * (nDbmValue + 130);
	/* Check the parameters */
	if (!(nDbmValue >= -130 && nDbmValue <= -2)) {
		while (1) {}
	}
	/* Writes the new value on the RSSI_TH register */
	m_driverSpi->writeRegisters(RSSI_TH_BASE, 1, &tempRegValue);
}

/* Returns the PQI value. */
uint8_t CSpiritQi::getPqi(void) {
	uint8_t tempRegValue;
	/* Reads the LINK_QUALIF2 register value */
	m_driverSpi->readRegisters(LINK_QUALIF2_BASE, 1, &tempRegValue);
	/* Returns the PQI value */
	return tempRegValue;
}

/* Returns the SQI value. */
uint8_t CSpiritQi::getSqi(void) {
	uint8_t tempRegValue;
	/* Reads the register LINK_QUALIF1 value */
	m_driverSpi->readRegisters(LINK_QUALIF1_BASE, 1, &tempRegValue);
	/* Rebuild and return the SQI value */
	return (tempRegValue & 0x7F);
}

/* Returns the LQI value. */
uint8_t CSpiritQi::getLqi(void) {
	uint8_t tempRegValue;
	/* Reads the LINK_QUALIF0 register value */
	m_driverSpi->readRegisters(LINK_QUALIF0_BASE, 1, &tempRegValue);
	/* Rebuild and return the LQI value */
	return ((tempRegValue & 0xF0) >> 4);
}

/* Returns the CS status. */
CSpiritTypes::SpiritFlagStatus CSpiritQi::getCs(void) {
	uint8_t tempRegValue;
	/* Reads the LINK_QUALIF1 register value */
	m_driverSpi->readRegisters(LINK_QUALIF1_BASE, 1, &tempRegValue);
	/* Rebuild and returns the CS status value */
	if ((tempRegValue & 0x80) == 0) {
		return CSpiritTypes::S_RESET;
	} else {
		return CSpiritTypes::S_SET;
	}
}

/*
 * Returns the RSSI value. The measured power is reported in steps of half a dB from 0 to 255 and is offset in such a way that -120 dBm corresponds
 * to 20.
 */
uint8_t CSpiritQi::getRssi(void)
{
  uint8_t tempRegValue;
  /* Reads the RSSI_LEVEL register value */
  m_driverSpi->readRegisters(RSSI_LEVEL_BASE, 1, &tempRegValue);
  /* Returns the RSSI value */
  return tempRegValue;
}

/*
 * Sets the RSSI filter gain. This parameter sets the bandwidth of a low pass IIR filter (RSSI_FLT register, allowed values 0..15), a
 * lower values gives a faster settling of the measurements but lower precision. The recommended value for such parameter is 14.
 */
void CSpiritQi::setRssiFilterGain(RssiFilterGain xRssiFg) {
	uint8_t tempRegValue;
	/* Reads the RSSI_FLT register */
	m_driverSpi->readRegisters(RSSI_FLT_BASE, 1, &tempRegValue);
	/* Sets the specified filter gain */
	tempRegValue &= 0x0F;
	tempRegValue |= ((uint8_t) xRssiFg);
	/* Writes the new value on the RSSI_FLT register */
	m_driverSpi->writeRegisters(RSSI_FLT_BASE, 1, &tempRegValue);
}

/* Returns the RSSI filter gain. */
CSpiritQi::RssiFilterGain CSpiritQi::getRssiFilterGain(void) {
	uint8_t tempRegValue;
	/* Reads the RSSI_FLT register */
	m_driverSpi->readRegisters(RSSI_FLT_BASE, 1, &tempRegValue);
	/* Rebuild and returns the filter gain value */
	return (RssiFilterGain) (tempRegValue & 0xF0);
}

/*
 * Sets the CS Mode. When static carrier sensing is used (cs_mode = 0), the carrier sense signal is asserted when the measured RSSI is above the
 * value specified in the RSSI_TH register and is deasserted when the RSSI falls 3 dB below the same threshold.
 * When dynamic carrier sense is used (cs_mode = 1, 2, 3), the carrier sense signal is asserted if the signal is above the
 * threshold and a fast power increase of 6, 12 or 18 dB is detected; it is deasserted if a power fall of the same amplitude is
 * detected.
 */
void CSpiritQi::setCsMode(CSMode xCsMode) {
	uint8_t tempRegValue;
	/* Reads the RSSI_FLT register */
	m_driverSpi->readRegisters(RSSI_FLT_BASE, 1, &tempRegValue);
	/* Sets bit to select the CS mode */
	tempRegValue &= ~0x0C;
	tempRegValue |= ((uint8_t) xCsMode);
	/* Writes the new value on the RSSI_FLT register */
	m_driverSpi->writeRegisters(RSSI_FLT_BASE, 1, &tempRegValue);
}

/* Returns the CS Mode. */
CSpiritQi::CSMode CSpiritQi::getCsMode(void) {
	uint8_t tempRegValue;
	/* Reads the RSSI_FLT register */
	m_driverSpi->readRegisters(RSSI_FLT_BASE, 1, &tempRegValue);
	/* Rebuild and returns the CS mode value */
	return (CSMode) (tempRegValue & 0x0C);
}

/* Enables/Disables the CS Timeout Mask. If enabled CS value contributes to timeout disabling. */
void CSpiritQi::setCsTimeoutMask(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the PROTOCOL2 register value */
	m_driverSpi->readRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
	/* Enables or disables the CS timeout mask */
	if (xNewState) {
		tempRegValue |= PROTOCOL2_CS_TIMEOUT_MASK;
	} else {
		tempRegValue &= ~PROTOCOL2_CS_TIMEOUT_MASK;
	}
	/* Writes the new value on the PROTOCOL2 register */
	m_driverSpi->writeRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
}

/* Enables/Disables the PQI Timeout Mask. If enabled PQI value contributes to timeout disabling. */
void CSpiritQi::setPqiTimeoutMask(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the PROTOCOL2 register */
	m_driverSpi->readRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
	/* Enables or disables the PQI timeout mask */
	if (xNewState) {
		tempRegValue |= PROTOCOL2_PQI_TIMEOUT_MASK;
	} else {
		tempRegValue &= ~PROTOCOL2_PQI_TIMEOUT_MASK;
	}
	/* Writes the new value on the PROTOCOL2 register */
	m_driverSpi->writeRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
}

/* Enables/Disables the SQI Timeout Mask. If enabled SQI value contributes to timeout disabling. */
void CSpiritQi::setSqiTimeoutMask(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the PROTOCOL2 register */
	m_driverSpi->readRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
	/* Enables or disables the SQI timeout mask */
	if (xNewState) {
		tempRegValue |= PROTOCOL2_SQI_TIMEOUT_MASK;
	} else {
		tempRegValue &= ~PROTOCOL2_SQI_TIMEOUT_MASK;
	}
	/* Writes the new value on the PROTOCOL2 register */
	m_driverSpi->writeRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
}
