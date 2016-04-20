/*
 * CSpiritPktCommon.h
 *
 *  Created on: 21 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITPKTCOMMON_H_
#define CSPIRITPKTCOMMON_H_
#include "../Util/CSpiritInterface.h"

#define IS_PKT_LENGTH_WIDTH_BITS(BITS)                (BITS<=16)
#define IS_PKT_SEQ_NUMBER_RELOAD(SEQN)                (SEQN<=3)
/* Macro used to compute the lower part of the packet length, to write in the PCKTLEN0 register */
#define BUILD_PCKTLEN0(nLength) ((nLength) & 0xFF)
/* Macro used to compute the upper part of the packet length, to write the PCKTLEN1 register */
#define BUILD_PCKTLEN1(nLength) ((nLength) >> 8)

class CSpiritPktCommon {
public:
	/* Preamble length in bytes enumeration. */
	typedef enum {
		PKT_PREAMBLE_LENGTH_01BYTE 	= 0x00, /*!< Preamble length 1 byte*/
		PKT_PREAMBLE_LENGTH_02BYTES = 0x08, /*!< Preamble length 2 bytes */
		PKT_PREAMBLE_LENGTH_03BYTES = 0x10, /*!< Preamble length 3 bytes */
		PKT_PREAMBLE_LENGTH_04BYTES = 0x18, /*!< Preamble length 4 bytes */
		PKT_PREAMBLE_LENGTH_05BYTES = 0x20, /*!< Preamble length 5 bytes */
		PKT_PREAMBLE_LENGTH_06BYTES = 0x28, /*!< Preamble length 6 bytes */
		PKT_PREAMBLE_LENGTH_07BYTES = 0x30, /*!< Preamble length 7 bytes */
		PKT_PREAMBLE_LENGTH_08BYTES = 0x38, /*!< Preamble length 8 bytes */
		PKT_PREAMBLE_LENGTH_09BYTES = 0x40, /*!< Preamble length 9 bytes */
		PKT_PREAMBLE_LENGTH_10BYTES = 0x48, /*!< Preamble length 10 bytes */
		PKT_PREAMBLE_LENGTH_11BYTES = 0x50, /*!< Preamble length 11 bytes */
		PKT_PREAMBLE_LENGTH_12BYTES = 0x58, /*!< Preamble length 12 bytes */
		PKT_PREAMBLE_LENGTH_13BYTES = 0x60, /*!< Preamble length 13 bytes */
		PKT_PREAMBLE_LENGTH_14BYTES = 0x68, /*!< Preamble length 14 bytes */
		PKT_PREAMBLE_LENGTH_15BYTES = 0x70, /*!< Preamble length 15 bytes */
		PKT_PREAMBLE_LENGTH_16BYTES = 0x78, /*!< Preamble length 16 bytes */
		PKT_PREAMBLE_LENGTH_17BYTES = 0x80, /*!< Preamble length 17 bytes */
		PKT_PREAMBLE_LENGTH_18BYTES = 0x88, /*!< Preamble length 18 bytes */
		PKT_PREAMBLE_LENGTH_19BYTES = 0x90, /*!< Preamble length 19 bytes */
		PKT_PREAMBLE_LENGTH_20BYTES = 0x98, /*!< Preamble length 20 bytes */
		PKT_PREAMBLE_LENGTH_21BYTES = 0xA0, /*!< Preamble length 21 bytes */
		PKT_PREAMBLE_LENGTH_22BYTES = 0xA8, /*!< Preamble length 22 bytes */
		PKT_PREAMBLE_LENGTH_23BYTES = 0xB0, /*!< Preamble length 23 bytes */
		PKT_PREAMBLE_LENGTH_24BYTES = 0xB8, /*!< Preamble length 24 bytes */
		PKT_PREAMBLE_LENGTH_25BYTES = 0xC0, /*!< Preamble length 25 bytes */
		PKT_PREAMBLE_LENGTH_26BYTES = 0xC8, /*!< Preamble length 26 bytes */
		PKT_PREAMBLE_LENGTH_27BYTES = 0xD0, /*!< Preamble length 27 bytes */
		PKT_PREAMBLE_LENGTH_28BYTES = 0xD8, /*!< Preamble length 28 bytes */
		PKT_PREAMBLE_LENGTH_29BYTES = 0xE0, /*!< Preamble length 29 bytes */
		PKT_PREAMBLE_LENGTH_30BYTES = 0xE8, /*!< Preamble length 30 bytes */
		PKT_PREAMBLE_LENGTH_31BYTES = 0xF0, /*!< Preamble length 31 bytes */
		PKT_PREAMBLE_LENGTH_32BYTES = 0xF8 /*!< Preamble length 32 bytes */
	} PktPreambleLength;

