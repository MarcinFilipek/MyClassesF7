/*
 * CSpiritManagement.cpp
 *
 *  Created on: 19 lis 2015
 *      Author: Marcin
 */

#include "CSpiritManagement.h"
#include <math.h>

uint8_t CSpiritManagement::m_communicationState = COMMUNICATION_STATE_NONE;

/* Factor is: B/2 used in the formula for SYNTH word calculation */
const uint8_t CSpiritManagement::m_vectcBHalfFactor[4]={(HIGH_BAND_FACTOR/2), (MIDDLE_BAND_FACTOR/2), (LOW_BAND_FACTOR/2), (VERY_LOW_BAND_FACTOR/2)};

/* BS value to write in the SYNT0 register according to the selected band */
const uint8_t CSpiritManagement::m_vectcBandRegValue[4]={SYNT0_BS_6, SYNT0_BS_12, SYNT0_BS_16, SYNT0_BS_32};

/* It represents the available VCO frequencies */
const uint16_t CSpiritManagement::m_vectnVCOFreq[16]=
{
  4644, 4708, 4772, 4836, 4902, 4966, 5030, 5095, \
    5161, 5232, 5303, 5375, 5448, 5519, 5592, 5663
};


void CSpiritManagement::init(CSpiritInterface* driverSpi, CSpiritCommand* spiritCommand){
	m_driverSpi = driverSpi;
	m_spiritCalibration.init(m_driverSpi);
	m_spiritCommand = spiritCommand;
}

void CSpiritManagement::waTRxFcMem(uint32_t desiredFreq){
	m_communicationState = COMMUNICATION_STATE_NONE;
	m_desiredFrequency = desiredFreq;
}

/* Sets the XTAL frequency. */
void CSpiritManagement::setXtalFrequency(uint32_t freq){
	m_xtalFrequency = freq;
}

/* Gets the XTAL frequency. */
uint32_t CSpiritManagement::getXtalFrequency(){
	return m_xtalFrequency;
}

/* Enables or Disables the synthesizer reference divider. */
void CSpiritManagement::setRefDiv(bool wlacz) {
	uint8_t tempRegValue;
	/* Reads the SYNTH_CONFIG1_BASE and mask the REFDIV bit field */
	m_driverSpi->readRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
	if (wlacz) {
		tempRegValue |= 0x80;
	} else {
		tempRegValue &= 0x7F;
	}
	/* Writes the new value in the SYNTH_CONFIG1_BASE register */
	m_driverSpi->writeRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
}

/* Get the the synthesizer reference divider state. */
bool CSpiritManagement::getRefDiv(void) {
	uint8_t tempRegValue;
	m_driverSpi->readRegisters(SYNTH_CONFIG1_BASE, 1, &tempRegValue);
	if (((tempRegValue >> 7) & 0x1)) {
		return true;
	} else {
		return false;
	}
}


/* Sets the FC OFFSET register starting from frequency offset expressed in Hz. */
void CSpiritManagement::setFrequencyOffset(int32_t lFOffset){
	uint8_t tempArray[2];
	int16_t offset;
	uint32_t xtalFrequency = getXtalFrequency();
	/* Calculates the offset value to write in the FC_OFFSET register */
	offset = (int16_t)(((float)lFOffset*FBASE_DIVIDER)/xtalFrequency);
	/* Build the array related to the FC_OFFSET_1 and FC_OFFSET_0 register */
	tempArray[0]=(uint8_t)((((uint16_t)offset)>>8)&0x0F);
	tempArray[1]=(uint8_t)(offset);
	/* Writes the FC_OFFSET registers */
	m_driverSpi->writeRegisters(FC_OFFSET1_BASE, 2, tempArray);
}

