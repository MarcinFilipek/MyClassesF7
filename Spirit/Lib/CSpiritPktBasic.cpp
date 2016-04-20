/*
 * CSpiritPktBasic.cpp
 *
 *  Created on: 20 lis 2015
 *      Author: marcin
 */

#include "CSpiritPktBasic.h"

void CSpiritPktBasic::init(CSpiritInterface* driverSpi, CSpiritPktCommon* pktCommon){
	m_driverSpi = driverSpi;
	m_pktCommon = pktCommon;
}

void CSpiritPktBasic::initPktBasic(PktBasicInit* pxPktBasicInit){
	m_basicInitStruct = pxPktBasicInit;
	uint8_t tempRegValue[4], i;
	/* Reads the PROTOCOL1 register */
	m_driverSpi->readRegisters(PROTOCOL1_BASE, 1, &tempRegValue[0]);
	/* Mask a reserved bit */
	tempRegValue[0] &= ~0x20;
	/* Always set the automatic packet filtering */
	tempRegValue[0] |= PROTOCOL1_AUTO_PCKT_FLT_MASK;
	/* Writes the value on register */
	m_driverSpi->writeRegisters(PROTOCOL1_BASE, 1, &tempRegValue[0]);
	/* Reads the PCKT_FLT_OPTIONS register */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);
	/* Always reset the control and source filtering (also if it is not present in basic) */
	tempRegValue[0] &= ~(PCKT_FLT_OPTIONS_SOURCE_FILTERING_MASK | PCKT_FLT_OPTIONS_CONTROL_FILTERING_MASK);
	/* Writes the value on register */
	m_driverSpi->writeRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);
	if (pxPktBasicInit->xAddressField) {
		tempRegValue[0] = 0x08;
	} else {
		tempRegValue[0] = 0x00;
	}
	/* Address and control length setting */
	tempRegValue[0] |= ((uint8_t) pxPktBasicInit->xControlLength);
	/* Packet format and width length setting */
	pxPktBasicInit->cPktLengthWidth == 0 ? pxPktBasicInit->cPktLengthWidth = 1 : pxPktBasicInit->cPktLengthWidth;
	tempRegValue[1] = ((uint8_t) PCKTCTRL3_PCKT_FRMT_BASIC) | ((uint8_t) (pxPktBasicInit->cPktLengthWidth - 1));
	/* Preamble, sync and fixed or variable length setting */
	tempRegValue[2] = ((uint8_t) pxPktBasicInit->xPreambleLength) | ((uint8_t) pxPktBasicInit->xSyncLength) | ((uint8_t) pxPktBasicInit->xFixVarLength);
	/* CRC length, whitening and FEC setting */
	tempRegValue[3] = (uint8_t) pxPktBasicInit->xCrcMode;
	if (pxPktBasicInit->xDataWhitening) {
		tempRegValue[3] |= PCKTCTRL1_WHIT_MASK;
	}
	if (pxPktBasicInit->xFec) {
		tempRegValue[3] |= PCKTCTRL1_FEC_MASK;
	}

	/* Writes registers */
	m_driverSpi->writeRegisters(PCKTCTRL4_BASE, 4, tempRegValue);
	/* Sync words setting */
	for (i = 0; i < 4; i++) {
		if (i < 3 - (pxPktBasicInit->xSyncLength >> 1)) {
			tempRegValue[i] = 0;
		} else {
			tempRegValue[i] = (uint8_t) (pxPktBasicInit->lSyncWords >> (8 * i));
		}
	}
	/* Sets CRC check bit */
	if (pxPktBasicInit->xCrcMode == CSpiritPktCommon::PKT_NO_CRC) {
		m_pktCommon->setFilterOnCrc(false);
	} else {
		m_pktCommon->setFilterOnCrc(true);
	}
	m_driverSpi->writeRegisters(SYNC4_BASE, 4, tempRegValue);
}

/* Returns the SPIRIT Basic packet structure according to the specified parameters in the registers. */
void CSpiritPktBasic::getInfo(PktBasicInit* pxPktBasicInit) {
	uint8_t tempRegValue[10];
	/* Reads registers */
	m_driverSpi->readRegisters(PCKTCTRL4_BASE, 10, tempRegValue);
	/* Length width */
	pxPktBasicInit->cPktLengthWidth = (tempRegValue[1] & 0x0F) + 1;
	/* Address field */
	pxPktBasicInit->xAddressField = (bool)(
			(tempRegValue[0] >> 3) & 0x01);
	/* Control length */
	pxPktBasicInit->xControlLength = (CSpiritPktCommon::PktControlLength)(
			tempRegValue[0] & 0x07);
	/* CRC mode */
	pxPktBasicInit->xCrcMode = (CSpiritPktCommon::PktCrcMode)(tempRegValue[3] & 0xE0);
	/* Whitening */
	pxPktBasicInit->xDataWhitening = (bool)(
			(tempRegValue[3] >> 4) & 0x01);
	/* FEC */
	pxPktBasicInit->xFec = (bool)(tempRegValue[3] & 0x01);
	/* FIX or VAR bit */
	pxPktBasicInit->xFixVarLength = (CSpiritPktCommon::PktFixVarLength)(tempRegValue[2] & 0x01);
	/* Preamble length */
	pxPktBasicInit->xPreambleLength = (CSpiritPktCommon::PktPreambleLength)(
			tempRegValue[2] & 0xF8);
	/* Sync length */
	pxPktBasicInit->xSyncLength = (CSpiritPktCommon::PktSyncLength)(tempRegValue[2] & 0x06);
	/* sync Words */
	pxPktBasicInit->lSyncWords = 0;
	for (uint8_t i = 0; i < 4; i++) {
		if (i > 2 - (((uint8_t) pxPktBasicInit->xSyncLength) >> 1)) {
			pxPktBasicInit->lSyncWords |= (uint32_t) (tempRegValue[i + 6])
					<< (8 * i);
		}
	}
}

