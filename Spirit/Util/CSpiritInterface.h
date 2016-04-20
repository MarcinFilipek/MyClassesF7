/*
 * CSpiritInterface.h
 *
 *  Created on: 15 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITINTERFACE_H_
#define CSPIRITINTERFACE_H_

#include "../Lib/CSpiritTypes.h"
#include "../Lib/SpiritRegs.h"

#define S_ABS(a) ((a)>0?(a):-(a))

class CSpiritInterface {
public:
	void initSpiSpirit(void);
	void enterShutdown(void);
	void exitShutdown(void);
	static uint16_t POR_TIME;
	void readRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
	void writeRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);
	void commandStrobes(uint8_t commandCode);
	void writeFifo(uint8_t cNbBytes, uint8_t* pcBuffer);
	void readFifo(uint8_t cNbBytes, uint8_t* pcBuffer);
private:
	GPIO_TypeDef* m_gpioSDN;
	uint16_t m_pinSDN;
	GPIO_TypeDef* m_gpioEXTI;
	uint16_t m_pinEXTI;
	static CSpiritSpi m_spiDriver;
	CSpiritTypes m_spiritTypes;
public:
	static CSpiritSpi* getSpiDriver(void){ return &m_spiDriver; }
	void pinShutdownInit(GPIO_TypeDef* gpio, uint16_t pin);
	void pinInterruptInit(GPIO_TypeDef* gpio, uint16_t pin);
	void interruptCmd(bool wlacz);
	CSpiritTypes* getSpiritTypes(void) { return &m_spiritTypes; }
};

#endif /* CSPIRITINTERFACE_H_ */
