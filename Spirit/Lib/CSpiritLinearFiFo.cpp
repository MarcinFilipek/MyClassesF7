/*
 * CSpiritLinearFiFo.cpp
 *
 *  Created on: 1 gru 2015
 *      Author: Marcin
 */

#include "CSpiritLinearFiFo.h"

void CSpiritLinearFiFo::init(CSpiritInterface* driverSpi){
	m_driverSpi = driverSpi;
}

/*
 *  Returns the number of elements in the Rx FIFO.
 */
uint8_t CSpiritLinearFiFo::readNumElementsRxFifo(void) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(LINEAR_FIFO_STATUS0_BASE, 1, &tempRegValue);
	/* Build and return value */
	return (tempRegValue & 0x7F);
}

/*
 * Returns the number of elements in the Tx FIFO.
 */
uint8_t CSpiritLinearFiFo::readNumElementsTxFifo(void) {
	uint8_t tempRegValue;
	/* Reads the number of elements in TX FIFO and return the value */
	m_driverSpi->readRegisters(LINEAR_FIFO_STATUS1_BASE, 1, &tempRegValue);
	/* Build and return value */
	return (tempRegValue & 0x7F);
}

/*
 * Sets the almost full threshold for the Rx FIFO. When the number of elements in RX FIFO reaches this value an interrupt can be generated to the MCU.
 */
void CSpiritLinearFiFo::setAlmostFullThresholdRx(uint8_t cThrRxFifo) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(FIFO_CONFIG3_RXAFTHR_BASE, 1, &tempRegValue);
	/* Build the register value */
	tempRegValue &= 0x80;
	tempRegValue |= cThrRxFifo;
	/* Writes the Almost Full threshold for RX in the corresponding register */
	m_driverSpi->writeRegisters(FIFO_CONFIG3_RXAFTHR_BASE, 1, &tempRegValue);
}

/*
 * Returns the almost full threshold for RX FIFO.
 */
uint8_t CSpiritLinearFiFo::getAlmostFullThresholdRx(void) {
	uint8_t tempRegValue;
	/* Reads the almost full threshold for RX FIFO and return the value */
	m_driverSpi->readRegisters(FIFO_CONFIG3_RXAFTHR_BASE, 1, &tempRegValue);
	/* Build and return value */
	return (tempRegValue & 0x7F);
}

/*
 * Sets the almost empty threshold for the Rx FIFO. When the number of elements in RX FIFO reaches this value an interrupt can be generated to the MCU.
 */
void CSpiritLinearFiFo::setAlmostEmptyThresholdRx(uint8_t cThrRxFifo) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(FIFO_CONFIG2_RXAETHR_BASE, 1, &tempRegValue);
	/* Build the register value */
	tempRegValue &= 0x80;
	tempRegValue |= cThrRxFifo;
	/* Writes the Almost Empty threshold for RX in the corresponding register */
	m_driverSpi->writeRegisters(FIFO_CONFIG2_RXAETHR_BASE, 1, &tempRegValue);
}


/*
 * Returns the almost empty threshold for Rx FIFO.
 */
uint8_t CSpiritLinearFiFo::getAlmostEmptyThresholdRx(void) {
	uint8_t tempRegValue;
	/* Reads the almost empty threshold for RX FIFO and returns the value */
	m_driverSpi->readRegisters(FIFO_CONFIG2_RXAETHR_BASE, 1, &tempRegValue);
	/* Build and return value */
	return (tempRegValue & 0x7F);
}

/*
 * Sets the almost full threshold for the Tx FIFO. When the number of elements in TX FIFO reaches this value an interrupt can be generated to the MCU.
 */
void CSpiritLinearFiFo::setAlmostFullThresholdTx(uint8_t cThrTxFifo) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(FIFO_CONFIG1_TXAFTHR_BASE, 1, &tempRegValue);
	/* Build the register value */
	tempRegValue &= 0x80;
	tempRegValue |= cThrTxFifo;
	/* Writes the Almost Full threshold for Tx in the corresponding register */
	m_driverSpi->writeRegisters(FIFO_CONFIG1_TXAFTHR_BASE, 1, &tempRegValue);
}

/*
 * Returns the almost full threshold for Tx FIFO.
 */
uint8_t CSpiritLinearFiFo::getAlmostFullThresholdTx(void) {
	uint8_t tempRegValue;
	/* Reads the almost full threshold for Tx FIFO and returns the value */
	m_driverSpi->readRegisters(FIFO_CONFIG1_TXAFTHR_BASE, 1, &tempRegValue);
	/* Build and returns value */
	return (tempRegValue & 0x7F);
}

/*
 * Sets the almost empty threshold for the Tx FIFO. When the number of elements in Tx FIFO reaches this value an interrupt can can be generated to the MCU.
 */
void CSpiritLinearFiFo::setAlmostEmptyThresholdTx(uint8_t cThrTxFifo) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(FIFO_CONFIG0_TXAETHR_BASE, 1, &tempRegValue);
	/* Build the register value */
	tempRegValue &= 0x80;
	tempRegValue |= cThrTxFifo;
	/* Writes the Almost Empty threshold for Tx in the corresponding register */
	m_driverSpi->writeRegisters(FIFO_CONFIG0_TXAETHR_BASE, 1, &tempRegValue);
}

/*
 * Returns the almost empty threshold for Tx FIFO.
 */
uint8_t CSpiritLinearFiFo::getAlmostEmptyThresholdTx(void) {
	uint8_t tempRegValue;
	/* Reads the almost empty threshold for TX FIFO and returns the value */
	m_driverSpi->readRegisters(FIFO_CONFIG0_TXAETHR_BASE, 1, &tempRegValue);
	/* Build and return value */
	return (tempRegValue & 0x7F);
}
