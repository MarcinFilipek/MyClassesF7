/*
 * CSpiritUtil.h
 *
 *  Created on: 9 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITUTIL_H_
#define CSPIRITUTIL_H_

#include "../Lib/SpiritConfig.h"

class CSpiritUtil {
public:
	typedef enum
	{
	  RANGE_EXT_NONE = 0x00,
	  RANGE_EXT_SKYWORKS_169,
	  RANGE_EXT_SKYWORKS_868
	} RangeExtType;
	static const uint32_t XtalFrequency;
	static const CSpiritGeneral::SpiritVersion SpiritVersion;

	CSpiritUtil(void);
	void init(void);
	void initGpioIrq(CSpiritGpio::SGpioInit* pGpioIRQ);
	void initRadio(CSpiritRadio::SRadioInit* radioInitStruct);
	void setPower(uint8_t cIndex, float fPowerdBm);
	void setRangeExtType(RangeExtType extType);
	RangeExtType getRangeExtType(void){ return m_rangeExtType; }
	CSpiritCommand* getSpiritCommand(void){ return &m_spiritCommand; }
	void packetConfig(void);
	void enableSQI(void);
	void setRssiTH(int dbmValue);
	void disableIrq(void);
	void enableRxIrq(void);
	void setPayloadLength(uint8_t lendth);
	void setRxTimeout(float cRxTimeOut);
	void clearIRQ(void);
	void startRX(void);
	void getRxPacket(uint8_t *buffer, uint8_t *size );
	void enableTxIrq(void);
	void setDestinationAddress(uint8_t address);
	void startTX(uint8_t *buffer, uint8_t size, FlagStatus* txDoneFlag);
	void getStatusIrq(CSpiritIrq::SpiritIrqs* statusIrq);
	void flushRXFifo(void);
private:
	CSpiritPktBasic::PktBasicInit m_xBasicInit;
	CSpiritPktBasic::PktBasicAddressesInit m_xAddressInit;
	RangeExtType m_rangeExtType;
	CSpiritInterface m_interface;
	CSpiritRadio m_spiritRadio;
	CSpiritGeneral m_spiritGeneral;
	CSpiritGpio m_spiritGpio;
	CSpiritManagement m_spiritManagement;
	CSpiritCommand m_spiritCommand;
	CSpiritPktBasic m_spiritPktBasic;
	CSpiritPktCommon m_spiritPktCommon;
	CSpiritQi m_spiritQi;
	CSpiritIrq m_spiritIrq;
	CSpiritTimer m_spiritTimer;
	CSpiritLinearFiFo m_spiritFifo;
	void managementIdentificationRFBoard(void);
	void managementRangeExtInit(void);
	void strobeTx(void);
protected:
};

#endif /* CSPIRITUTIL_H_ */
