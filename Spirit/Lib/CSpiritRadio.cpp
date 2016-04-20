/*
 * CSpiritRadio.cpp
 *
 *  Created on: 17 lis 2015
 *      Author: marcin
 */

#include "CSpiritRadio.h"
#include <math.h>

/*
 * It represents the available channel bandwidth times 10 for 26 Mhz xtal.
 * The channel bandwidth for others xtal frequencies can be computed since this table
 * multiplying the current table by a factor xtal_frequency/26e6.
 */
const uint16_t CSpiritRadio::m_vectnBandwidth26M[]=
{
  8001, 7951, 7684, 7368, 7051, 6709, 6423, 5867, 5414, \
    4509, 4259, 4032, 3808, 3621, 3417, 3254, 2945, 2703, \
      2247, 2124, 2011, 1900, 1807, 1706, 1624, 1471, 1350, \
        1123, 1062, 1005,  950,  903,  853,  812,  735,  675, \
          561,  530,  502,  474,  451,  426,  406,  367,  337, \
            280,  265,  251,  237,  226,  213,  203,  184,  169, \
              140,  133,  126,  119,  113,  106,  101,   92,   84, \
                70,   66,   63,   59,   56,   53,   51,   46,   42, \
                  35,   33,   31,   30,   28,   27,   25,   23,   21, \
                    18,   17,   16,   15,   14,   13,   13,   12,   11
};


void CSpiritRadio::init(CSpiritInterface* driverSpi, CSpiritManagement* spiritManagement){
	m_driverSpi = driverSpi;
	m_spiritManagement = spiritManagement;
	m_spiritCalibration = m_spiritManagement->getSpiritCalibration();
}
/*
 * Initializes the SPIRIT analog and digital radio part according to the specified
 * parameters in the pxSRadioInitStruct.
 */
uint8_t CSpiritRadio::initRadio(SRadioInit* radioInitStruct){
	int32_t FOffsetTmp;
	uint8_t anaRadioRegArray[8], digRadioRegArray[4];
	int16_t xtalOffsetFactor;
	uint8_t drM, drE, FdevM, FdevE, bwM, bwE;
	uint32_t xtalFrequency = getXtalFrequency();
	/* Workaround for Vtune */
	uint8_t value = 0xA0;
	m_driverSpi->writeRegisters(0x9F, 1, &value);
	/* Calculates the offset respect to RF frequency and according to xtal_ppm parameter: (xtal_ppm*FBase)/10^6 */
	FOffsetTmp = (int32_t) (((float) radioInitStruct->nXtalOffsetPpm * radioInitStruct->lFrequencyBase) / PPM_FACTOR);
	/* Disable the digital, ADC, SMPS reference clock divider if fXO>24MHz or fXO<26MHz */
	m_driverSpi->commandStrobes(COMMAND_STANDBY);
	do {
		/* Delay for state transition */
		for (volatile uint8_t i = 0; i != 0xFF; i++);
		/* Reads the MC_STATUS register */
		m_driverSpi->getSpiritTypes()->refreshStatus();
	} while (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE != CSpiritTypes::MC_STATE_STANDBY);
	if (xtalFrequency < 48000000) {
		setDigDiv(false);
	} else {
		setDigDiv(true);
	}
	/* Goes in READY state */
	m_driverSpi->commandStrobes(COMMAND_READY);
	do {
		/* Delay for state transition */
		for (volatile uint8_t i = 0; i != 0xFF; i++)
			;
		/* Reads the MC_STATUS register */
		m_driverSpi->getSpiritTypes()->refreshStatus();
	} while (m_driverSpi->getSpiritTypes()->getSpiritStatus().MC_STATE != CSpiritTypes::MC_STATE_READY);
	/* Calculates the FC_OFFSET parameter and cast as signed int: FOffsetTmp = (Fxtal/2^18)*FC_OFFSET */
	xtalOffsetFactor = (int16_t) (((float) FOffsetTmp * FBASE_DIVIDER) / xtalFrequency);
	anaRadioRegArray[2] = (uint8_t) ((((uint16_t) xtalOffsetFactor) >> 8) & 0x0F);
	anaRadioRegArray[3] = (uint8_t) (xtalOffsetFactor);
	/* Calculates the channel space factor */
	anaRadioRegArray[0] = ((uint32_t) radioInitStruct->nChannelSpace << 9) / (xtalFrequency >> 6) + 1;
	m_spiritManagement->waTRxFcMem(radioInitStruct->lFrequencyBase);
	/* 2nd order DEM algorithm enabling */
	uint8_t tmpreg;
	m_driverSpi->readRegisters(0xA3, 1, &tmpreg);
	tmpreg &= ~0x02;
	m_driverSpi->writeRegisters(0xA3, 1, &tmpreg);
	/* Calculates the datarate mantissa and exponent */
	searchDatarateME(radioInitStruct->lDatarate, &drM, &drE);
	digRadioRegArray[0] = (uint8_t) (drM);
	digRadioRegArray[1] = (uint8_t) (0x00 | radioInitStruct->xModulationSelect | drE);
	/* Calculates the frequency deviation mantissa and exponent */
	searchFreqCevME(radioInitStruct->lFreqDev, &FdevM, &FdevE);
	digRadioRegArray[2] = (uint8_t) ((FdevE << 4) | 0x00 | FdevM);
	/* Calculates the channel filter mantissa and exponent */
	searchChannelBwME(radioInitStruct->lBandwidth, &bwM, &bwE);
	uint8_t ifOffsetAna;
	if (xtalFrequency == 24000000) {
		ifOffsetAna = 0xB6;
		anaRadioRegArray[1] = 0xB6;
	}
	if (xtalFrequency == 25000000) {
		ifOffsetAna = 0xAC;
		anaRadioRegArray[1] = 0xAC;
	}
	if (xtalFrequency == 26000000) {
		ifOffsetAna = 0xA3;
		anaRadioRegArray[1] = 0xA3;
	}
	if (xtalFrequency == 48000000) {
		ifOffsetAna = 0x3B;
		anaRadioRegArray[1] = 0xB6;
	}
	if (xtalFrequency == 50000000) {
		ifOffsetAna = 0x36;
		anaRadioRegArray[1] = 0xAC;
	}
	if (xtalFrequency == 52000000) {
		ifOffsetAna = 0x31;
		anaRadioRegArray[1] = 0xA3;
	}
	m_driverSpi->writeRegisters(IF_OFFSET_ANA_BASE, 1, &ifOffsetAna);
	/* Sets Xtal configuration */
	if (xtalFrequency > 26000000) {
		setXtalFlag(XTAL_FLAG((xtalFrequency / 2)));
	} else {
		setXtalFlag(XTAL_FLAG(xtalFrequency));
	}
	/* Sets the channel number in the corresponding register */
	m_driverSpi->writeRegisters(CHNUM_BASE, 1, &radioInitStruct->cChannelNumber);
	/* Configures the Analog Radio registers */
	m_driverSpi->writeRegisters(CHSPACE_BASE, 4, anaRadioRegArray);
	/* Configures the Digital Radio registers */
	m_driverSpi->writeRegisters(MOD1_BASE, 4, digRadioRegArray);
	/* Enable the freeze option of the AFC on the SYNC word */
	AFCFreezeOnSync(true);
	/* Set the IQC correction optimal value */
	anaRadioRegArray[0]=0x80;
	anaRadioRegArray[1]=0xE3;
	m_driverSpi->writeRegisters(0x99, 2, anaRadioRegArray);
	return m_spiritManagement->setRadioFrequencyBase(radioInitStruct->lFrequencyBase);
}


