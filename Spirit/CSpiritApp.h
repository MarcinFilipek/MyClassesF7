/*
 * CSpiritApp.h
 *
 *  Created on: 9 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITAPP_H_
#define CSPIRITAPP_H_

#include "../CLed.h"
#include "Util/CSpiritUtil.h"
#include "RadioConfParam.h"

class CSpiritApp {
public:
	typedef struct
	{
	  uint8_t Cmdtag;
	  uint8_t CmdType;
	  uint8_t CmdLen;
	  uint8_t Cmd;
	  uint8_t DataLen;
	  uint8_t* DataBuff;
	}AppliFrame;

	CSpiritApp(void);
	void init(void);
	void initButtonIrq(void);
	void initP2P(void);
	void dataCommOn(uint8_t *pTxBuff, uint8_t cTxlen, uint8_t* pRxBuff, uint8_t cRxlen);
	static void interruptHandler(void);
	static void setKeyStatus(FlagStatus status);
private:
	Led m_ledMainBoard;
	Led m_ledShieldSpirit;
	static CSpiritUtil m_spiritDriver;
	CSpiritGpio::SGpioInit m_gpioIRQ;
	CSpiritRadio::SRadioInit m_radioInitStruct;
	FlagStatus m_cmdFlag;
	FlagStatus m_exitTime;
	static FlagStatus m_rxDoneFlag;
	static FlagStatus m_txDoneFlag;
	static FlagStatus m_rxTimeout;
	static FlagStatus m_keyStatus;
	uint16_t m_exitCounter;
	uint16_t m_txCounter;
	AppliFrame xTxFrame, xRxFrame;
	static CSpiritIrq::SpiritIrqs m_irqStatus;
	static uint8_t m_txFrameBuff[MAX_BUFFER_LEN];
	void sendBuff(AppliFrame *xTxFrame, uint8_t cTxlen);
	void receiveBuff(uint8_t *RxFrameBuff, uint8_t cRxlen);
protected:
};


#ifdef __cplusplus
 extern "C" {
#endif
void EXTI15_10_IRQHandler(void);
#ifdef __cplusplus
}
#endif

#endif /* CSPIRITAPP_H_ */