/* Returns the actual frequency offset. */
int32_t CSpiritManagement::getFrequencyOffset(void)
{
	uint8_t tempArray[2];
	int16_t xtalOffsetFactor;
	uint32_t xtalFrequency = getXtalFrequency();
	/* Reads the FC_OFFSET registers */
	m_driverSpi->readRegisters(FC_OFFSET1_BASE, 2, tempArray);
	/* Calculates the Offset Factor */
	uint16_t xtalOffTemp = ((((uint16_t)tempArray[0])<<8)+((uint16_t)tempArray[1]));
	if (xtalOffTemp & 0x0800) {
		xtalOffTemp = xtalOffTemp | 0xF000;
	} else {
		xtalOffTemp = xtalOffTemp & 0x0FFF;
	}
	xtalOffsetFactor = *((int16_t*)(&xtalOffTemp));
	/* Calculates the frequency offset and return it */
	return ((int32_t)(xtalOffsetFactor*xtalFrequency)/FBASE_DIVIDER);
}

/*
 * Sets the channel space factor in channel space register.
 * The channel spacing step is computed as F_Xo/32768.
 */
void CSpiritManagement::setChannelSpace(uint32_t fChannelSpace){
	uint8_t cChannelSpaceFactor;
	uint32_t xtalFrequency = getXtalFrequency();
	/* Round to the nearest integer */
	cChannelSpaceFactor = ((uint32_t)fChannelSpace*CHSPACE_DIVIDER)/xtalFrequency;
	/* Write value into the register */
	m_driverSpi->writeRegisters(CHSPACE_BASE, 1, &cChannelSpaceFactor);
}

/* Returns the channel space register. */
uint32_t CSpiritManagement::getChannelSpace(void){
	uint8_t channelSpaceFactor;
	uint32_t xtalFrequency = getXtalFrequency();
	/* Reads the CHSPACE register, calculate the channel space and return it */
	m_driverSpi->readRegisters(CHSPACE_BASE, 1, &channelSpaceFactor);
	/* Compute the Hertz value and return it */
	return ((channelSpaceFactor*xtalFrequency)/CHSPACE_DIVIDER);
}

/* Sets the channel number. */
void CSpiritManagement::setChannel(uint8_t cChannel){
	/* Writes the CHNUM register */
	m_driverSpi->writeRegisters(CHNUM_BASE, 1, &cChannel);
}

/* Returns the actual channel number. */
uint8_t CSpiritManagement::getChannel(void){
	uint8_t tempRegValue;
	/* Reads the CHNUM register and return the value */
	m_driverSpi->readRegisters(CHNUM_BASE, 1, &tempRegValue);
	return tempRegValue;
}

/* Returns the charge pump word for a given VCO frequency. */
uint8_t CSpiritManagement::searchWCP(uint32_t lFc) {
	int8_t i = 0;
	uint32_t vcofreq = 0;
	uint8_t BFactor = 0;
	/* Search the operating band */
	if (IS_FREQUENCY_BAND_HIGH(lFc)) {
		BFactor = HIGH_BAND_FACTOR;
	} else if (IS_FREQUENCY_BAND_MIDDLE(lFc)) {
		BFactor = MIDDLE_BAND_FACTOR;
	} else if (IS_FREQUENCY_BAND_LOW(lFc)) {
		BFactor = LOW_BAND_FACTOR;
	} else if (IS_FREQUENCY_BAND_VERY_LOW(lFc)) {
		BFactor = VERY_LOW_BAND_FACTOR;
	}
	/* Calculates the VCO frequency VCOFreq = lFc*B */
	vcofreq = (lFc / 1000000) * BFactor;
	/* Search in the vco frequency array the charge pump word */
	if (vcofreq >= m_vectnVCOFreq[15]) {
		i = 15;
	} else {
		/* Search the value */
		for (i = 0; i < 15 && vcofreq > m_vectnVCOFreq[i]; i++);
		/* Be sure that it is the best approssimation */
		if (i != 0 && m_vectnVCOFreq[i] - vcofreq > vcofreq - m_vectnVCOFreq[i - 1])
			i--;
	}
	/* Return index */
	return (i % 8);
}

/*
 * Sets the Synth word and the Band Select register according to desired base carrier frequency.
 * In this API the Xtal configuration is read out from
 * the corresponding register. The user shall fix it before call this API.
 */
