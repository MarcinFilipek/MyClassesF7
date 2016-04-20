/*
 * CSpiritCalibration.h
 *
 *  Created on: 19 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITCALIBRATION_H_
#define CSPIRITCALIBRATION_H_

#include "../Util/CSpiritInterface.h"
class CSpiritCalibration {
public:
	typedef enum {

		CALIB_TIME_7_33_US_24MHZ = 0x00, /*!< calibration window of 7.33 us with XTAL=24MHz */
		CALIB_TIME_14_67_US_24MHZ, /*!< calibration window of 14.67 us with XTAL=24MHz */
		CALIB_TIME_29_33_US_24MHZ, /*!< calibration window of 29.33 us with XTAL=24MHz */
		CALIB_TIME_58_67_US_24MHZ, /*!< calibration window of 58.67 us with XTAL=24MHz */

		CALIB_TIME_6_77_US_26MHZ = 0x00, /*!< calibration window of 6.77 us with XTAL=26MHz */
		CALIB_TIME_13_54_US_26MHZ, /*!< calibration window of 13.54 us with XTAL=26MHz */
		CALIB_TIME_27_08_US_26MHZ, /*!< calibration window of 27.08 us with XTAL=26MHz */
		CALIB_TIME_54_15_US_26MHZ /*!< calibration window of 54.15 us with XTAL=26MHz */

	} VcoWin;

	typedef enum {

		VCO_L = 0x00, /*!< VCO lower */
		VCO_H, /*!< VCO higher */
	} VcoSel;

	void init(CSpiritInterface* driverSpi);
	void setRco(bool wlacz);
	void setVco(bool wlacz);
	void setRcoCalWords(uint8_t cRwt, uint8_t cRfb);
	void getRcoCalWords(uint8_t* pcRwt, uint8_t* pcRfb);
	uint8_t getVcoCalData(void);
	void setVcoCalDataTx(uint8_t cVcoCalData);
	uint8_t getVcoCalDataTx(void);
	void setVcoCalDataRx(uint8_t cVcoCalData);
	uint8_t getVcoCalDataRx(void);
	void setVcoWindow(VcoWin xRefWord);
	VcoWin getVcoWindow(void);
	void selectVco(VcoSel xVco);
	VcoSel getVcoSelecttion(void);
private:
	CSpiritInterface* m_driverSpi;
protected:
};

#endif /* CSPIRITCALIBRATION_H_ */