/* Sets the XTAL frequency. */
void CSpiritRadio::setXtalFrequency(uint32_t freq){
	m_spiritManagement->setXtalFrequency(freq);
}

/* Gets the XTAL frequency. */
uint32_t CSpiritRadio::getXtalFrequency(void){
	return m_spiritManagement->getXtalFrequency();
}

/* Enables or Disables the synthesizer reference divider. */
void CSpiritRadio::setDigDiv(bool wlacz){
	uint8_t tempRegValue;
	/* Reads the XO_RCO_TEST_BASE and mask the PD_CLKDIV bit field */
	m_driverSpi->readRegisters(XO_RCO_TEST_BASE, 1, &tempRegValue);
	if(wlacz){
		tempRegValue &= 0xf7;
	} else {
		tempRegValue |= 0x08;
	}
	m_driverSpi->writeRegisters(XO_RCO_TEST_BASE, 1, &tempRegValue);
}

/* Get the the synthesizer reference divider state. */
bool CSpiritRadio::getDigDiv(){
	uint8_t tempRegValue;
	m_driverSpi->readRegisters(XO_RCO_TEST_BASE, 1, &tempRegValue);
	if((tempRegValue>>3)&0x1){
		return false;
	} else {
		return true;
	}
}


/*
 * Returns the mantissa and exponent, whose value used in the datarate formula
 * will give the datarate value closer to the given datarate.
 */