void CSpiritPktBasic::initAddress(PktBasicAddressesInit* pxPktBasicAddresses){
		m_basicAddressInitStruct = pxPktBasicAddresses;
	  uint8_t tempRegValue[3];
	  /* Reads the PCKT_FLT_OPTIONS ragister */
	  m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);
	  /* Enables or disables filtering on my address */
	  if(pxPktBasicAddresses->xFilterOnMyAddress)
	  {
	    tempRegValue[0] |= PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK;
	  }
	  else
	  {
	    tempRegValue[0] &= ~PCKT_FLT_OPTIONS_DEST_VS_TX_ADDR_MASK;
	  }
	  /* Enables or disables filtering on multicast address */
	  if(pxPktBasicAddresses->xFilterOnMulticastAddress)
	  {
	    tempRegValue[0] |= PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK;
	  }
	  else
	  {
	    tempRegValue[0] &= ~PCKT_FLT_OPTIONS_DEST_VS_MULTICAST_ADDR_MASK;
	  }
	  /* Enables or disables filtering on broadcast address */
	  if(pxPktBasicAddresses->xFilterOnBroadcastAddress)
	  {
	    tempRegValue[0] |= PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK;
	  }
	  else
	  {
	    tempRegValue[0] &= ~PCKT_FLT_OPTIONS_DEST_VS_BROADCAST_ADDR_MASK;
	  }
	  /* Writes the new value on the PCKT_FLT_OPTIONS register */
	  m_driverSpi->writeRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);
	  /* Fills the array with the addresses passed in the structure */
	  tempRegValue[0] = pxPktBasicAddresses->cBroadcastAddress;
	  tempRegValue[1] = pxPktBasicAddresses->cMulticastAddress;
	  tempRegValue[2] = pxPktBasicAddresses->cMyAddress;
	  /* Writes values on the PCKT_FLT_GOALS registers */
	  m_driverSpi->writeRegisters(PCKT_FLT_GOALS_BROADCAST_BASE, 3, tempRegValue);
}

/*
 * Returns the SPIRIT Basic packet addresses structure according to the specified
 * parameters in the registers.
 */
void CSpiritPktBasic::getAddressesInfo(PktBasicAddressesInit* pxPktBasicAddresses)
{
  uint8_t tempRegValue[3];

  /* Reads values on the PCKT_FLT_GOALS registers */
  m_driverSpi->readRegisters(PCKT_FLT_GOALS_BROADCAST_BASE, 3, tempRegValue);

  /* Fit the structure with the read addresses */
  pxPktBasicAddresses->cBroadcastAddress = tempRegValue[0];
  pxPktBasicAddresses->cMulticastAddress = tempRegValue[1];
  pxPktBasicAddresses->cMyAddress = tempRegValue[2];

  m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 1, &tempRegValue[0]);

  /* Fit the structure with the read filtering bits */
  pxPktBasicAddresses->xFilterOnBroadcastAddress = (bool)((tempRegValue[0] >> 1) & 0x01);
  pxPktBasicAddresses->xFilterOnMulticastAddress = (bool)((tempRegValue[0] >> 2) & 0x01);
  pxPktBasicAddresses->xFilterOnMyAddress = (bool)((tempRegValue[0] >> 3) & 0x01);
}

/* Configures the Basic packet format as packet used by SPIRIT. */
void CSpiritPktBasic::setFormat(void) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);
	/* Build the new value. Also set to 0 the direct RX mode bits */
	tempRegValue &= 0x0F;
	tempRegValue |= (uint8_t) PCKTCTRL3_PCKT_FRMT_BASIC;
	/* Writes the  value on the PCKTCTRL3 register */
	m_driverSpi->writeRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);
	/* Reads the PCKTCTRL1_BASE register */
	m_driverSpi->readRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);
	/* Build the new value. Set to 0 the direct TX mode bits */
	tempRegValue &= 0xF3;
	/* Writes the value on the PCKTCTRL1 register */
	m_driverSpi->writeRegisters(PCKTCTRL1_BASE, 1, &tempRegValue);
	/* Reads the PROTOCOL1 register */
	m_driverSpi->readRegisters(PROTOCOL1_BASE, 1, &tempRegValue);
	/* Mask a reserved bit */
	tempRegValue &= ~0x20;
	/* Writes the value on register */
	m_driverSpi->writeRegisters(PROTOCOL1_BASE, 1, &tempRegValue);
}