	/* Sync length in bytes enumeration. */
	typedef enum
	{
	  PKT_SYNC_LENGTH_1BYTE            = 0x00, /*!< Sync length 1 byte*/
	  PKT_SYNC_LENGTH_2BYTES           = 0x02, /*!< Sync length 2 bytes*/
	  PKT_SYNC_LENGTH_3BYTES           = 0x04, /*!< Sync length 3 bytes */
	  PKT_SYNC_LENGTH_4BYTES           = 0x06 , /*!< Sync length 4 bytes */
	}PktSyncLength;

	/* CRC length in bytes enumeration. */
	typedef enum
	{
	  PKT_NO_CRC               = 0x00, /*!< No CRC                              */
	  PKT_CRC_MODE_8BITS       = 0x20, /*!< CRC length 8 bits  - poly: 0x07     */
	  PKT_CRC_MODE_16BITS_1    = 0x40, /*!< CRC length 16 bits - poly: 0x8005   */
	  PKT_CRC_MODE_16BITS_2    = 0x60, /*!< CRC length 16 bits - poly: 0x1021   */
	  PKT_CRC_MODE_24BITS      = 0x80, /*!< CRC length 24 bits - poly: 0x864CFB */
	}PktCrcMode;

	/* Fixed or variable payload length enumeration. */
	typedef enum
	{
	  PKT_LENGTH_FIX  = 0x00,    /*!< Fixed payload length     */
	  PKT_LENGTH_VAR  = 0x01     /*!< Variable payload length  */
	}PktFixVarLength;

	/* Control length in bytes enumeration for SPIRIT packets. */
	typedef enum
	{
	  PKT_CONTROL_LENGTH_0BYTES = 0x00,     /*!< Control length 0 byte*/
	  PKT_CONTROL_LENGTH_1BYTE,             /*!< Control length 1 byte*/
	  PKT_CONTROL_LENGTH_2BYTES,            /*!< Control length 2 bytes*/
	  PKT_CONTROL_LENGTH_3BYTES,            /*!< Control length 3 bytes*/
	  PKT_CONTROL_LENGTH_4BYTES             /*!< Control length 4 bytes*/
	}PktControlLength;

	/* Sync words enumeration for SPIRIT packets. */
	typedef enum
	{
	  PKT_SYNC_WORD_1=0x01,  /*!< Index of the 1st sync word*/
	  PKT_SYNC_WORD_2,       /*!< Index of the 2nd sync word*/
	  PKT_SYNC_WORD_3,       /*!< Index of the 3rd sync word*/
	  PKT_SYNC_WORD_4        /*!< Index of the 4th sync word*/
	}PktSyncX;

