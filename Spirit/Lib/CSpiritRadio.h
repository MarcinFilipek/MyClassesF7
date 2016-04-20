/*
 * CSpiritRadio.h
 *
 *  Created on: 17 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITRADIO_H_
#define CSPIRITRADIO_H_

#include "stdint.h"
#include "SpiritRegs.h"
#include "../Lib/CSpiritManagement.h"


class CSpiritRadio {
public:
	/* SPIRIT XTAL frequency enumeration */
	typedef enum
	{
		XTAL_FLAG_24_MHz     = 0x00, /*!< 24 MHz Xtal selected */
		XTAL_FLAG_26_MHz     = 0x01  /*!< 26 MHz Xtal selected */
	}XtalFlag;

	/* SPIRIT Modulation enumeration */
	typedef enum
	{
		FSK         = 0x00, /*!< 2-FSK modulation selected */
		GFSK_BT05   = 0x50, /*!< GFSK modulation selected with BT=0.5 */
		GFSK_BT1    = 0x10, /*!< GFSK modulation selected with BT=1 */
		ASK_OOK     = 0x20, /*!< ASK or OOK modulation selected. ASK will use power ramping */
		MSK         = 0x30  /*!< MSK modulation selected */
	}ModulationSelect;

	/* SPIRIT PA additional load capacitors bank enumeration */
	typedef enum
	{
		LOAD_0_PF    = PA_POWER0_CWC_0,    /*!< No additional PA load capacitor */
		LOAD_1_2_PF  = PA_POWER0_CWC_1_2P, /*!< 1.2pF additional PA load capacitor */
		LOAD_2_4_PF  = PA_POWER0_CWC_2_4P, /*!< 2.4pF additional PA load capacitor */
		LOAD_3_6_PF  = PA_POWER0_CWC_3_6P  /*!< 3.6pF additional PA load capacitor */
	}PALoadCapacitor;

	/* SPIRIT AFC Mode selection */
	typedef enum
	{
		AFC_SLICER_CORRECTION  = AFC2_AFC_MODE_SLICER,    /*!< AFC loop closed on slicer */
		AFC_2ND_IF_CORRECTION  = AFC2_AFC_MODE_MIXER      /*!< AFC loop closed on 2nd conversion stage */
	}AFCMode;

	/* SPIRIT AGC Mode selection */
	typedef enum
	{
		AGC_LINEAR_MODE  = AGCCTRL0_AGC_MODE_LINEAR,    /*!< AGC works in linear mode */
		AGC_BINARY_MODE  = AGCCTRL0_AGC_MODE_BINARY     /*!< AGC works in binary mode */
	}AGCMode;

	/* SPIRIT Clock Recovery Mode selection */
	typedef enum
	{
	 	CLK_REC_PLL  = FDEV0_CLOCK_REG_ALGO_SEL_PLL,    /*!< PLL alogrithm for clock recovery */
	 	CLK_REC_DLL  = FDEV0_CLOCK_REG_ALGO_SEL_DLL     /*!< DLL alogrithm for clock recovery */
	}ClkRecMode;

	/* SPIRIT Postfilter length */
	typedef enum
	{
		PSTFLT_LENGTH_8   = 0x00,    /*!< Postfilter length is 8 symbols */
		PSTFLT_LENGTH_16  = 0x10     /*!< Postfilter length is 16 symbols */
	}PstFltLength;

	/* SPIRIT OOK Peak Decay */
	typedef enum
	{
		FAST_DECAY   = 0x00,        /*!< Peak decay control for OOK: fast decay */
		MEDIUM_FAST_DECAY  = 0x01,  /*!< Peak decay control for OOK: medium_fast decay */
		MEDIUM_SLOW_DECAY = 0x02,   /*!< Peak decay control for OOK: medium_fast decay */
		SLOW_DECAY = 0x03           /*!< Peak decay control for OOK: slow decay */
	}OokPeakDecay;

	/* SPIRIT Radio Init structure definition */
	typedef struct
	{
	  int16_t           nXtalOffsetPpm;     /*!< Specifies the offset frequency (in ppm)
	                                             to compensate crystal inaccuracy expressed
	                                             as signed value.*/

	  uint32_t          lFrequencyBase;     /*!< Specifies the base carrier frequency (in Hz),
	                                             i.e. the carrier frequency of channel #0.
	                                             This parameter can be in one of the following ranges:
	                                             High_Band: from 779 MHz to 915 MHz
	                                             Middle Band: from 387 MHz to 470 MHz
	                                             Low Band: from 300 MHz to 348 MHz */
	  uint32_t          nChannelSpace;      /*!< Specifies the channel spacing expressed in Hz.
	                                             The channel spacing is expressed as:
	                                             NxFREQUENCY_STEPS, where FREQUENCY STEPS
	                                             is F_Xo/2^15.
	                                             This parameter can be in the range: [0, F_Xo/2^15*255] Hz */
	  uint8_t           cChannelNumber;      /*!< Specifies the channel number. This value
	                                             is multiplied by the channel spacing and
	                                             added to synthesizer base frequency to
	                                             generate the actual RF carrier frequency */
	  ModulationSelect  xModulationSelect;   /*!< Specifies the modulation. This
	                                             parameter can be any value of
	                                             @ref ModulationSelect */
	  uint32_t          lDatarate;          /*!< Specifies the datarate expressed in bps.
	                                             This parameter can be in the range between
	                                             100 bps and 500 kbps */
	  uint32_t          lFreqDev;           /*!< Specifies the frequency deviation expressed in Hz.
	                                             This parameter can be in the range: [F_Xo*8/2^18, F_Xo*7680/2^18] Hz */
	  uint32_t          lBandwidth;          /*!< Specifies the channel filter bandwidth
	                                             expressed in Hz. This parameter can be
	                                             in the range between 1100 and 800100 Hz */

	}SRadioInit;

	void init(CSpiritInterface* driverSpi, CSpiritManagement* spiritManagement);
	uint8_t initRadio(SRadioInit* radioInitStruct);
	void setXtalFrequency(uint32_t freq);
	uint32_t getXtalFrequency(void);
	static const uint16_t m_vectnBandwidth26M[];
	void setPALeveldBm(uint8_t cIndex, float fPowerdBm);
	void setPALevelMaxIndex(uint8_t cIndex);
	void setPACwc(PALoadCapacitor xCLoad);
	PALoadCapacitor getPACwc(void);
private:
	CSpiritInterface* m_driverSpi;
	CSpiritManagement* m_spiritManagement;
	CSpiritCalibration* m_spiritCalibration;
	void setDigDiv(bool wlacz);
	bool getDigDiv(void);
	void searchDatarateME(uint32_t lDatarate, uint8_t* pcM, uint8_t* pcE);
	void searchFreqCevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE);
	void searchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE);
	void setXtalFlag(XtalFlag xXtal);
	XtalFlag getXtalFlag(void);
	void AFCFreezeOnSync(bool wlacz);
	float getPALeveldBm(uint8_t cIndex);
	uint8_t getPALevelMaxIndex(void);
protected:
};

#endif /* CSPIRITRADIO_H_ */
