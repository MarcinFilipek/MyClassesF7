/*
 * CSpiritIrq.cpp
 *
 *  Created on: 25 lis 2015
 *      Author: marcin
 */

#include "CSpiritIrq.h"

void CSpiritIrq::init(CSpiritInterface* driverSpi){
	m_driverSpi = driverSpi;
}

/*
 * De initializate the SpiritIrqs structure setting all the bitfield to 0.
 * Moreover, it sets the IRQ mask registers to 0x00000000, disabling all IRQs.
 */
void CSpiritIrq::deInit(SpiritIrqs* pxIrqInit) {
	uint8_t tempRegValue[4] = { 0x00, 0x00, 0x00, 0x00 };
	if (pxIrqInit != NULL) {
		uint32_t tempValue = 0x00000000;
		/* Sets the bitfields of passed structure to one */
		*pxIrqInit = (*(SpiritIrqs*) (&tempValue));
	}
	/* Writes the IRQ_MASK registers */
	m_driverSpi->writeRegisters(IRQ_MASK3_BASE, 4, tempRegValue);
}

/* Enables all the IRQs according to the user defined pxIrqInit structure. */
void CSpiritIrq::init(SpiritIrqs* pxIrqInit) {
	uint8_t tempRegValue[4];
	uint8_t* tmpPoint;
	/* Cast the bitfields structure in an array of char using */
	tmpPoint = (uint8_t*) (pxIrqInit);
	for (uint8_t i = 0; i < 4; i++) {
		tempRegValue[3 - i] = tmpPoint[i];
	}
	/* Writes the IRQ_MASK registers */
	m_driverSpi->writeRegisters(IRQ_MASK3_BASE, 4, tempRegValue);
}

/* Enables or disables a specific IRQ. */
void CSpiritIrq::irq(IrqList xIrq, bool xNewState) {
	uint8_t tempRegValue[4];
	uint32_t tempValue = 0;
	/* Reads the IRQ_MASK registers */
	m_driverSpi->readRegisters(IRQ_MASK3_BASE, 4, tempRegValue);
	/* Build the IRQ mask word */
	for (uint8_t i = 0; i < 4; i++) {
		tempValue += ((uint32_t) tempRegValue[i]) << (8 * (3 - i));
	}
	/* Rebuild the new mask according to user request */
	if (!xNewState) {
		tempValue &= (~xIrq);
	} else {
		tempValue |= (xIrq);
	}
	/* Build the array of bytes to write in the IRQ_MASK registers */
	for (uint8_t j = 0; j < 4; j++) {
		tempRegValue[j] = (uint8_t) (tempValue >> (8 * (3 - j)));
	}
	/* Writes the new IRQ mask in the corresponding registers */
	m_driverSpi->writeRegisters(IRQ_MASK3_BASE, 4, tempRegValue);
}

/* Fills a pointer to a structure of SpiritIrqs type reading the IRQ_MASK registers. */
void CSpiritIrq::getMask(SpiritIrqs* pxIrqMask) {
	uint8_t tempRegValue[4];
	uint8_t* pIrqPointer = (uint8_t*) pxIrqMask;
	/* Reads IRQ_MASK registers */
	m_driverSpi->readRegisters(IRQ_MASK3_BASE, 4, tempRegValue);
	/* Build the IRQ mask word */
	for (char i = 0; i < 4; i++) {
		*pIrqPointer = tempRegValue[3 - i];
		pIrqPointer++;
	}
}

/* Filla a pointer to a structure of SpiritIrqs type reading the IRQ_STATUS registers. */
void CSpiritIrq::getStatus(SpiritIrqs* pxIrqStatus) {
	uint8_t tempRegValue[4];
	uint8_t* pIrqPointer = (uint8_t*) pxIrqStatus;
	/* Reads IRQ_STATUS registers */
	m_driverSpi->readRegisters(IRQ_STATUS3_BASE, 4, tempRegValue);
	/* Build the IRQ Status word */
	for (uint8_t i = 0; i < 4; i++) {
		*pIrqPointer = tempRegValue[3 - i];
		pIrqPointer++;
	}
}

/* Clear the IRQ status registers. */
void CSpiritIrq::clearStatus(void) {
	uint8_t tempRegValue[4];
	/* Reads the IRQ_STATUS registers clearing all the flags */
	m_driverSpi->readRegisters(IRQ_STATUS3_BASE, 4, tempRegValue);
}

/*
 * Verifies if a specific IRQ has been generated.
 * The call resets all the IRQ status, so it can't be used in case of multiple raising interrupts.
 */
bool CSpiritIrq::checkFlag(IrqList xFlag) {
	uint8_t tempRegValue[4];
	uint32_t tempValue = 0;
	bool flag;
	/* Reads registers and build the status word */
	m_driverSpi->readRegisters(IRQ_STATUS3_BASE, 4, tempRegValue);
	for (uint8_t i = 0; i < 4; i++) {
		tempValue += ((uint32_t) tempRegValue[i]) << (8 * (3 - i));
	}
	if (tempValue & xFlag) {
		flag = true;
	} else {
		flag = false;
	}
	return flag;
}
