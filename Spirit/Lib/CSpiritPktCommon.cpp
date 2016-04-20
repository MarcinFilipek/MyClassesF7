/*
 * CSpiritPktCommon.cpp
 *
 *  Created on: 21 lis 2015
 *      Author: marcin
 */

#include "CSpiritPktCommon.h"

void CSpiritPktCommon::init(CSpiritInterface* driverSpi){
	m_driverSpi = driverSpi;
}

/* Sets the CONTROL field length for SPIRIT packets. */
void CSpiritPktCommon::setControlLength(PktControlLength xControlLength) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL4 register value */
	m_driverSpi->readRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);
	/* Set the control length */
	tempRegValue &= ~PCKTCTRL4_CONTROL_LEN_MASK;
	tempRegValue |= (uint8_t) xControlLength;
	/* Writes the new value on the PCKTCTRL4 register */
	m_driverSpi->writeRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);
}

/* Returns the CONTROL field length for SPIRIT packets. */
uint8_t CSpiritPktCommon::getControlLength(void) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL4 register value */
	m_driverSpi->readRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);
	/* Rebuild and return value */
	return (tempRegValue & PCKTCTRL4_CONTROL_LEN_MASK);
}

/* Sets the PREAMBLE field Length mode for SPIRIT packets. */
void CSpiritPktCommon::setPreambleLength(PktPreambleLength xPreambleLength) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL2 register value */
	m_driverSpi->readRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);
	/* Set the preamble length */
	tempRegValue &= ~PCKTCTRL2_PREAMBLE_LENGTH_MASK;
	tempRegValue |= (uint8_t) xPreambleLength;
	/* Writes the new value on the PCKTCTRL2 register */
	m_driverSpi->writeRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);
}

/* Returns the PREAMBLE field Length mode for SPIRIT packets. */
uint8_t CSpiritPktCommon::getPreambleLength(void) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL2 register value */
	m_driverSpi->readRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);
	/* Rebuild and return value */
	return ((tempRegValue & PCKTCTRL2_PREAMBLE_LENGTH_MASK) >> 3) + 1;
}

/* Sets the SYNC field Length for SPIRIT packets. */
void CSpiritPktCommon::setSyncLength(PktSyncLength xSyncLength) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL2 register value */
	m_driverSpi->readRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);
	/* Set the sync length */
	tempRegValue &= ~PCKTCTRL2_SYNC_LENGTH_MASK;
	tempRegValue |= (uint8_t) xSyncLength;
	/* Writes the new value on the PCKTCTRL2 register */
	m_driverSpi->writeRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);
}

/* Returns the SYNC field Length for SPIRIT packets. */
uint8_t CSpiritPktCommon::getSyncLength(void) {
	uint8_t tempRetValue;
	/* Reads the PCKTCTRL2 register value */
	m_driverSpi->readRegisters(PCKTCTRL2_BASE, 1, &tempRetValue);
	/* Rebuild and return value */
	return ((tempRetValue & PCKTCTRL2_SYNC_LENGTH_MASK) >> 1) + 1;
}

/* Sets fixed or variable payload length mode for SPIRIT packets. */
void CSpiritPktCommon::setFixVarLength(PktFixVarLength xFixVarLength) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL2 register value */
	m_driverSpi->readRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);
	/* Set fixed or variable address mode */
	tempRegValue &= ~PCKTCTRL2_FIX_VAR_LEN_MASK;
	tempRegValue |= (uint8_t) xFixVarLength;
	/* Writes the new value on the PCKTCTRL2 register */
	m_driverSpi->writeRegisters(PCKTCTRL2_BASE, 1, &tempRegValue);
}

/* Enables or Disables the filtering on CRC. */
void CSpiritPktCommon::setFilterOnCrc(bool wlacz) {
	uint8_t tempRegValue;
	/* Reads the PCKT_FLT_OPTIONS register value */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
	/* Modify the register value: enable or disable the CRC filtering */
	if (wlacz) {
		tempRegValue |= PCKT_FLT_OPTIONS_CRC_CHECK_MASK;
	} else {
		tempRegValue &= ~PCKT_FLT_OPTIONS_CRC_CHECK_MASK;
	}
	/* Writes the PCKT_FLT_OPTIONS register value */
	m_driverSpi->writeRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
}