/* Sets the address length for SPIRIT Basic packets. */
void CSpiritPktBasic::setAddressField(bool xAddressField) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL4 register value */
	m_driverSpi->readRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);
	/* Build the address length for the register */
	if (xAddressField) {
		tempRegValue |= 0x08;
	} else {
		tempRegValue &= 0x07;
	}
	/* Writes the new value on the PCKTCTRL4 register */
	m_driverSpi->writeRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);
}


/* Specifies if the Address field for SPIRIT Basic packets is enabled or disabled. */
bool CSpiritPktBasic::getAddressField(void) {
	uint8_t tempRegValue;
	/* Reads the PCKTCTRL4 register value */
	m_driverSpi->readRegisters(PCKTCTRL4_BASE, 1, &tempRegValue);
	/* Returns the address field value */
	if (tempRegValue & PCKTCTRL4_ADDRESS_LEN_MASK) {
		return true;
	} else {
		return false;
	}
}

/*
 * Sets the payload length for SPIRIT Basic packets. Since the packet length
 * depends from the address and the control field size, this
 * function reads the correspondent registers in order to determine
 * the correct packet length to be written.
 */
void CSpiritPktBasic::setPayloadLength(uint16_t nPayloadLength) {
	uint8_t tempRegValue[2];
	uint16_t overSize = 0;
	/* Computes the oversize (address + control) size */
	if (getAddressField()) {
		overSize = 1;
	}
	overSize += (uint16_t) m_pktCommon->getControlLength();
	/* Computes PCKTLEN0 value from nPayloadLength */
	tempRegValue[1] = BUILD_PCKTLEN0(nPayloadLength + overSize);
	/* Computes PCKTLEN1 value from nPayloadLength */
	tempRegValue[0] = BUILD_PCKTLEN1(nPayloadLength + overSize);
	/* Writes data on the PCKTLEN1/0 register */
	m_driverSpi->writeRegisters(PCKTLEN1_BASE, 2, tempRegValue);
}

/*
 * Returns the payload length for SPIRIT Basic packets. Since the
 * packet length depends from the address and the control
 * field size, this function reads the correspondent
 * registers in order to determine the correct payload length
 * to be returned.
 */
uint16_t CSpiritPktBasic::getPayloadLength(void) {
	uint8_t tempRegValue[2];
	uint16_t overSize = 0;
	/* Computes the oversize (address + control) size */
	if (getAddressField()) {
		overSize = 1;
	}
	overSize += (uint16_t) m_pktCommon->getControlLength();
	/* Reads the packet length registers */
	m_driverSpi->readRegisters(PCKTLEN1_BASE, 2, tempRegValue);
	/* Returns the packet length */
	return ((((uint16_t) tempRegValue[0]) << 8) + (uint16_t) tempRegValue[1])
			- overSize;
}

/* Returns the packet length field of the received packet. */
uint16_t CSpiritPktBasic::getReceivedPktLength(void) {
	uint8_t tempRegValue[2];
	uint16_t overSize = 0;
	/* Computes the oversize (address + control) size */
	if (getAddressField()) {
		overSize = 1;
	}
	overSize += (uint16_t) m_pktCommon->getControlLength();
	/* Reads the RX_PCKT_LENx registers value */
	m_driverSpi->readRegisters(RX_PCKT_LEN1_BASE, 2, tempRegValue);
	/* Rebuild and return the the length field */
	return ((((uint16_t) tempRegValue[0]) << 8) + (uint16_t) tempRegValue[1] - overSize);
}

/* Computes and sets the variable payload length for SPIRIT Basic packets. */
void CSpiritPktBasic::setVarLengthWidth(uint16_t nMaxPayloadLength,
		bool xAddressField, CSpiritPktCommon::PktControlLength xControlLength) {
	uint8_t tempRegValue, addressLength, i;
	uint32_t packetLength;
	/* Sets the address length according to xAddressField */
	if (xAddressField) {
		addressLength = 1;
	} else {
		addressLength = 0;
	}
	/* packet length = payload length + address length + control length */
	packetLength = nMaxPayloadLength + addressLength + xControlLength;
	/* Computes the number of bits */
	for (i = 0; i < 16; i++) {
		if (packetLength == 0)
			break;
		{
			packetLength >>= 1;
		}
	}
	i == 0 ? i = 1 : i;
	/* Reads the PCKTCTRL3 register value */
	m_driverSpi->readRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);
	/* Build value for the length width */
	tempRegValue &= ~PCKTCTRL3_LEN_WID_MASK;
	tempRegValue |= (uint8_t) (i - 1);
	/* Writes the PCKTCTRL3 register value */
	m_driverSpi->writeRegisters(PCKTCTRL3_BASE, 1, &tempRegValue);
}
