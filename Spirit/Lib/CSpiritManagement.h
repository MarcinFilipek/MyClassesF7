/*
 * CSpiritManagement.h
 *
 *  Created on: 19 lis 2015
 *      Author: Marcin
 */

#ifndef CSPIRITMANAGEMENT_H_
#define CSPIRITMANAGEMENT_H_

#include "CSpiritCalibration.h"
#include "CSpiritCommand.h"

#define COMMUNICATION_STATE_TX          0
#define COMMUNICATION_STATE_RX          1
#define COMMUNICATION_STATE_NONE        2

/* Radio_Band */
#define FBASE_DIVIDER           262144           /*!< 2^18 factor dividing fxo in fbase formula */

#define HIGH_BAND_FACTOR      6       /*!< Band select factor for high band. Factor B in the equation 2 */
#define MIDDLE_BAND_FACTOR    12      /*!< Band select factor for middle band. Factor B in the equation 2 */
#define LOW_BAND_FACTOR       16      /*!< Band select factor for low band. Factor B in the equation 2 */
#define VERY_LOW_BAND_FACTOR  32      /*!< Band select factor for very low band. Factor B in the equation 2 */

#define HIGH_BAND_LOWER_LIMIT         778000000   /*!< Lower limit of the high band: 779 MHz */
#define HIGH_BAND_UPPER_LIMIT         957100000   /*!< Upper limit of the high band: 956 MHz */
#define MIDDLE_BAND_LOWER_LIMIT       386000000   /*!< Lower limit of the middle band: 387 MHz */
#define MIDDLE_BAND_UPPER_LIMIT       471100000   /*!< Upper limit of the middle band: 470 MHz */
#define LOW_BAND_LOWER_LIMIT          299000000   /*!< Lower limit of the low band: 300 MHz */
#define LOW_BAND_UPPER_LIMIT          349100000   /*!< Upper limit of the low band: 348 MHz */
#define VERY_LOW_BAND_LOWER_LIMIT     149000000   /*!< Lower limit of the very low band: 150 MHz */
#define VERY_LOW_BAND_UPPER_LIMIT     175100000   /*!< Upper limit of the very low band: 174 MHz */