/* Returns the CRC filtering enable bit. */
bool CSpiritPktCommon::getFilterOnCrc(void) {
	uint8_t tempRegValue;
	/* Reads the PCKT_FLT_OPTIONS register value */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
	/* Check the CRC filtering bit */
	if (tempRegValue & PCKT_FLT_OPTIONS_CRC_CHECK_MASK) {
		return true;
	} else {
		return false;
	}
}

/* Sets the CRC mode for SPIRIT packets. */
void CSpiritPktCommon::setCrcMode(PktCrcMode xCrcMode) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL1 register value */
	m_driverSpi->readRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);
	/* Build data to write setting the CRC mode */
	tempRegValue &= ~PCKTCTRL1_CRC_MODE_MASK;
	tempRegValue |= (uint8_t) xCrcMode;
	/* Writes the new value on the PCKTCTRL1 register */
	m_driverSpi->writeRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);
}

/* Returns the CRC mode for SPIRIT packets. */
CSpiritPktCommon::PktCrcMode CSpiritPktCommon::getCrcMode(void) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL1 register */
	m_driverSpi->readRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);
	/* Rebuild and return value */
	return (PktCrcMode) (tempRegValue & 0xE0);
}

/* Enables or Disables WHITENING for SPIRIT packets. */
void CSpiritPktCommon::whitening(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL1 register value */
	m_driverSpi->readRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);
	/* Build data to write: set or reset the whitening enable bit */
	if (xNewState) {
		tempRegValue |= PCKTCTRL1_WHIT_MASK;
	} else {
		tempRegValue &= ~PCKTCTRL1_WHIT_MASK;
	}
	/* Writes the new value on the PCKTCTRL1 register */
	m_driverSpi->writeRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);
}

/* Enables or Disables FEC for SPIRIT packets. */
void CSpiritPktCommon::fec(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL1 register value */
	m_driverSpi->readRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);
	/* Build data to write: set or reset the FEC enable bit */
	if (xNewState) {
		tempRegValue |= PCKTCTRL1_FEC_MASK;
	} else {
		tempRegValue &= ~PCKTCTRL1_FEC_MASK;
	}
	/* Writes data on the PCKTCTRL1 register */
	m_driverSpi->writeRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);
}

/* Sets a specific SYNC word for SPIRIT packets. */
void CSpiritPktCommon::setSyncxWord(PktSyncX xSyncX, uint8_t cSyncWord) {
	uint8_t tempRegAddress = 0;
	/* Set the specified address */
	switch (xSyncX) {
	case PKT_SYNC_WORD_1:
		tempRegAddress = SYNC1_BASE;
		break;
	case PKT_SYNC_WORD_2:
		tempRegAddress = SYNC2_BASE;
		break;
	case PKT_SYNC_WORD_3:
		tempRegAddress = SYNC3_BASE;
		break;
	case PKT_SYNC_WORD_4:
		tempRegAddress = SYNC4_BASE;
		break;
	}
	/* Writes value on the selected register */
	m_driverSpi->writeRegisters(tempRegAddress, 1, &cSyncWord);
}

/* Returns a specific SYNC word for SPIRIT packets. */
uint8_t CSpiritPktCommon::getSyncxWord(PktSyncX xSyncX) {
	uint8_t tempRegAddress = 0, tempRegValue = 0;
	/* Set the specified address */
	switch (xSyncX) {
	case PKT_SYNC_WORD_1:
		tempRegAddress = SYNC1_BASE;
		break;
	case PKT_SYNC_WORD_2:
		tempRegAddress = SYNC2_BASE;
		break;
	case PKT_SYNC_WORD_3:
		tempRegAddress = SYNC3_BASE;
		break;
	case PKT_SYNC_WORD_4:
		tempRegAddress = SYNC4_BASE;
		break;
	}
	/* Reads the selected register value */
	m_driverSpi->readRegisters(tempRegAddress, 1, &tempRegValue);
	/* Returns the read value */
	return tempRegValue;
}

