/*
 * CSpiritQi.h
 *
 *  Created on: 25 lis 2015
 *      Author: Marcin
 */

#ifndef CSPIRITQI_H_
#define CSPIRITQI_H_

#include "../Util/CSpiritInterface.h"

class CSpiritQi {
public:
	/* PQI threshold value enumeration. */
	typedef enum {
		PQI_TH_0 = 0x00,
		PQI_TH_1 = 0x04,
		PQI_TH_2 = 0x08,
		PQI_TH_3 = 0x0C,
		PQI_TH_4 = 0x10,
		PQI_TH_5 = 0x14,
		PQI_TH_6 = 0x18,
		PQI_TH_7 = 0x1C,
		PQI_TH_8 = 0x20,
		PQI_TH_9 = 0x24,
		PQI_TH_10 = 0x28,
		PQI_TH_11 = 0x2C,
		PQI_TH_12 = 0x30,
		PQI_TH_13 = 0x34,
		PQI_TH_14 = 0x38,
		PQI_TH_15 = 0x3C
	} PqiThreshold;

	/* SQI threshold value enumeration. */
	typedef enum {
		SQI_TH_0 = 0x00, SQI_TH_1 = 0x40, SQI_TH_2 = 0x80, SQI_TH_3 = 0xC0
	} SqiThreshold;

	/* RSSI filter gain value enumeration. */
	typedef enum {
		RSSI_FG_0 = 0x00,
		RSSI_FG_1 = 0x10,
		RSSI_FG_2 = 0x20,
		RSSI_FG_3 = 0x30,
		RSSI_FG_4 = 0x40,
		RSSI_FG_5 = 0x50,
		RSSI_FG_6 = 0x60,
		RSSI_FG_7 = 0x70,
		RSSI_FG_8 = 0x80,
		RSSI_FG_9 = 0x90,
		RSSI_FG_10 = 0xA0,
		RSSI_FG_11 = 0xB0,
		RSSI_FG_12 = 0xC0,
		RSSI_FG_13 = 0xD0,
		RSSI_FG_14 = 0xE0, /*<! recommended value */
		RSSI_FG_15 = 0xF0
	} RssiFilterGain;

	/* CS mode enumeration. */
	typedef enum {
		CS_MODE_STATIC_3DB = 0x00,
		CS_MODE_DYNAMIC_6DB = 0x04,
		CS_MODE_DYNAMIC_12DB = 0x08,
		CS_MODE_DYNAMIC_18DB = 0x0C
	} CSMode;

	void init(CSpiritInterface* driverSpi);
	void sqiCheck(bool xNewState);
	void setSqiThreshold(SqiThreshold xSqiThr);
	SqiThreshold getSqiThreshold(void);
	void setRssiThresholddBm(int nDbmValue);
private:
	CSpiritInterface* m_driverSpi;
	void pqiCheck(bool xNewState);
	void setPqiThreshold(PqiThreshold xPqiThr);
	PqiThreshold getPqiThreshold(void);
	void setRssiThreshold(uint8_t cRssiThr);
	uint8_t getRssiThreshold(void);
	uint8_t computeRssiThreshold(int cDbmValue);
	uint8_t getPqi(void);
	uint8_t getSqi(void);
	uint8_t getLqi(void);
	CSpiritTypes::SpiritFlagStatus getCs(void);
	uint8_t getRssi(void);
	void setRssiFilterGain(RssiFilterGain xRssiFg);
	RssiFilterGain getRssiFilterGain(void);
	void setCsMode(CSMode xCsMode);
	CSMode getCsMode(void);
	void setCsTimeoutMask(bool xNewState);
	void setPqiTimeoutMask(bool xNewState);
	void setSqiTimeoutMask(bool xNewState);
protected:
};

#endif /* CSPIRITQI_H_ */
