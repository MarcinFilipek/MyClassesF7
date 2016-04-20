/*
 * CSpiritGeneral.h
 *
 *  Created on: 18 lis 2015
 *      Author: Marcin
 */

#ifndef CSPIRITGENERAL_H_
#define CSPIRITGENERAL_H_

#include "../Util/CSpiritInterface.h"
class CSpiritGeneral {
public:
	/* SPIRIT version type enumeration */
	typedef enum
	{
	  SPIRIT_VERSION_2_1 = 0x01,
	  SPIRIT_VERSION_3_0,
	  SPIRIT_VERSION_3_0_D1
	} SpiritVersion;

	/* SPIRIT ModeExtRef enumeration */
	typedef enum
	{
	  MODE_EXT_XO = 0,
	  MODE_EXT_XIN = !MODE_EXT_XO
	} ModeExtRef;

	CSpiritGeneral(void);
	void init(CSpiritInterface* driverSpi);
	void setSpiritVersion(SpiritVersion version);
	SpiritVersion getSpiritVersion(void){ return m_spiritVersion; }
	void setExtRef(ModeExtRef extRef);
private:
	SpiritVersion m_spiritVersion;
	CSpiritInterface* m_driverSpi;
	uint8_t lol;
protected:
};

#endif /* CSPIRITGENERAL_H_ */