uint8_t CSpiritManagement::setRadioFrequencyBase(uint32_t lFBase){
	uint32_t synthWord, Fc;
	uint8_t band = 0, anaRadioRegArray[4], wcp;
	uint32_t xtalFrequency = getXtalFrequency();
	/* Search the operating band */
	if (IS_FREQUENCY_BAND_HIGH(lFBase)) {
		band = HIGH_BAND;
	} else if (IS_FREQUENCY_BAND_MIDDLE(lFBase)) {
		band = MIDDLE_BAND;
	} else if (IS_FREQUENCY_BAND_LOW(lFBase)) {
		band = LOW_BAND;
	} else if (IS_FREQUENCY_BAND_VERY_LOW(lFBase)) {
		band = VERY_LOW_BAND;
	}
	int32_t FOffset  = getFrequencyOffset();
	uint32_t lChannelSpace  = getChannelSpace();
	uint8_t cChannelNum = getChannel();
	/* Calculates the channel center frequency */
	Fc = lFBase + FOffset + lChannelSpace*cChannelNum;
	/* Reads the reference divider */
	uint8_t cRefDiv = (uint8_t)getRefDiv()+1;
	/* Selects the VCO */
	switch (band) {
	case VERY_LOW_BAND:
		if (Fc < 161281250) {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_L);
		} else {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_H);
		}
		break;

	case LOW_BAND:
		if (Fc < 322562500) {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_L);
		} else {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_H);
		}
		break;

	case MIDDLE_BAND:
		if (Fc < 430083334) {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_L);
		} else {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_H);
		}
		break;

	case HIGH_BAND:
		if (Fc < 860166667) {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_L);
		} else {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_H);
		}
	}
	/* Search the VCO charge pump word and set the corresponding register */
	wcp = searchWCP(Fc);
	synthWord = (uint32_t)(lFBase*m_vectcBHalfFactor[band]*(((double)(FBASE_DIVIDER*cRefDiv))/xtalFrequency));
	/* Build the array of registers values for the analog part */
	anaRadioRegArray[0] = (uint8_t) (((synthWord >> 21) & (0x0000001F)) | (wcp << 5));
	anaRadioRegArray[1] = (uint8_t) ((synthWord >> 13) & (0x000000FF));
	anaRadioRegArray[2] = (uint8_t) ((synthWord >> 5) & (0x000000FF));
	anaRadioRegArray[3] = (uint8_t) (((synthWord & 0x0000001F) << 3) | m_vectcBandRegValue[band]);
	/* Configures the needed Analog Radio registers */
	m_driverSpi->writeRegisters(SYNT3_BASE, 4, anaRadioRegArray);
	return waVcoCalibration();
}

/* Private SpiritManagementSetFrequencyBase function only used in SpiritManagementWaVcoCalibration. */
void CSpiritManagement::setFrequencyBase(uint32_t lFBase){
	uint32_t synthWord, Fc;
	uint8_t band = 0, anaRadioRegArray[4], wcp;
	uint32_t xtalFrequency = getXtalFrequency();
	/* Search the operating band */
	if (IS_FREQUENCY_BAND_HIGH(lFBase)) {
		band = HIGH_BAND;
	} else if (IS_FREQUENCY_BAND_MIDDLE(lFBase)) {
		band = MIDDLE_BAND;
	} else if (IS_FREQUENCY_BAND_LOW(lFBase)) {
		band = LOW_BAND;
	} else if (IS_FREQUENCY_BAND_VERY_LOW(lFBase)) {
		band = VERY_LOW_BAND;
	}
	int32_t FOffset  = getFrequencyOffset();
	uint32_t lChannelSpace  = getChannelSpace();
	uint8_t cChannelNum = getChannel();
	/* Calculates the channel center frequency */
	Fc = lFBase + FOffset + lChannelSpace*cChannelNum;
	/* Reads the reference divider */
	uint8_t cRefDiv = (uint8_t)getRefDiv()+1;
	/* Selects the VCO */
	switch (band) {
	case VERY_LOW_BAND:
		if (Fc < 161281250) {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_L);
		} else {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_H);
		}
		break;

	case LOW_BAND:
		if (Fc < 322562500) {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_L);
		} else {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_H);
		}
		break;

	case MIDDLE_BAND:
		if (Fc < 430083334) {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_L);
		} else {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_H);
		}
		break;

	case HIGH_BAND:
		if (Fc < 860166667) {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_L);
		} else {
			m_spiritCalibration.selectVco(CSpiritCalibration::VCO_H);
		}
	}
	/* Search the VCO charge pump word and set the corresponding register */
	wcp = searchWCP(Fc);
	synthWord = (uint32_t)(lFBase*m_vectcBHalfFactor[band]*(((double)(FBASE_DIVIDER*cRefDiv))/xtalFrequency));
	/* Build the array of registers values for the analog part */
	anaRadioRegArray[0] = (uint8_t) (((synthWord >> 21) & (0x0000001F)) | (wcp << 5));
	anaRadioRegArray[1] = (uint8_t) ((synthWord >> 13) & (0x000000FF));
	anaRadioRegArray[2] = (uint8_t) ((synthWord >> 5) & (0x000000FF));
	anaRadioRegArray[3] = (uint8_t) (((synthWord & 0x0000001F) << 3) | m_vectcBandRegValue[band]);
	/* Configures the needed Analog Radio registers */
	m_driverSpi->writeRegisters(SYNT3_BASE, 4, anaRadioRegArray);
}

