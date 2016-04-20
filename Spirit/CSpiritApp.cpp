/*
 * CSpiritApp.cpp
 *
 *  Created on: 9 lis 2015
 *      Author: marcin
 */

#include "CSpiritApp.h"

CSpiritUtil CSpiritApp::m_spiritDriver;
FlagStatus CSpiritApp::m_rxDoneFlag = RESET;
FlagStatus CSpiritApp::m_txDoneFlag = RESET;
FlagStatus CSpiritApp::m_rxTimeout = RESET;
FlagStatus CSpiritApp::m_keyStatus = RESET;
uint8_t CSpiritApp::m_txFrameBuff[] = {0x00};
CSpiritIrq::SpiritIrqs CSpiritApp::m_irqStatus;

CSpiritApp::CSpiritApp(){
	m_gpioIRQ={CSpiritGpio::SPIRIT_GPIO_3, CSpiritGpio::SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP, CSpiritGpio::SPIRIT_GPIO_DIG_OUT_IRQ};
	m_radioInitStruct = {
			XTAL_OFFSET_PPM,
			BASE_FREQUENCY,
			CHANNEL_SPACE,
			CHANNEL_NUMBER,
			(CSpiritRadio::ModulationSelect)MODULATION_SELECT,
			DATARATE,
			FREQ_DEVIATION,
			BANDWIDTH
	};
	m_cmdFlag = RESET;
}
void CSpiritApp::init(){
	m_ledMainBoard.init(GPIOG, GPIO_PIN_7);
	m_ledShieldSpirit.init(GPIOA, GPIO_PIN_8);
	m_spiritDriver.init();
	initButtonIrq();
	initP2P();
}