/* Sets multiple SYNC words for SPIRIT packets. */
void CSpiritPktCommon::setSyncWords(uint32_t lSyncWords, PktSyncLength xSyncLength) {
	uint8_t tempRegValue[4];
	/* Split the 32-bit value in 4 8-bit values */
	for (uint8_t i = 0; i < 4; i++) {
		if (i < ((3 - xSyncLength) >> 1)) {
			tempRegValue[i] = 0;
		} else {
			tempRegValue[i] = (uint8_t) (lSyncWords >> (8 * i));
		}
	}

	/* Writes SYNC value on the SYNCx registers */
	m_driverSpi->writeRegisters(SYNC4_BASE, 4, tempRegValue);
}


/* Returns multiple SYNC words for SPIRIT packets. */
uint32_t CSpiritPktCommon::getSyncWords(PktSyncLength xSyncLength) {
	uint8_t tempRegValue[4];
	uint32_t tempRetValue = 0;

	/* Reads the SYNCx registers value */
	m_driverSpi->readRegisters(SYNC4_BASE, 4, tempRegValue);
	/* Rebuild the SYNC words */
	for (uint8_t i = 0; i < 4; i++) {
		if (i > 2 - (xSyncLength >> 1)) {
			tempRetValue |= tempRegValue[i] << (8 * i);
		}
	}
	/* Return SYNC words */
	return tempRetValue;
}

/*  Returns the variable length width (in number of bits). */
uint8_t CSpiritPktCommon::getVarLengthWidth(void) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL3 register value */
	m_driverSpi->readRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);
	/* Rebuild and return value */
	return (tempRegValue & PCKTCTRL3_LEN_WID_MASK) + 1;
}

/* Sets the destination address for the Tx packet. */
void CSpiritPktCommon::setDestinationAddress(uint8_t cAddress)
{
  /* Writes value on PCKT_FLT_GOALS_SOURCE_ADDR register */
	m_driverSpi->writeRegisters(PCKT_FLT_GOALS_SOURCE_ADDR_BASE, 1, &cAddress);
}

/* Returns the settled destination address. */
uint8_t CSpiritPktCommon::getTransmittedDestAddress(void) {
	uint8_t tempRegValue;
	/* Reads value on the PCKT_FLT_GOALS_SOURCE_ADDR register */\
	m_driverSpi->readRegisters(PCKT_FLT_GOALS_SOURCE_ADDR_BASE, 1, &tempRegValue);
	/* Return value */
	return tempRegValue;
}

/*
 * Sets the node my address. When the filtering on my address is on, if the destination address extracted from the received packet is equal to the content of the
 * my address, then the packet is accepted (this is the address of the node).
 */
void CSpiritPktCommon::setMyAddress(uint8_t cAddress) {
	/* Writes value on the PCKT_FLT_GOALS_TX_ADDR register */
	m_driverSpi->writeRegisters(PCKT_FLT_GOALS_TX_ADDR_BASE, 1, &cAddress);
}

/* Returns the address of the present node. */
uint8_t CSpiritPktCommon::getMyAddress(void) {
	uint8_t tempRegValue;
	/* Reads value on the PCKT_FLT_GOALS_TX_ADDR register */
	m_driverSpi->readRegisters(PCKT_FLT_GOALS_TX_ADDR_BASE, 1, &tempRegValue);
	/* Return value */
	return tempRegValue;
}

/*
 * Sets the broadcast address. If the destination address extracted from the received packet is equal to the content of the
 * BROADCAST_ADDR register, then the packet is accepted.
 */
void CSpiritPktCommon::setBroadcastAddress(uint8_t cAddress)
{
  /* Writes value on the PCKT_FLT_GOALS_BROADCAST register */
	m_driverSpi->writeRegisters(PCKT_FLT_GOALS_BROADCAST_BASE, 1, &cAddress);
}

/* Returns the broadcast address. */
uint8_t CSpiritPktCommon::getBroadcastAddress(void) {
	uint8_t tempRegValue;
	/* Reads value on the PCKT_FLT_GOALS_BROADCAST register */
	m_driverSpi->readRegisters(PCKT_FLT_GOALS_BROADCAST_BASE, 1, &tempRegValue);
	/* Return value */
	return tempRegValue;
}

/*
 * Sets the multicast address. When the multicast filtering is on, if the destination address extracted from the received packet is equal to the content of the
 * MULTICAST_ADDR register, then the packet is accepted.
 */