/* Returns the base carrier frequency. */
uint32_t CSpiritManagement::getFrequencyBase(){
	  uint32_t synthWord;
	  BandSelect band;
	  uint32_t xtalFrequency = getXtalFrequency();
	  /* Reads the synth word */
	  synthWord = getSynthWord();
	  /* Reads the operating band */
	  band = getBand();
	  uint8_t cRefDiv = (uint8_t)getRefDiv() + 1;
	  /* Calculates the frequency base and return it */
	  return (uint32_t)round(synthWord*(((double)xtalFrequency)/(FBASE_DIVIDER*cRefDiv*m_vectcBHalfFactor[band])));
}

/* Sets the SYNTH registers. */
void CSpiritManagement::setSynthWord(uint32_t lSynthWord) {
	uint8_t tempArray[4];
	uint8_t tempRegValue;
	/* Reads the SYNT0 register */
	m_driverSpi->readRegisters(SYNT0_BASE, 1, &tempRegValue);
	/* Mask the Band selected field */
	tempRegValue &= 0x07;
	/* Build the array for SYNTH registers */
	tempArray[0] = (uint8_t)((lSynthWord>>21)&(0x0000001F));
	tempArray[1] = (uint8_t)((lSynthWord>>13)&(0x000000FF));
	tempArray[2] = (uint8_t)((lSynthWord>>5)&(0x000000FF));
	tempArray[3] = (uint8_t)(((lSynthWord&0x0000001F)<<3)| tempRegValue);
	/* Writes the synth word in the SYNTH registers */
	m_driverSpi->writeRegisters(SYNT3_BASE, 4, tempArray);
}

/* Returns the synth word. */
uint32_t CSpiritManagement::getSynthWord(void){
	uint8_t regArray[4];
	/* Reads the SYNTH registers, build the synth word and return it */
	m_driverSpi->readRegisters(SYNT3_BASE, 4, regArray);
	return ((((uint32_t)(regArray[0]&0x1F))<<21)+(((uint32_t)(regArray[1]))<<13)+\
			(((uint32_t)(regArray[2]))<<5)+(((uint32_t)(regArray[3]))>>3));
}

/* Sets the operating band. */
void CSpiritManagement::setBand(BandSelect xBand) {
	uint8_t tempRegValue;
	/* Reads the SYNT0 register*/
	m_driverSpi->readRegisters(SYNT0_BASE, 1, &tempRegValue);
	/* Mask the SYNTH[4;0] field and write the BS value */
	tempRegValue &= 0xF8;
	tempRegValue |= m_vectcBandRegValue[xBand];

	/* Configures the SYNT0 register setting the operating band */
	m_driverSpi->writeRegisters(SYNT0_BASE, 1, &tempRegValue);
}