void CSpiritApp::initButtonIrq(){
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;/*GPIO_MODE_IT_FALLING;*/
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_SetPriority(EXTI15_10_IRQn, 0x03);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void CSpiritApp::initP2P(){
	m_spiritDriver.initGpioIrq(&m_gpioIRQ);
	m_spiritDriver.initRadio(&m_radioInitStruct);
	m_spiritDriver.setPower(POWER_INDEX, POWER_DBM);
	m_spiritDriver.enableSQI();
	m_spiritDriver.setRssiTH(RSSI_THRESHOLD);
}

/* SPIRIT1 Data Transfer Routine. */
void CSpiritApp::dataCommOn(uint8_t *pTxBuff, uint8_t cTxlen, uint8_t* pRxBuff, uint8_t cRxlen) {
	receiveBuff(pRxBuff, cRxlen);
	if (m_keyStatus) {
		m_keyStatus = RESET;
		xTxFrame.Cmd = LED_TOGGLE;
		xTxFrame.CmdLen = 0x01;
		xTxFrame.Cmdtag = m_txCounter++;
		xTxFrame.CmdType = APPLI_CMD;
		xTxFrame.DataBuff = pTxBuff;
		xTxFrame.DataLen = cTxlen;
		sendBuff(&xTxFrame, xTxFrame.DataLen);
		receiveBuff(pRxBuff, cRxlen);
	}
	if (m_cmdFlag) {
		m_cmdFlag = RESET;
		xTxFrame.Cmd = ACK_OK;
		xTxFrame.CmdLen = 0x01;
		xTxFrame.Cmdtag = xRxFrame.Cmdtag;
		xTxFrame.CmdType = APPLI_CMD;
		xTxFrame.DataBuff = pTxBuff;
		xTxFrame.DataLen = cTxlen;
		sendBuff(&xTxFrame, xTxFrame.DataLen);
		HAL_Delay(DELAY_TX_LED_GLOW);
		m_ledShieldSpirit.setOff();
		m_ledMainBoard.setOff();
	}
}

/*
 * This function handles the point-to-point packet transmission
 */
void CSpiritApp::sendBuff(AppliFrame *xTxFrame, uint8_t cTxlen) {
	uint8_t xIndex = 0;
	uint8_t trxLength = 0;
	m_txFrameBuff[0] = xTxFrame->Cmd;
	m_txFrameBuff[1] = xTxFrame->CmdLen;
	m_txFrameBuff[2] = xTxFrame->Cmdtag;
	m_txFrameBuff[3] = xTxFrame->CmdType;
	m_txFrameBuff[4] = xTxFrame->DataLen;
	for (; xIndex < cTxlen; xIndex++) {
		m_txFrameBuff[xIndex + 5] = xTxFrame->DataBuff[xIndex];
	}
	trxLength = (xIndex + 5);
	/* Spirit IRQs enable */
	m_spiritDriver.disableIrq();
	m_spiritDriver.enableTxIrq();
	/* payload length config */
	m_spiritDriver.setPayloadLength(trxLength);
	/* rx timeout config */
	m_spiritDriver.setRxTimeout(RECEIVE_TIMEOUT);
	/* IRQ registers blanking */
	m_spiritDriver.clearIRQ();
	/* destination address */
	m_spiritDriver.setDestinationAddress(DESTINATION_ADDRESS);
	/* send the TX command */
	m_spiritDriver.startTX(m_txFrameBuff, trxLength, &m_txDoneFlag);
}


/*
 * This function handles the point-to-point packet reception
 */
void CSpiritApp::receiveBuff(uint8_t *RxFrameBuff, uint8_t cRxlen) {
	uint8_t xIndex = 0;
	uint8_t ledToggleCtr = 0;
	m_cmdFlag = RESET;
	m_exitTime = SET;
	m_exitCounter = TIME_TO_EXIT_RX;
	/* Spirit IRQs enable */
	m_spiritDriver.disableIrq();
	m_spiritDriver.enableRxIrq();
	/* payload length config */
	m_spiritDriver.setPayloadLength(PAYLOAD_LEN);
	/* rx timeout config */
	m_spiritDriver.setRxTimeout(RECEIVE_TIMEOUT);
	/* IRQ registers blanking */
	m_spiritDriver.clearIRQ();
	/* RX command */
	m_spiritDriver.startRX();
	/* wait for data received or timeout period occured */
	while ((RESET == m_rxDoneFlag) && (RESET == m_rxTimeout)
			&& (SET == m_exitTime))
		;
	if ((m_rxTimeout == SET) || (m_exitTime == RESET)) {
		m_rxTimeout = RESET;
		m_ledMainBoard.setToggle();
	} else if (m_rxDoneFlag) {
		m_rxDoneFlag = RESET;
		m_spiritDriver.getRxPacket(RxFrameBuff, &cRxlen);
		/*rRSSIValue = Spirit1GetRssiTH();*/
		xRxFrame.Cmd = RxFrameBuff[0];
		xRxFrame.CmdLen = RxFrameBuff[1];
		xRxFrame.Cmdtag = RxFrameBuff[2];
		xRxFrame.CmdType = RxFrameBuff[3];
		xRxFrame.DataLen = RxFrameBuff[4];
		for (xIndex = 5; xIndex < cRxlen; xIndex++) {
			xRxFrame.DataBuff[xIndex] = RxFrameBuff[xIndex];
		}

		if (xRxFrame.Cmd == LED_TOGGLE) {
			m_ledShieldSpirit.setOn();
			m_cmdFlag = SET;
		}
		if (xRxFrame.Cmd == ACK_OK) {
			for (; ledToggleCtr < 5; ledToggleCtr++) {
				m_ledShieldSpirit.setToggle();
				HAL_Delay(DELAY_RX_LED_TOGGLE);
			}
			m_ledShieldSpirit.setOff();
			m_ledMainBoard.setOff();

#if defined(LPM_ENABLE)
#if defined(RF_STANDBY)/*||defined(RF_SLEEP)*/
			wakeupFlag = RESET;
#endif
			Enter_LP_mode();
#endif
		}
	}
}


void CSpiritApp::setKeyStatus(FlagStatus status){
	m_keyStatus = status;
}

void CSpiritApp::interruptHandler() {
	m_spiritDriver.getStatusIrq(&m_irqStatus);
	/* Check the SPIRIT TX_DATA_SENT IRQ flag */
	if (m_irqStatus.IRQ_TX_DATA_SENT || m_irqStatus.IRQ_MAX_RE_TX_REACH) {
		m_txDoneFlag = SET;
	}
	/* Check the SPIRIT RX_DATA_READY IRQ flag */
	else if (m_irqStatus.IRQ_RX_DATA_READY) {
		m_rxDoneFlag = SET;
	}
	/* Check the SPIRIT RX_DATA_DISC IRQ flag */
	else if (m_irqStatus.IRQ_RX_DATA_DISC) {
		/* RX command - to ensure the device will be ready for the next reception */
		if (m_irqStatus.IRQ_RX_TIMEOUT) {
			m_spiritDriver.flushRXFifo();
			m_rxTimeout = SET;
		}
	}
}

void EXTI15_10_IRQHandler(void) {
	/* EXTI line 7 interrupt detected */
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
		CSpiritApp::interruptHandler();
	}

	/* EXTI line 13 interrupt detected */
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
		CSpiritApp::setKeyStatus(SET);
	}
}
