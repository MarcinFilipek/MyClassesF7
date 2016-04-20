/*
 * CSpiritTimer.h
 *
 *  Created on: 27 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITTIMER_H_
#define CSPIRITTIMER_H_

#include "../Util/CSpiritInterface.h"

class CSpiritTimer {
public:
	/* All the possible RX timeout stop conditions enumeration. */
	typedef enum{
	     NO_TIMEOUT_STOP = 0x00,                /*!< Timeout never stopped */
	     TIMEOUT_ALWAYS_STOPPED = 0x08,         /*!< Timeout always stopped (default) */
	     RSSI_ABOVE_THRESHOLD = 0x04,           /*!< Timeout stopped on RSSI above threshold */
	     SQI_ABOVE_THRESHOLD = 0x02,            /*!< Timeout stopped on SQI above threshold */
	     PQI_ABOVE_THRESHOLD = 0x01,            /*!< Timeout stopped on PQI above threshold */
	     RSSI_AND_SQI_ABOVE_THRESHOLD = 0x06,   /*!< Timeout stopped on both RSSI and SQI above threshold */
	     RSSI_AND_PQI_ABOVE_THRESHOLD = 0x05,   /*!< Timeout stopped on both RSSI and PQI above threshold */
	     SQI_AND_PQI_ABOVE_THRESHOLD = 0x03,    /*!< Timeout stopped on both SQI and PQI above threshold */
	     ALL_ABOVE_THRESHOLD = 0x07,            /*!< Timeout stopped only if RSSI, SQI and PQI are above threshold */
	     RSSI_OR_SQI_ABOVE_THRESHOLD = 0x0E,    /*!< Timeout stopped if one between RSSI or SQI are above threshold */
	     RSSI_OR_PQI_ABOVE_THRESHOLD = 0x0D,    /*!< Timeout stopped if one between RSSI or PQI are above threshold */
	     SQI_OR_PQI_ABOVE_THRESHOLD = 0x0B,     /*!< Timeout stopped if one between SQI or PQI are above threshold */
	     ANY_ABOVE_THRESHOLD = 0x0F             /*!< Timeout stopped if one among RSSI, SQI or SQI are above threshold */
	} RxTimeoutStopCondition;
	void init(CSpiritInterface* driverSpi, uint32_t xtalFrequency);
	void setRxTimeoutStopCondition(RxTimeoutStopCondition xStopCondition);
	void setRxTimeoutCounter(uint8_t cCounter);
	void setRxTimeoutMs(float fDesiredMsec);
private:
	uint32_t m_xtalFrequency;
	CSpiritInterface* m_driverSpi;
	void ldcrMode(bool xNewState);
	void ldcrAutoReload(bool xNewState);
	bool ldcrGetAutoReload(void);
	void setRxTimeout(uint8_t cCounter , uint8_t cPrescaler);
	void setRxTimeoutPrescaler(uint8_t cPrescaler);
	void getRxTimeout(float* pfTimeoutMsec, uint8_t* pcCounter , uint8_t* pcPrescaler);
	void setWakeUpTimer(uint8_t cCounter , uint8_t cPrescaler);
	void setWakeUpTimerMs(float fDesiredMsec);
	void setWakeUpTimerCounter(uint8_t cCounter);
	void setWakeUpTimerPrescaler(uint8_t cPrescaler);
	void setWakeUpTimerReloadMs(float fDesiredMsec);
	void getWakeUpTimer(float* pfWakeUpMsec, uint8_t* pcCounter , uint8_t* pcPrescaler);
	void setWakeUpTimerReload(uint8_t cCounter , uint8_t cPrescaler);
	void setWakeUpTimerReloadCounter(uint8_t cCounter);
	void setWakeUpTimerReloadPrescaler(uint8_t cPrescaler);
	void getWakeUpTimerReload(float* pfWakeUpReloadMsec, uint8_t* pcCounter , uint8_t* pcPrescaler);
	void computeWakeUpValues(float fDesiredMsec , uint8_t* pcCounter , uint8_t* pcPrescaler);
	void computeRxTimeoutValues(float fDesiredMsec , uint8_t* pcCounter , uint8_t* pcPrescaler);
	void reloadStrobe(void);
protected:
};

#endif /* CSPIRITTIMER_H_ */