void CSpiritPktCommon::setMulticastAddress(uint8_t cAddress) {
	/* Writes value on the PCKT_FLT_GOALS_MULTICAST register */
	m_driverSpi->writeRegisters(PCKT_FLT_GOALS_MULTICAST_BASE, 1, &cAddress);
}

/* Returns the multicast address. */
uint8_t CSpiritPktCommon::getMulticastAddress(void) {
	uint8_t tempRegValue;
	/* Reads value on the PCKT_FLT_GOALS_MULTICAST register */
	m_driverSpi->readRegisters(PCKT_FLT_GOALS_MULTICAST_BASE, 1, &tempRegValue);
	/* Return value */
	return tempRegValue;
}

/*
 * Sets the control mask. The 1 bits of the CONTROL_MASK indicate the
 * bits to be used in filtering. (All 0s no filtering)
 */
void CSpiritPktCommon::setCtrlMask(uint32_t lMask) {
	uint8_t tempRegValue[4];
	/* Split the 32-bit value in 4 8-bit values */
	tempRegValue[0] = (uint8_t) lMask;
	tempRegValue[1] = (uint8_t) (lMask >> 8);
	tempRegValue[2] = (uint8_t) (lMask >> 16);
	tempRegValue[3] = (uint8_t) (lMask >> 24);
	/* Writes values on the CKT_FLT_GOALS_CONTROLx_MASK registers */
	m_driverSpi->writeRegisters(PCKT_FLT_GOALS_CONTROL0_MASK_BASE, 4,
			tempRegValue);
}
/*
 * Returns the control mask. The 1 bits of the CONTROL_MASK indicate the
 * bits to be used in filtering. (All 0s no filtering)
 */
uint32_t CSpiritPktCommon::getCtrlMask(void) {
	uint8_t tempRegValue[4];
	uint32_t tempRetValue = 0;
	/* Reads the PCKT_FLT_GOALS_CONTROLx_MASK registers */
	m_driverSpi->readRegisters(PCKT_FLT_GOALS_CONTROL0_MASK_BASE, 4,
			tempRegValue);
	/* Rebuild the control mask value on a 32-bit integer variable */
	for (uint8_t i = 0; i < 4; i++) {
		tempRetValue |= ((uint32_t) tempRegValue[i]) << (8 * i);
	}
	/* Return value */
	return tempRetValue;
}

/*
 * Sets the control field reference. If the bits enabled by the CONTROL_MASK
 * match the ones of the control fields extracted from the received packet
 * then the packet is accepted.
 */
void CSpiritPktCommon::setCtrlReference(uint32_t lReference) {
	uint8_t tempRegValue[4];
	/* Split the 32-bit value in 4 8-bit values */
	tempRegValue[0] = (uint8_t) lReference;
	tempRegValue[1] = (uint8_t) (lReference >> 8);
	tempRegValue[2] = (uint8_t) (lReference >> 16);
	tempRegValue[3] = (uint8_t) (lReference >> 24);
	/* Writes values on the CKT_FLT_GOALS_CONTROLx_FIELD registers */
	m_driverSpi->writeRegisters(PCKT_FLT_GOALS_CONTROL0_FIELD_BASE, 4,
			tempRegValue);
}

/* Returns the control field reference. */
uint32_t CSpiritPktCommon::getCtrlReference(void) {
	uint8_t tempRegValue[4];
	uint32_t tempRetValue = 0;
	/* Reads the PCKT_FLT_GOALS_CONTROLx_FIELD registers */
	m_driverSpi->readRegisters(PCKT_FLT_GOALS_CONTROL0_FIELD_BASE, 4, tempRegValue);
	/* Rebuild the control mask value on a 32-bit integer variable */
	for (uint8_t i = 0; i < 4; i++) {
		tempRetValue |= ((uint32_t) tempRegValue[i]) << (8 * i);
	}
	/* Return value */
	return tempRetValue;
}

