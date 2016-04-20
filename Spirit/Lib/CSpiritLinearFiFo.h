/*
 * CSpiritLinearFiFo.h
 *
 *  Created on: 1 gru 2015
 *      Author: Marcin
 */

#ifndef CSPIRITLINEARFIFO_H_
#define CSPIRITLINEARFIFO_H_

#include "../Util/CSpiritInterface.h"

class CSpiritLinearFiFo {
public:
	void init(CSpiritInterface* driverSpi);
	uint8_t readNumElementsRxFifo(void);
private:
	CSpiritInterface* m_driverSpi;
	uint8_t readNumElementsTxFifo(void);
	void setAlmostFullThresholdRx(uint8_t cThrRxFifo);
	uint8_t getAlmostFullThresholdRx(void);
	void setAlmostEmptyThresholdRx(uint8_t cThrRxFifo);
	uint8_t getAlmostEmptyThresholdRx(void);
	void setAlmostFullThresholdTx(uint8_t cThrTxFifo);
	uint8_t getAlmostFullThresholdTx(void);
	void setAlmostEmptyThresholdTx(uint8_t cThrTxFifo);
	uint8_t getAlmostEmptyThresholdTx(void);
protected:
};

#endif /* CSPIRITLINEARFIFO_H_ */