void CSpiritRadio::searchDatarateME(uint32_t lDatarate, uint8_t* pcM, uint8_t* pcE){
	bool find = false;
	int8_t i=15;
	uint8_t cMantissaTmp;
	uint8_t cDivider = 0;
	uint32_t xtalFrequency = getXtalFrequency();
	cDivider = (uint8_t)getDigDiv();
	/* Search in the datarate array the exponent value */
	while (!find && i >= 0) {
		if (lDatarate >= (xtalFrequency >> (20 - i + cDivider))) {
			find = true;
		} else {
			i--;
		}
	}
	i < 0 ? i = 0 : i;
	*pcE = i;
	/* Calculates the mantissa value according to the datarate formula */
	cMantissaTmp = (lDatarate*((uint32_t)1<<(23-i)))/(xtalFrequency>>(5+cDivider))-256;
	/* Finds the mantissa value with less approximation */
	int16_t mantissaCalculation[3];
	for (uint8_t j = 0; j < 3; j++) {
		if ((cMantissaTmp + j - 1)) {
			mantissaCalculation[j] = lDatarate - (((256 + cMantissaTmp + j - 1) * (xtalFrequency >> (5 + cDivider))) >> (23 - i));
		} else {
			mantissaCalculation[j] = 0x7FFF;
		}
	}
	uint16_t mantissaCalculationDelta = 0xFFFF;
	for (uint8_t j = 0; j < 3; j++) {
		if (S_ABS(mantissaCalculation[j]) < mantissaCalculationDelta) {
			mantissaCalculationDelta = S_ABS(mantissaCalculation[j]);
			*pcM = cMantissaTmp + j - 1;
		}
	}
}

/*
 * Returns the mantissa and exponent, whose value used in the frequency deviation formula
 * will give a frequency deviation value most closer to the given frequency deviation.
 */
void CSpiritRadio::searchFreqCevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE){
	  uint8_t i;
	  uint32_t a,bp,b=0;
	  uint32_t xtalFrequency = getXtalFrequency();
	  float xtalDivtmp=(float)xtalFrequency/(((uint32_t)1)<<18);
	for (i = 0; i < 10; i++) {
		a = (uint32_t) (xtalDivtmp * (uint32_t) (7.5 * (1 << i)));
		if (lFDev < a)
			break;
	}
	(*pcE) = i;

	for (i = 0; i < 8; i++) {
		bp = b;
		b = (uint32_t) (xtalDivtmp * (uint32_t) ((8.0 + i) / 2 * (1 << (*pcE))));
		if (lFDev < b)
			break;
	}

	(*pcM) = i;
	if ((lFDev - bp) < (b - lFDev))
		(*pcM)--;
}

/*
 * Returns the mantissa and exponent for a given bandwidth.
 * Even if it is possible to pass as parameter any value in the below mentioned range,
 * the API will search the closer value according to a fixed table of channel
 * bandwidth values (@ref s_vectnBandwidth), as defined in the datasheet, returning the corresponding mantissa
 * and exponent value.
 */
void CSpiritRadio::searchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE){
	int8_t i, i_tmp;
	uint8_t cDivider = 1;
	uint32_t xtalFrequency = getXtalFrequency();
	/* Search in the channel filter bandwidth table the exponent value */
	if (getDigDiv()) {
		cDivider = 2;
	} else {
		cDivider = 1;
	}
	uint32_t lChfltFactor = (xtalFrequency/cDivider)/100;
	for(i=0;i<90 && (lBandwidth<(uint32_t)((m_vectnBandwidth26M[i]*lChfltFactor)/2600));i++);
	if (i != 0) {
		/* Finds the mantissa value with less approximation */
		i_tmp = i;
		int16_t chfltCalculation[3];
		for (uint8_t j = 0; j < 3; j++) {
			if (((i_tmp + j - 1) >= 0) || ((i_tmp + j - 1) <= 89)) {
				chfltCalculation[j] = lBandwidth - (uint32_t) ((m_vectnBandwidth26M[i_tmp + j - 1] * lChfltFactor) / 2600);
			} else {
				chfltCalculation[j] = 0x7FFF;
			}
		}
		uint16_t chfltDelta = 0xFFFF;

		for (uint8_t j = 0; j < 3; j++) {
			if (S_ABS(chfltCalculation[j]) < chfltDelta) {
				chfltDelta = S_ABS(chfltCalculation[j]);
				i = i_tmp + j - 1;
			}
		}
	}
	(*pcE) = (uint8_t) (i / 9);
	(*pcM) = (uint8_t) (i % 9);
}

/* Sets the Xtal configuration in the ANA_FUNC_CONF0 register. */
void CSpiritRadio::setXtalFlag(XtalFlag xtal) {
	uint8_t tempRegValue = 0x00;
	/* Reads the ANA_FUNC_CONF_0 register */
	m_driverSpi->readRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);
	if (xtal == XTAL_FLAG_26_MHz) {
		tempRegValue |= SELECT_24_26_MHZ_MASK;
	} else {
		tempRegValue &= (~SELECT_24_26_MHZ_MASK);
	}
	/* Sets the 24_26MHz_SELECT field in the ANA_FUNC_CONF_0 register */
	m_driverSpi->writeRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);
}

