/*
 * CSpiritTypes.cpp
 *
 *  Created on: 12 lis 2015
 *      Author: marcin
 */

#include "CSpiritTypes.h"

void CSpiritTypes::init(CSpiritSpi *driverSpi){
	m_driverSpi = driverSpi;
}
/* Updates the gState (the global variable used to maintain memory of Spirit Status)
 *         reading the MC_STATE register of SPIRIT.
 */
void CSpiritTypes::refreshStatus(){
	uint8_t tempRegValue;
	int16_t tempstatus;
	tempstatus = m_driverSpi->readRegisters(MC_STATE1_BASE, 1, &tempRegValue);
	SpiritStatus* wskStatus = (SpiritStatus*)&tempstatus;
	m_status = *wskStatus;
}