/* Returns the operating band. */
CSpiritManagement::BandSelect CSpiritManagement::getBand(void) {
	uint8_t tempRegValue;
	/* Reads the SYNT0 register */
	m_driverSpi->readRegisters(SYNT0_BASE, 1, &tempRegValue);
	/* Mask the Band selected field */
	if ((tempRegValue & 0x07) == SYNT0_BS_6) {
		return HIGH_BAND;
	} else if ((tempRegValue & 0x07) == SYNT0_BS_12) {
		return MIDDLE_BAND;
	} else if ((tempRegValue & 0x07) == SYNT0_BS_16) {
		return LOW_BAND;
	} else {
		return VERY_LOW_BAND;
	}
}

uint8_t CSpiritManagement::waVcoCalibration(void) {
	uint8_t s_cVcoWordRx;
	uint8_t s_cVcoWordTx;
	uint32_t nFreq;
	uint8_t cRestore = 0;
	uint8_t cStandby = 0;
	uint32_t xtal_frequency = getXtalFrequency();
	/* Enable the reference divider if the XTAL is between 48 and 52 MHz */
	if (xtal_frequency > 26000000) {
		if (!getRefDiv()) {
			cRestore = 1;
			nFreq = getFrequencyBase();
			setRefDiv(true);
			setFrequencyBase(nFreq);
		}
	}
	nFreq = getFrequencyBase();
	/* Increase the VCO current */
	uint8_t tmp = 0x19;
	m_driverSpi->writeRegisters(0xA1, 1, &tmp);
	m_spiritCalibration.setVco(true);
	m_driverSpi->getSpiritTypes()->refreshStatus();
	if (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE == CSpiritTypes::MC_STATE_STANDBY) {
		cStandby = 1;
		m_spiritCommand->strobeCommand(CSpiritCommand::CMD_READY);
		do {
			m_driverSpi->getSpiritTypes()->refreshStatus();
			if (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE == 0x13) {
				return 1;
			}
		} while (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE != CSpiritTypes::MC_STATE_READY);
	}
	m_spiritCommand->strobeCommand(CSpiritCommand::CMD_LOCKTX);
	do {
		m_driverSpi->getSpiritTypes()->refreshStatus();
		if (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE == 0x13) {
			return 1;
		}
	} while (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE != CSpiritTypes::MC_STATE_LOCK);
	s_cVcoWordTx = m_spiritCalibration.getVcoCalData();
	m_spiritCommand->strobeCommand(CSpiritCommand::CMD_READY);
	do {
		m_driverSpi->getSpiritTypes()->refreshStatus();
	} while (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE != CSpiritTypes::MC_STATE_READY);
	m_spiritCommand->strobeCommand(CSpiritCommand::CMD_LOCKRX);
	do {
		m_driverSpi->getSpiritTypes()->refreshStatus();
		if (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE == 0x13) {
			return 1;
		}
	} while (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE != CSpiritTypes::MC_STATE_LOCK);
	s_cVcoWordRx = m_spiritCalibration.getVcoCalData();
	m_spiritCommand->strobeCommand(CSpiritCommand::CMD_READY);
	do {
		m_driverSpi->getSpiritTypes()->refreshStatus();
		if (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE == 0x13) {
			return 1;
		}
	} while (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE != CSpiritTypes::MC_STATE_READY);

	if (cStandby == 1) {
		m_spiritCommand->strobeCommand(CSpiritCommand::CMD_STANDBY);
	}
	m_spiritCalibration.setVco(false);
	/* Disable the reference divider if the XTAL is between 48 and 52 MHz */
	if (cRestore) {
		setRefDiv(false);
		setFrequencyBase(nFreq);
	}
	/* Restore the VCO current */
	tmp = 0x11;
	m_driverSpi->writeRegisters(0xA1, 1, &tmp);
	m_spiritCalibration.setVcoCalDataTx(s_cVcoWordTx);
	m_spiritCalibration.setVcoCalDataRx(s_cVcoWordRx);
	return 0;
}
