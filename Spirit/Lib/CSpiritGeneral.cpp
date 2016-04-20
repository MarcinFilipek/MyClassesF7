/*
 * CSpiritGeneral.cpp
 *
 *  Created on: 18 lis 2015
 *      Author: Marcin
 */

#include "CSpiritGeneral.h"

CSpiritGeneral::CSpiritGeneral(){
	m_spiritVersion = SPIRIT_VERSION_3_0;
}

void CSpiritGeneral::init(CSpiritInterface* driverSpi){
	m_driverSpi = driverSpi;
}
void CSpiritGeneral::setSpiritVersion(SpiritVersion version){
	m_spiritVersion = version;
}

void CSpiritGeneral::setExtRef(ModeExtRef extRef) {
	uint8_t tempRegValue;
	/* Reads the ANA_FUNC_CONF0_BASE register value */
	m_driverSpi->readRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);
	/* Build the value to write */
	if (extRef == MODE_EXT_XO) {
		tempRegValue &= ~EXT_REF_MASK;
	} else {
		tempRegValue |= EXT_REF_MASK;
	}
	/* Writes value on register */
	m_driverSpi->writeRegisters(ANA_FUNC_CONF0_BASE, 1, &tempRegValue);
}