/* Sets the TX control field. */
void CSpiritPktCommon::setTransmittedCtrlField(uint32_t lField) {
	uint8_t tempRegValue[4];
	/* Split the 32-bit value in 4 8-bit values */
	tempRegValue[3] = (uint8_t) lField;
	tempRegValue[2] = (uint8_t) (lField >> 8);
	tempRegValue[1] = (uint8_t) (lField >> 16);
	tempRegValue[0] = (uint8_t) (lField >> 24);
	/* Writes value on the TX_CTRL_FIELDx register */
	m_driverSpi->writeRegisters(TX_CTRL_FIELD3_BASE, 4, tempRegValue);
}

/* Returns the Tx control field. */
uint32_t CSpiritPktCommon::getTransmittedCtrlField(void) {
	uint8_t tempRegValue[4];
	uint32_t tempRetValue = 0;
	/* Reads the TX_CTRL_FIELDx registers */
	m_driverSpi->readRegisters(TX_CTRL_FIELD3_BASE, 4, tempRegValue);
	/* Rebuild value: build a 32-bit value from the read bytes */
	for (uint8_t i = 0; i < 4; i++) {
		tempRetValue |= ((uint32_t) tempRegValue[i]) << (8 * (3 - i));
	}
	/* Return value */
	return tempRetValue;
}

/* If enabled RX packet is accepted if its destination address matches with My address. */
void CSpiritPktCommon::filterOnMyAddress(bool xNewState) {
	uint8_t tempRegValue;
	/* Modify the register value: set or reset the TX source address control */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
	/* Set or reset the DESTINATION vs TX enabling bit */
	if (xNewState) {
		tempRegValue |= PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK;
	} else {
		tempRegValue &= ~PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK;
	}
	/* Writes the new value on the PCKT_FLT_OPTIONS register */
	m_driverSpi->writeRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
}

/* If enabled RX packet is accepted if its destination address matches with multicast address. */
void CSpiritPktCommon::filterOnMulticastAddress(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the PCKT_FLT_OPTIONS register value */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
	/* Enable or disable the filtering option */
	if (xNewState) {
		tempRegValue |= PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK;
	} else {
		tempRegValue &= ~PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK;
	}
	/* Writes the new value on the PCKT_FLT_OPTIONS register */
	m_driverSpi->writeRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
}

/* If enabled RX packet is accepted if its destination address matches with broadcast address. */
void CSpiritPktCommon::filterOnBroadcastAddress(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
	/* Enable or disable the filtering option */
	if (xNewState) {
		tempRegValue |= PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK;
	} else {
		tempRegValue &= ~PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK;
	}
	/* Writes the new value on the PCKT_FLT_OPTIONS register */
	m_driverSpi->writeRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
}

/* Returns the enable bit of the my address filtering. */
bool CSpiritPktCommon::getFilterOnMyAddress(void) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
	/* Gets the enable/disable bit in form of SpiritFunctionalState type */
	if (tempRegValue & 0x08) {
		return true;
	} else {
		return false;
	}
}

/* Returns the enable bit of the multicast address filtering. */
bool CSpiritPktCommon::getFilterOnMulticastAddress(void) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
	/* Get the enable/disable bit in form of SpiritFunctionalState type */
	if (tempRegValue & 0x04) {
		return true;
	} else {
		return false;
	}
}

/* Returns the enable bit of the broadcast address filtering. */
bool CSpiritPktCommon::getFilterOnBroadcastAddress(void) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
	/* Get the enable/disable bit in form of SpiritFunctionalState type */
	if (tempRegValue & 0x02) {
		return true;
	} else {
		return false;
	}
}

/* Returns the destination address of the received packet. */
uint8_t CSpiritPktCommon::getReceivedDestAddress(void) {
	uint8_t tempRegValue;
	/* Reads the RX_ADDR_FIELD0 register value */
	m_driverSpi->readRegisters(RX_ADDR_FIELD0_BASE, 1, &tempRegValue);
	/* Return value */
	return tempRegValue;
}

/* Returns the control field of the received packet. */
uint32_t CSpiritPktCommon::getReceivedCtrlField(void) {
	uint8_t tempRegValue[4];
	uint32_t tempRetValue = 0;
	/* Reads the PCKT_FLT_GOALS_CONTROLx_MASK registers */
	m_driverSpi->readRegisters(RX_CTRL_FIELD0_BASE, 4, tempRegValue);
	/* Rebuild the control mask value on a 32-bit integer variable */
	for (uint8_t i = 0; i < 4; i++) {
		tempRetValue |= ((uint32_t) tempRegValue[i]) << (8 * i);
	}
	/* Returns value */
	return tempRetValue;
}

