/*
 * CSpiritCommand.cpp
 *
 *  Created on: 20 lis 2015
 *      Author: Marcin
 */

#include "CSpiritCommand.h"

/* Sends a specific command to SPIRIT. */

void CSpiritCommand::init(CSpiritInterface* driverSpi){
	m_driverSpi = driverSpi;
}

/* Sends a specific command to SPIRIT. */
void CSpiritCommand::strobeCommand(SpiritCmd commandCode)
{
	m_driverSpi->commandStrobes(commandCode);
}
