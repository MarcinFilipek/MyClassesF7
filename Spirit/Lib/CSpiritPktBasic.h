/*
 * CSpiritPktBasic.h
 *
 *  Created on: 20 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITPKTBASIC_H_
#define CSPIRITPKTBASIC_H_

#include "CSpiritPktCommon.h"

class CSpiritPktBasic {
public:
	/*
	 * SPIRIT Basic Packet Init structure definition. This structure allows users to set the main options
	 * for the Basic packet.
	 */
	typedef struct
	{

	  CSpiritPktCommon::PktPreambleLength           xPreambleLength;        /*!< Specifies the preamble length.
	                                                             	 	 	 	 This parameter can be any value of @ref BasicPreambleLength */
	  CSpiritPktCommon::PktSyncLength               xSyncLength;            /*!< Specifies the sync word length. The 32bit word passed (lSyncWords) will be stored in the SYNCx registers from the MSb
	                                                             	 	 	 	 until the number of bytes in xSyncLength has been stored.
	                                                             	 	 	 	 This parameter can be any value of @ref BasicSyncLength */
	  uint32_t                      				lSyncWords;             /*!< Specifies the sync words.
	                                                             	 	 	 	 This parameter is a uint32_t word with format: 0x|SYNC1|SYNC2|SYNC3|SYNC4| */
	  CSpiritPktCommon::PktFixVarLength             xFixVarLength;          /*!< Specifies if a fixed length of packet has to be used.
	                                                             	 	 	 	 This parameter can be any value of @ref BasicFixVarLength */
	  uint8_t                       				cPktLengthWidth;        /*!< Specifies the size of the length of packet in bits. This field is useful only if
	                                                             	 	 	 	 the field xFixVarLength is set to BASIC_LENGTH_VAR. For Basic packets the length width
	                                                             	 	 	 	 is log2( max payload length + control length (0 to 4) + address length (0 or 1)).
	                                                             	 	 	 	 This parameter is an uint8_t */
	  CSpiritPktCommon::PktCrcMode                  xCrcMode;               /*!< Specifies the CRC word length of packet.
	                                                             	 	 	 	 This parameter can be any value of @ref BasicCrcMode */
	  CSpiritPktCommon::PktControlLength            xControlLength;         /*!< Specifies the length of a control field to be sent.
	                                                             	 	 	 	 This parameter can be any value of @ref BasicControlLength */
	  bool         									xAddressField;          /*!< Specifies if the destination address has to be sent.
	                                                             	 	 	 	 This parameter can be S_ENABLE or S_DISABLE */
	  bool         									xFec;                   /*!< Specifies if FEC has to be enabled.
	                                                             	 	 	 	 This parameter can be S_ENABLE or S_DISABLE */
	  bool         									xDataWhitening;         /*!< Specifies if data whitening has to be enabled.
	                                                             	 	 	 	 This parameter can be S_ENABLE or S_DISABLE */
	}PktBasicInit;

	/*
	 * SPIRIT Basic Packet address structure definition. This structure allows users to specify
	 * the node/multicast/broadcast addresses and the correspondent filtering options.
	 */
	typedef struct
	{

	  bool         					xFilterOnMyAddress;             /*!< If set RX packet is accepted if its destination address matches with cMyAddress.
	                                                                     This parameter can be S_ENABLE or S_DISABLE */
	  uint8_t                       cMyAddress;                     /*!< Specifies the TX packet source address (address of this node).
	                                                                     This parameter is an uint8_t */
	  bool         					xFilterOnMulticastAddress;      /*!< If set RX packet is accepted if its destination address matches with cMulticastAddress.
	                                                                     This parameter can be S_ENABLE or S_DISABLE */
	  uint8_t                       cMulticastAddress;              /*!< Specifies the Multicast group address for this node.
	                                                                     This parameter is an uint8_t */
	  bool				         	xFilterOnBroadcastAddress;      /*!< If set RX packet is accepted if its destination address matches with cBroadcastAddress.
	                                                                     This parameter can be S_ENABLE or S_DISABLE */
	  uint8_t                       cBroadcastAddress;              /*!< Specifies the Broadcast address for this node.
	                                                                     This parameter is an uint8_t */
	}PktBasicAddressesInit;

	void init(CSpiritInterface* driverSpi, CSpiritPktCommon* pktCommon);
	void initPktBasic(PktBasicInit* pxPktBasicInit);
	void initAddress(PktBasicAddressesInit* pxPktBasicAddresses);
	void setPayloadLength(uint16_t nPayloadLength);
private:
	CSpiritInterface* m_driverSpi;
	CSpiritPktCommon* m_pktCommon;
	PktBasicInit* m_basicInitStruct;
	PktBasicAddressesInit* m_basicAddressInitStruct;
	void getInfo(PktBasicInit* pxPktBasicInit);
	void getAddressesInfo(PktBasicAddressesInit* pxPktBasicAddresses);
	void setFormat(void);
	void setAddressField(bool xAddressField);
	bool getAddressField(void);
	uint16_t getPayloadLength(void);
	uint16_t getReceivedPktLength(void);
	void setVarLengthWidth(uint16_t nMaxPayloadLength,bool xAddressField, CSpiritPktCommon::PktControlLength xControlLength);
protected:
};

#endif /* CSPIRITPKTBASIC_H_ */