	/* Max retransmissions number enumeration for SPIRIT packets. */
	typedef enum
	{
	  PKT_DISABLE_RETX    = 0x00,   /*!< No retrasmissions*/
	  PKT_N_RETX_1        = 0x10,   /*!< Max retrasmissions 1*/
	  PKT_N_RETX_2        = 0x20,   /*!< Max retrasmissions 2*/
	  PKT_N_RETX_3        = 0x30,   /*!< Max retrasmissions 3*/
	  PKT_N_RETX_4        = 0x40,   /*!< Max retrasmissions 4*/
	  PKT_N_RETX_5        = 0x50,   /*!< Max retrasmissions 5*/
	  PKT_N_RETX_6        = 0x60,   /*!< Max retrasmissions 6*/
	  PKT_N_RETX_7        = 0x70,   /*!< Max retrasmissions 7*/
	  PKT_N_RETX_8        = 0x80,   /*!< Max retrasmissions 8*/
	  PKT_N_RETX_9        = 0x90,   /*!< Max retrasmissions 9*/
	  PKT_N_RETX_10       = 0xA0,   /*!< Max retrasmissions 10*/
	  PKT_N_RETX_11       = 0xB0,   /*!< Max retrasmissions 11*/
	  PKT_N_RETX_12       = 0xC0,   /*!< Max retrasmissions 12*/
	  PKT_N_RETX_13       = 0xD0,   /*!< Max retrasmissions 13*/
	  PKT_N_RETX_14       = 0xE0,   /*!< Max retrasmissions 14*/
	  PKT_N_RETX_15       = 0xF0    /*!< Max retrasmissions 15*/
	}PktNMaxReTx;

	void init(CSpiritInterface* driverSpi);
	void setFilterOnCrc(bool wlacz);
	bool getFilterOnCrc(void);
	void setControlLength(PktControlLength xControlLength);
	uint8_t getControlLength(void);
	void setDestinationAddress(uint8_t cAddress);
	uint8_t getTransmittedDestAddress(void);
private:
	CSpiritInterface* m_driverSpi;
	void setPreambleLength(PktPreambleLength xPreambleLength);
	uint8_t getPreambleLength(void);
	void setSyncLength(PktSyncLength xSyncLength);
	uint8_t getSyncLength(void);
	void setFixVarLength(PktFixVarLength xFixVarLength);
	void setCrcMode(PktCrcMode xCrcLength);
	PktCrcMode getCrcMode(void);
	void whitening(bool xNewState);
	void fec(bool xNewState);
	void setSyncxWord(PktSyncX xSyncX,  uint8_t cSyncWord);
	uint8_t getSyncxWord(PktSyncX xSyncX);
	void setSyncWords(uint32_t lSyncWords, PktSyncLength xSyncLength);
	uint32_t getSyncWords(PktSyncLength xSyncLength);
	uint8_t getVarLengthWidth(void);
	void setMyAddress(uint8_t cAddress);
	uint8_t getMyAddress(void);
	void setBroadcastAddress(uint8_t cAddress);
	uint8_t getBroadcastAddress(void);
	void setMulticastAddress(uint8_t cAddress);
	uint8_t getMulticastAddress(void);
	void setCtrlMask(uint32_t lMask);
	uint32_t getCtrlMask(void);
	void setCtrlReference(uint32_t lReference);
	uint32_t getCtrlReference(void);
	void setTransmittedCtrlField(uint32_t lField);
	uint32_t getTransmittedCtrlField(void);
	void filterOnMyAddress(bool xNewState);
	void filterOnMulticastAddress(bool xNewState);
	void filterOnBroadcastAddress(bool xNewState);
	bool getFilterOnMyAddress(void);
	bool getFilterOnMulticastAddress(void);
	bool getFilterOnBroadcastAddress(void);
	uint8_t getReceivedDestAddress(void);
	uint32_t getReceivedCtrlField(void);
	void getReceivedCrcField(uint8_t* cCrcFieldVect);
	void autoAck(bool xAutoAck,bool xPiggybacking);
	void requireAck(bool xRequireAck);
	void setTransmittedSeqNumberReload(uint8_t cSeqNumberReload);
	void setNMaxReTx(PktNMaxReTx xNMaxReTx);
	uint8_t getNMaxReTx(void);
	uint8_t getReceivedSourceAddress(void);
	uint8_t getReceivedSeqNumber(void);
	uint8_t getReceivedNackRx(void);
	uint8_t getTransmittedSeqNumber(void);
	uint8_t getNReTx(void);
	void setFilterOnControlField(bool xNewState);
	bool getFilterOnControlField(void);
protected:
};

#endif /* CSPIRITPKTCOMMON_H_ */