#define IS_FREQUENCY_BAND_HIGH(FREQUENCY) ((FREQUENCY)>=HIGH_BAND_LOWER_LIMIT && \
                                           (FREQUENCY)<=HIGH_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND_MIDDLE(FREQUENCY) ((FREQUENCY)>=MIDDLE_BAND_LOWER_LIMIT && \
                                             (FREQUENCY)<=MIDDLE_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND_LOW(FREQUENCY) ((FREQUENCY)>=LOW_BAND_LOWER_LIMIT && \
                                          (FREQUENCY)<=LOW_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND_VERY_LOW(FREQUENCY) ((FREQUENCY)>=VERY_LOW_BAND_LOWER_LIMIT && \
                                          (FREQUENCY)<=VERY_LOW_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND(FREQUENCY) (IS_FREQUENCY_BAND_HIGH(FREQUENCY)|| \
                                      IS_FREQUENCY_BAND_MIDDLE(FREQUENCY)|| \
                                      IS_FREQUENCY_BAND_LOW(FREQUENCY)|| \
                                      IS_FREQUENCY_BAND_VERY_LOW(FREQUENCY))

/* Radio IF Offset */
#define IF_OFFSET_ANA(F_Xo) (lroundf(480140.0/(F_Xo)*12288-64.0))      /*!< It represents the IF_OFFSET_ANA in order
                                                                               to have an intermediate frequency of 480 kHz */

/* Radio FC Offset */
#define F_OFFSET_DIVIDER           262144             /*!< 2^18 factor dividing fxo in foffset formula */
#define PPM_FACTOR                 1000000            /*!< 10^6 factor to use with Xtal_offset_ppm */


#define F_OFFSET_LOWER_LIMIT(F_Xo)			((-(int32_t)F_Xo)/F_OFFSET_DIVIDER*2048)
#define F_OFFSET_UPPER_LIMIT(F_Xo)			((int32_t)(F_Xo/F_OFFSET_DIVIDER*2047))
#define IS_FREQUENCY_OFFSET(OFFSET, F_Xo) (OFFSET>=F_OFFSET_LOWER_LIMIT(F_Xo) && OFFSET<=F_OFFSET_UPPER_LIMIT(F_Xo))

/* Radio Datarate */
#define MINIMUM_DATARATE                 100  /*!< Minimum datarate supported by SPIRIT1 100 bps */
#define MAXIMUM_DATARATE                 510000  /*!< Maximum datarate supported by SPIRIT1 500 kbps */
#define IS_DATARATE(DATARATE)           (DATARATE>=MINIMUM_DATARATE && DATARATE<=MAXIMUM_DATARATE)

/* Radio Frequency Deviation */
#define F_DEV_MANTISSA_UPPER_LIMIT      7  /*!< Maximum value for the mantissa in frequency deviation formula */
#define F_DEV_EXPONENT_UPPER_LIMIT      9  /*!< Maximum value for the exponent in frequency deviation formula */
#define F_DEV_LOWER_LIMIT(F_Xo)		(F_Xo>>16)
#define F_DEV_UPPER_LIMIT(F_Xo)		((F_Xo*15)>>10)
#define IS_F_DEV(FDEV,F_Xo)             (FDEV>=F_DEV_LOWER_LIMIT(F_Xo) && FDEV<=F_DEV_UPPER_LIMIT(F_Xo))

/* Radio Channel Bandwidth */
#define CH_BW_LOWER_LIMIT(F_Xo)      1100*(F_Xo/1000000)/26  /*!< Minimum value of the channel filter bandwidth */
#define CH_BW_UPPER_LIMIT(F_Xo)    800100*(F_Xo/1000000)/26  /*!< Maximum value of the channel filter bandwidth */
#define IS_CH_BW(BW,F_Xo)         ((BW)>=CH_BW_LOWER_LIMIT(F_Xo) && (BW)<=CH_BW_UPPER_LIMIT(F_Xo))

/* Radio Power Amplifier */
#define IS_PA_MAX_INDEX(INDEX)       ((INDEX)<=7)
#define IS_PAPOWER_DBM(PATABLE)      ((PATABLE)>= (-31) && (PATABLE)<=(12))
#define IS_PAPOWER(PATABLE)          ((PATABLE)<=90)
#define IS_PA_STEP_WIDTH(WIDTH)      ((WIDTH)>=1 && (WIDTH)<=4)

/* Radio Automatic Frequency Correction */
#define IS_AFC_FAST_GAIN(GAIN)      	((GAIN)<=15)
#define IS_AFC_SLOW_GAIN(GAIN)      	((GAIN)<=15)
#define IS_AFC_PD_LEAKAGE(LEAKAGE)      ((LEAKAGE)<=31)

/* Radio Automatic Gain Control */
#define AGC_MEASURE_TIME_UPPER_LIMIT_US(F_Xo)			(393216.0/F_Xo)
#define IS_AGC_MEASURE_TIME_US(TIME, F_Xo)              (TIME<=AGC_MEASURE_TIME_UPPER_LIMIT_US(F_Xo))
#define IS_AGC_MEASURE_TIME(TIME)                       (TIME<=15)
#define AGC_HOLD_TIME_UPPER_LIMIT_US(F_Xo)				(756.0/F_Xo)
#define IS_AGC_HOLD_TIME_US(TIME,F_Xo)                  (TIME<=AGC_HOLD_TIME_UPPER_LIMIT_US(F_Xo))
#define IS_AGC_HOLD_TIME(TIME)                          (TIME<=63)
#define IS_AGC_THRESHOLD(THRESHOLD)                     (THRESHOLD<=15)

/* Radio Clock Recovery */
#define IS_CLK_REC_P_GAIN(GAIN)       ((GAIN)<=7)
#define IS_CLK_REC_I_GAIN(GAIN)       ((GAIN)<=15)

/* Radio Private Macros */
#define XTAL_FLAG(xtalFrequency)               xtalFrequency>=25e6 ? XTAL_FLAG_26_MHz:XTAL_FLAG_24_MHz

/* Radio Channel Space */
#define CHSPACE_DIVIDER         32768              /*!< 2^15 factor dividing fxo in channel space formula */
#define IS_CHANNEL_SPACE(CHANNELSPACE, F_Xo)    (CHANNELSPACE<=(F_Xo/32768*255))

class CSpiritManagement {
public:
	/* SPIRIT Band enumeration */
	typedef enum
	{
		HIGH_BAND     = 0x00, /*!< High_Band selected: from 779 MHz to 915 MHz */
		MIDDLE_BAND   = 0x01, /*!< Middle Band selected: from 387 MHz to 470 MHz */
		LOW_BAND      = 0x02,  /*!< Low Band selected: from 300 MHz to 348 MHz */
		VERY_LOW_BAND = 0x03  /*!< Vary low Band selected: from 150 MHz to 174 MHz */
	}BandSelect;

	static const uint16_t m_vectnVCOFreq[];
	static const uint8_t m_vectcBHalfFactor[];
	static const uint8_t m_vectcBandRegValue[];
	void init(CSpiritInterface* driverSpi, CSpiritCommand* spiritCommand);
	void waTRxFcMem(uint32_t desiredFreq);
	CSpiritCalibration* getSpiritCalibration(void){ return &m_spiritCalibration; }
	void setXtalFrequency(uint32_t freq);
	uint32_t getXtalFrequency(void);
	/* Private SpiritManagementSetFrequencyBase function only used in SpiritManagementWaVcoCalibration. */
	void setFrequencyBase(uint32_t lFBase);
	uint8_t setRadioFrequencyBase(uint32_t lFBase);
	uint8_t* getCommunicationState(void) { return &m_communicationState; }
	void setCommunicationState(uint8_t communicationState) { m_communicationState = communicationState;  }
	uint32_t getDesiredFrequency(void){ return m_desiredFrequency; }
private:
	CSpiritInterface* m_driverSpi;
	CSpiritCommand* m_spiritCommand;
	CSpiritCalibration m_spiritCalibration;
	uint32_t m_desiredFrequency;
	static uint8_t m_communicationState;
	uint32_t m_xtalFrequency;
	void setRefDiv(bool wlacz);
	bool getRefDiv(void);
	void setFrequencyOffset(int32_t lFOffset);
	int32_t getFrequencyOffset(void);
	void setChannelSpace(uint32_t fChannelSpace);
	uint32_t getChannelSpace(void);
	void setChannel(uint8_t cChannel);
	uint8_t getChannel(void);
	uint8_t searchWCP(uint32_t lFc);
	uint32_t getFrequencyBase(void);
	void setSynthWord(uint32_t lSynthWord);
	uint32_t getSynthWord(void);
	void setBand(CSpiritManagement::BandSelect xBand);
	CSpiritManagement::BandSelect getBand(void);
	uint8_t waVcoCalibration(void);
protected:
};

#endif /* CSPIRITMANAGEMENT_H_ */