/* Returns the Xtal configuration in the ANA_FUNC_CONF0 register. */
CSpiritRadio::XtalFlag CSpiritRadio::getXtalFlag(void){
	uint8_t tempRegValue;
	/* Reads the Xtal configuration in the ANA_FUNC_CONF_0 register and return the value */
	m_driverSpi->readRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);
	return (XtalFlag)((tempRegValue & 0x40)>>6);

}

/*
 * Enables or Disables the AFC freeze on sync word detection.
 */
void CSpiritRadio::AFCFreezeOnSync(bool wlacz){
	uint8_t tempRegValue = 0x00;
	/* Reads the AFC_2 register and configure the AFC Freeze on Sync field */
	m_driverSpi->readRegisters(AFC2_BASE, 1, &tempRegValue);
	if (wlacz) {
		tempRegValue |= AFC2_AFC_FREEZE_ON_SYNC_MASK;
	} else {
		tempRegValue &= (~AFC2_AFC_FREEZE_ON_SYNC_MASK);
	}
	/* Sets the AFC_2 register */
	m_driverSpi->writeRegisters(AFC2_BASE, 1, &tempRegValue);
}

/*
 * Sets a specific PA_LEVEL register, with a value given in dBm.
 */
void CSpiritRadio::setPALeveldBm(uint8_t cIndex, float fPowerdBm) {
	uint8_t address, paLevelValue;
	/* Calculates the PA level value to write in the corresponding register using
	 an interpolation formula */
	float z = (fPowerdBm + 9.4089) / 12.08;
	paLevelValue = (uint8_t) round(0.082812 * z * z * z * z + 1.0653 * z * z * z - 0.82942 * z * z - 28.146 * z + 46.242);
	paLevelValue > 90 ? paLevelValue = 90 : paLevelValue;
	/* Sets the base address */
	address = PA_POWER8_BASE + 7 - cIndex;
	/* Configures the PA_LEVEL register */
	m_driverSpi->writeRegisters(address, 1, &paLevelValue);
}

/* Returns a specific PA_LEVEL register, returning a value in dBm. */
float CSpiritRadio::getPALeveldBm(uint8_t cIndex) {
	uint8_t address, paLevelValue;
	/* Sets the base address */
	address = PA_POWER8_BASE + 7 - cIndex;
	/* Reads the PA_LEVEL[cIndex] register */
	m_driverSpi->readRegisters(address, 1, &paLevelValue);
	//  float z= (paLevelValue-45.456)/26.037;
	float z = (paLevelValue - 45.5) / 26.125;
	/* Calculates the power level in dBm using the linearization formula and return the value */
	return -0.0045522 * z * z * z * z - 0.60856 * z * z * z - 0.3432 * z * z - 10.976 * z - 9.0615;
}

/* Sets a specific PA_LEVEL_MAX_INDEX. 8 */
void CSpiritRadio::setPALevelMaxIndex(uint8_t cIndex) {
	uint8_t tempRegValue;
	/* Reads the PA_POWER_0 register */
	m_driverSpi->readRegisters(PA_POWER0_BASE, 1, &tempRegValue);
	/* Mask the PA_LEVEL_MAX_INDEX[1:0] field and write the new value */
	tempRegValue &= 0xF8;
	tempRegValue |= cIndex;
	/* Configures the PA_POWER_0 register */
	m_driverSpi->writeRegisters(PA_POWER0_BASE, 1, &tempRegValue);
}

/* Returns the actual PA_LEVEL_MAX_INDEX. */
uint8_t CSpiritRadio::getPALevelMaxIndex(void)
{
  uint8_t tempRegValue;
  /* Reads the PA_POWER_0 register */
  m_driverSpi->readRegisters(PA_POWER0_BASE, 1, &tempRegValue);
  /* Mask the PA_LEVEL_MAX_INDEX[1:0] field and return the value */
  return (tempRegValue & 0x07);

}


/*
 * Sets the output stage additional load capacitor bank.
 */
void CSpiritRadio::setPACwc(PALoadCapacitor xCLoad) {
	uint8_t tempRegValue;
	/* Reads the PA_POWER_0 register */
	m_driverSpi->readRegisters(PA_POWER0_BASE, 1, &tempRegValue);
	/* Mask the CWC[1:0] field and write the new value */
	tempRegValue &= 0x3F;
	tempRegValue |= xCLoad;
	/* Configures the PA_POWER_0 register */
	m_driverSpi->writeRegisters(PA_POWER0_BASE, 1, &tempRegValue);
}

/*
 * Returns the output stage additional load capacitor bank.
 */
CSpiritRadio::PALoadCapacitor CSpiritRadio::getPACwc(void) {
	uint8_t tempRegValue;
	/* Reads the PA_POWER_0 register */
	m_driverSpi->readRegisters(PA_POWER0_BASE, 1, &tempRegValue);
	/* Mask the CWC[1:0] field and return the value*/
	return (PALoadCapacitor) (tempRegValue & 0xC0);
}