/* Returns the CRC field of the received packet. */
void CSpiritPktCommon::getReceivedCrcField(uint8_t* cCrcFieldVect) {
	uint8_t tempRegValue[3], crcLength;
	PktCrcMode crcMode;
	/* Gets the CRC mode in PktCrcMode enum */
	crcMode = getCrcMode();
	/* Cast to uint8_t */
	crcLength = (uint8_t) crcMode;
	/* Obtains the real length: see the @ref PktCrcMode enumeration */
	crcLength >>= 5;
	if (crcLength >= 3)
		crcLength--;
	/* Reads the CRC_FIELDx registers value */
	m_driverSpi->readRegisters(CRC_FIELD2_BASE, 3, tempRegValue);
	/* Sets the array to be returned */
	for (uint8_t i = 0; i < 3; i++) {
		if (i < crcLength) {
			cCrcFieldVect[i] = tempRegValue[2 - i];
		} else {
			cCrcFieldVect[i] = 0;
		}
	}
}

/*
 * Sets the AUTO ACKNOLEDGEMENT mechanism on the receiver. When the feature is enabled and
 * a data packet has been correctly received, then an acknowledgement packet is sent back to the originator of the received
 * packet. If the PIGGYBACKING bit is also set, payload data will be read from the FIFO; otherwise an empty packet is sent
 * only containing the source and destination addresses and the sequence number of the packet being acknowledged.
 */
void CSpiritPktCommon::autoAck(bool xAutoAck, bool xPiggybacking) {
	uint8_t tempRegValue[2];
	/* Reads the PROTOCOL[1:0] registers value */
	m_driverSpi->readRegisters(PROTOCOL1_BASE, 2, tempRegValue);
	/* Sets the specified LLP option */
	/* Autoack setting */
	if (xAutoAck) {
		tempRegValue[1] |= PROTOCOL0_AUTO_ACK_MASK;
	} else {
		tempRegValue[1] &= (~PROTOCOL0_AUTO_ACK_MASK);
	}
	/* Piggybacking setting */
	if (xPiggybacking) {
		tempRegValue[0] |= PROTOCOL1_PIGGYBACKING_MASK;
	} else {
		tempRegValue[0] &= (~PROTOCOL1_PIGGYBACKING_MASK);
	}
	/* Writes data on the PROTOCOL[1:0] registers */
	m_driverSpi->writeRegisters(PROTOCOL1_BASE, 2, tempRegValue);
}

/*
 * Sets the AUTO ACKNOLEDGEMENT mechanism on the transmitter. On the transmitter side, the NACK_TX field can be used to require or not an acknowledgment for each individual packet: if
 * NACK_TX is set to "1" then acknowledgment will not be required; if NACK_TX is set to "0" then acknowledgment will be required.
 */
void CSpiritPktCommon::requireAck(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads value on the PROTOCOL0 register */
	m_driverSpi->readRegisters(PROTOCOL0_BASE, 1, &tempRegValue);
	/* Enables or disables the ack requirement option */
	if (xNewState) {
		tempRegValue |= PROTOCOL0_NACK_TX_MASK;
	} else {
		tempRegValue &= ~PROTOCOL0_NACK_TX_MASK;
	}
	/* Writes value on the PROTOCOL0 register */
	m_driverSpi->writeRegisters(PROTOCOL0_BASE, 1, &tempRegValue);
}

/* Sets the TX sequence number to be used to start counting. */
void CSpiritPktCommon::setTransmittedSeqNumberReload(uint8_t cSeqNumberReload) {
	uint8_t tempRegValue;
	/* Reads value on the PROTOCOL2 register */
	m_driverSpi->readRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
	tempRegValue &= 0xE7;
	tempRegValue |= (cSeqNumberReload << 3);
	/* Writes value on the PROTOCOL2 register */
	m_driverSpi->writeRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
}

/* Sets the TX sequence number to be used to start counting. */
void CSpiritPktCommon::setNMaxReTx(PktNMaxReTx xNMaxReTx) {
	uint8_t tempRegValue;
	/* Reads the PROTOCOL0 register value */
	m_driverSpi->readRegisters(PROTOCOL0_BASE, 1, &tempRegValue);
	/* Build the value to be written */
	tempRegValue &= ~PROTOCOL0_NMAX_RETX_MASK;
	tempRegValue |= xNMaxReTx;
	/* Writes value on the PROTOCOL0 register */
	m_driverSpi->writeRegisters(PROTOCOL0_BASE, 1, &tempRegValue);
}

/* Returns the max number of automatic retransmission. */
uint8_t CSpiritPktCommon::getNMaxReTx(void) {
	uint8_t tempRegValue;
	/* Reads the PROTOCOL0 register value */
	m_driverSpi->readRegisters(PROTOCOL0_BASE, 1, &tempRegValue);
	/* Build the value to be written */
	return ((tempRegValue & PROTOCOL0_NMAX_RETX_MASK) >> 4);
}

/* Returns the source address of the received packet. */
uint8_t CSpiritPktCommon::getReceivedSourceAddress(void) {
	uint8_t tempRegValue;
	/* Reads the RX_ADDR_FIELD1 register value */
	m_driverSpi->readRegisters(RX_ADDR_FIELD1_BASE, 1, &tempRegValue);
	/* Returns value */
	return tempRegValue;
}

/* Returns the sequence number of the received packet. */
uint8_t CSpiritPktCommon::getReceivedSeqNumber(void) {
	uint8_t tempRegValue;
	/* Reads the RX_PCKT_INFO register value */
	m_driverSpi->readRegisters(RX_PCKT_INFO_BASE, 1, &tempRegValue);
	/* Obtains and returns the sequence number */
	return tempRegValue & 0x03;
}

/* Returns the Nack bit of the received packet */
uint8_t CSpiritPktCommon::getReceivedNackRx(void) {
	uint8_t tempRegValue;
	/* Reads the RX_PCKT_INFO register value */
	m_driverSpi->readRegisters(RX_PCKT_INFO_BASE, 1, &tempRegValue);
	/* Obtains and returns the RX nack bit */
	return (tempRegValue >> 2) & 0x01;
}

/* Returns the sequence number of the transmitted packet. */
uint8_t CSpiritPktCommon::getTransmittedSeqNumber(void) {
	uint8_t tempRegValue;
	/* Reads the TX_PCKT_INFO register value */
	m_driverSpi->readRegisters(TX_PCKT_INFO_BASE, 1, &tempRegValue);
	/* Obtains and returns the TX sequence number */
	return (tempRegValue >> 4) & 0x07;
}

/* Returns the number of retransmission done on the transmitted packet. */
uint8_t CSpiritPktCommon::getNReTx(void)
{
  uint8_t tempRetValue;
  /* Reads the TX_PCKT_INFO register value */
  m_driverSpi->readRegisters(TX_PCKT_INFO_BASE, 1, &tempRetValue);
  /* Obtains and returns the number of retransmission done */
  return (tempRetValue & 0x0F);
}

/*
 * If enabled RX packet is accepted only if the masked control field matches the
 * masked control field reference (CONTROL_MASK & CONTROL_FIELD_REF == CONTROL_MASK & RX_CONTROL_FIELD).
 */
void CSpiritPktCommon::setFilterOnControlField(bool xNewState) {
	uint8_t tempRegValue;
	/* Modify the register value: set or reset the control bit filtering */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
	/* Set or reset the CONTROL filtering enabling bit */
	if (xNewState) {
		tempRegValue |= PCKT_FLT_OPTIONS_CONTROL_FILTERING_MASK;
	} else {
		tempRegValue &= ~PCKT_FLT_OPTIONS_CONTROL_FILTERING_MASK;
	}
	/* Writes the new value on the PCKT_FLT_OPTIONS register */
	m_driverSpi->writeRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
}

/* Returns the enable bit of the control field filtering. */
bool CSpiritPktCommon::getFilterOnControlField(void) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue);
	/* Gets the enable/disable bit in form of SpiritFunctionalState type */
	if (tempRegValue & PCKT_FLT_OPTIONS_CONTROL_FILTERING_MASK) {
		return true;
	} else {
		return false;
	}
}

