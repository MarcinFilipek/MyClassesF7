/*
 * CSpiritUtil.cpp
 *
 *  Created on: 9 lis 2015
 *      Author: marcin
 */

#include "CSpiritUtil.h"

const uint32_t CSpiritUtil::XtalFrequency = 50000000;
const CSpiritGeneral::SpiritVersion CSpiritUtil::SpiritVersion = CSpiritGeneral::SPIRIT_VERSION_3_0;

CSpiritUtil::CSpiritUtil(){
	m_rangeExtType = RANGE_EXT_NONE;
	m_xBasicInit={
			CSpiritPktCommon::PKT_PREAMBLE_LENGTH_04BYTES,
			CSpiritPktCommon::PKT_SYNC_LENGTH_4BYTES,
			0x1A2635A8,
			CSpiritPktCommon::PKT_LENGTH_VAR,
			7,
			CSpiritPktCommon::PKT_CRC_MODE_8BITS,
			CSpiritPktCommon::PKT_CONTROL_LENGTH_0BYTES,
			false,
			false,
			true
	};
	m_xAddressInit={
			false,	//en filt my address
			0x34,	//my address
			false,	//en filt muliticast address
			0xEE,	//muliticast address
			false,	//en filt broadcast address
			0xFF	//broadcast address
	};
}

void CSpiritUtil::init(){
	m_interface.pinShutdownInit(GPIOG, GPIO_PIN_6);
	m_interface.initSpiSpirit();
	// <= initEeprom jak jest
	m_spiritGeneral.init(&m_interface);
	m_spiritGpio.init(&m_interface);
	m_spiritCommand.init(&m_interface);
	m_spiritPktCommon.init(&m_interface);
	m_spiritQi.init(&m_interface);
	m_spiritIrq.init(&m_interface);
	m_spiritFifo.init(&m_interface);
	m_spiritTimer.init(&m_interface, XtalFrequency);
	m_spiritManagement.init(&m_interface, &m_spiritCommand);
	m_spiritRadio.init(&m_interface, &m_spiritManagement);
	m_spiritPktBasic.init(&m_interface, &m_spiritPktCommon);
	m_interface.enterShutdown();
	m_interface.exitShutdown();
	managementIdentificationRFBoard();
	/* Initialize the signals to drive the range extender application board */
	managementRangeExtInit();
	/* Micro EXTI config */
	m_interface.pinInterruptInit(GPIOA, GPIO_PIN_15);
	m_interface.interruptCmd(false);
	m_interface.interruptCmd(true);
}

void CSpiritUtil::initGpioIrq(CSpiritGpio::SGpioInit* pGpioIRQ){
	m_spiritGpio.initGpio(pGpioIRQ);
}

void CSpiritUtil::initRadio(CSpiritRadio::SRadioInit* radioInitStruct){
	m_spiritRadio.initRadio(radioInitStruct);
}


#if defined(NO_EEPROM)
/*
 * Identifies the SPIRIT1 Xtal frequency and version.
 */
void CSpiritUtil::managementIdentificationRFBoard(){
	do{
		for(volatile uint16_t i=0; i!=0xFFF; i++);
		m_interface.getSpiritTypes()->refreshStatus();
	}while(m_interface.getSpiritTypes()->getSpiritStatus().MC_STATE != CSpiritTypes::MC_STATE_READY);
	m_spiritRadio.setXtalFrequency(XtalFrequency);
	m_spiritGeneral.setSpiritVersion(SpiritVersion);
}
#endif

void CSpiritUtil::managementRangeExtInit(){
	RangeExtType range_type = getRangeExtType();
	CSpiritGpio::SGpioInit gpioInit;
	if(range_type == RANGE_EXT_SKYWORKS_169){
		/* TCXO optimization power consumption */
		m_spiritGeneral.setExtRef(CSpiritGeneral::MODE_EXT_XIN);
		uint8_t temp = 0x01;
		m_interface.writeRegisters(0xB6, 1, &temp);
	    /* CSD control */
		gpioInit.xSpiritGpioPin = CSpiritGpio::SPIRIT_GPIO_0;
		gpioInit.xSpiritGpioMode = CSpiritGpio::SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP;
		gpioInit.xSpiritGpioIO = CSpiritGpio::SPIRIT_GPIO_DIG_OUT_TX_RX_MODE;
		m_spiritGpio.initGpio(&gpioInit);
		/* CTX/BYP control */
		gpioInit.xSpiritGpioPin = CSpiritGpio::SPIRIT_GPIO_1;
		gpioInit.xSpiritGpioMode = CSpiritGpio::SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP;
		gpioInit.xSpiritGpioIO = CSpiritGpio::SPIRIT_GPIO_DIG_OUT_TX_STATE;
		m_spiritGpio.initGpio(&gpioInit);
		/* Vcont control */
		gpioInit.xSpiritGpioPin = CSpiritGpio::SPIRIT_GPIO_2;
		gpioInit.xSpiritGpioMode = CSpiritGpio::SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP;
		gpioInit.xSpiritGpioIO = CSpiritGpio::SPIRIT_GPIO_DIG_OUT_RX_STATE;
		m_spiritGpio.initGpio(&gpioInit);
	} else if (range_type == RANGE_EXT_SKYWORKS_868){
	    /* CSD control */
		gpioInit.xSpiritGpioPin = CSpiritGpio::SPIRIT_GPIO_0;
		gpioInit.xSpiritGpioMode = CSpiritGpio::SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP;
		gpioInit.xSpiritGpioIO = CSpiritGpio::SPIRIT_GPIO_DIG_OUT_TX_RX_MODE;
		m_spiritGpio.initGpio(&gpioInit);
		/* CTX/BYP control */
		gpioInit.xSpiritGpioPin = CSpiritGpio::SPIRIT_GPIO_1;
		gpioInit.xSpiritGpioMode = CSpiritGpio::SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP;
		gpioInit.xSpiritGpioIO = CSpiritGpio::SPIRIT_GPIO_DIG_OUT_RX_STATE;
		m_spiritGpio.initGpio(&gpioInit);
		/* Vcont control */
		gpioInit.xSpiritGpioPin = CSpiritGpio::SPIRIT_GPIO_2;
		gpioInit.xSpiritGpioMode = CSpiritGpio::SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_HP;
		gpioInit.xSpiritGpioIO = CSpiritGpio::SPIRIT_GPIO_DIG_OUT_TX_STATE;
		m_spiritGpio.initGpio(&gpioInit);
	}
}


void CSpiritUtil::setRangeExtType(RangeExtType extType){
	m_rangeExtType = extType;
}

/* this function sets the radio power */
void CSpiritUtil::setPower(uint8_t cIndex, float fPowerdBm){
	/* Spirit Radio set power */
	m_spiritRadio.setPALeveldBm(cIndex, fPowerdBm);
	m_spiritRadio.setPALevelMaxIndex(cIndex);
}

/* this function sets the packet configuration according to the protocol used */
void CSpiritUtil::packetConfig() {
#if defined(USE_STack_PROTOCOL)
	STackProtocolInit();
#elif defined(USE_BASIC_PROTOCOL)
	m_spiritPktBasic.initPktBasic(&m_xBasicInit);
	m_spiritPktBasic.initAddress(&m_xAddressInit);
#endif
}

/* this function enables SQI check */
void CSpiritUtil::enableSQI(){
	m_spiritQi.setSqiThreshold(CSpiritQi::SQI_TH_0);
	m_spiritQi.sqiCheck(true);
}

/* this function sets the RSSI threshold */
void CSpiritUtil::setRssiTH(int dbmValue){
	m_spiritQi.setRssiThresholddBm(dbmValue);
}

/* this function disable IRQs */
void CSpiritUtil::disableIrq(){
	m_spiritIrq.deInit((CSpiritIrq::SpiritIrqs*)NULL);
}

/*this function enables the Rx IRQ */
void CSpiritUtil::enableRxIrq(){
	m_spiritIrq.irq(CSpiritIrq::RX_DATA_READY, true);
	m_spiritIrq.irq(CSpiritIrq::RX_DATA_DISC, true);
	m_spiritIrq.irq(CSpiritIrq::RX_TIMEOUT, true);
}

void CSpiritUtil::setPayloadLength(uint8_t length) {
#if defined(USE_STack_PROTOCOL)
	/* Payload length config */
	SpiritPktStackSetPayloadLength(length);

#elif defined(USE_BASIC_PROTOCOL)
	/* payload length config */
	m_spiritPktBasic.setPayloadLength(length);
#endif
}

/* this function set the receive timeout period */
void CSpiritUtil::setRxTimeout(float cRxTimeOut) {
	if (cRxTimeOut == 0) {
		/* rx timeout config */
		m_spiritTimer.setRxTimeoutCounter(0);
		m_spiritTimer.setRxTimeoutStopCondition(CSpiritTimer::ANY_ABOVE_THRESHOLD);
	} else {
		/* RX timeout config */
		m_spiritTimer.setRxTimeoutMs(cRxTimeOut);
		enableSQI();
		m_spiritTimer.setRxTimeoutStopCondition(CSpiritTimer::RSSI_AND_SQI_ABOVE_THRESHOLD);
	}
}

/* this function clear the IRQ status */
void CSpiritUtil::clearIRQ(){
	m_spiritIrq.clearStatus();
}

void CSpiritUtil::getStatusIrq(CSpiritIrq::SpiritIrqs* statusIrq){
	m_spiritIrq.getStatus(statusIrq);
}

void CSpiritUtil::flushRXFifo(){
	m_spiritCommand.strobeCommand(CSpiritCommand::CMD_FLUSHRXFIFO);
}
/*
 * this function starts the RX process
 */
void CSpiritUtil::startRX(){
	  if(*m_spiritManagement.getCommunicationState() != COMMUNICATION_STATE_RX)
	  {
	    uint8_t tmp = 0x90;
	    m_interface.writeRegisters(PM_CONFIG1_BASE, 1, &tmp);
	    /* Set SMPS switching frequency */
	    m_spiritRadio.setPACwc(CSpiritRadio::LOAD_0_PF);
	    m_spiritManagement.setCommunicationState(COMMUNICATION_STATE_RX);
	  }
	  m_spiritCommand.strobeCommand(CSpiritCommand::CMD_RX);
}

/*
 * Sends the TX command to SPIRIT. Start to transmit.
 */
void CSpiritUtil::strobeTx() {
	if (*m_spiritManagement.getCommunicationState() != COMMUNICATION_STATE_TX) {
		// xtal_frequency = SpiritRadioGetXtalFrequency();
		CSpiritGeneral::SpiritVersion spirit_version =
				m_spiritGeneral.getSpiritVersion();
		uint32_t desiredFreq = m_spiritManagement.getDesiredFrequency();
		/* To achive the max output power */
		if (desiredFreq >= 150000000 && desiredFreq <= 470000000) {
			/* Optimal setting for Tx mode only */
			m_spiritRadio.setPACwc(CSpiritRadio::LOAD_3_6_PF);
		} else {
			if (spirit_version == CSpiritGeneral::SPIRIT_VERSION_3_0_D1 && desiredFreq >= 863000000 && desiredFreq <= 870000000) {
				/* Optimal setting for Tx mode only */
				m_spiritRadio.setPACwc(CSpiritRadio::LOAD_2_4_PF);
			} else {
				/* Optimal setting for Tx mode only */
				m_spiritRadio.setPACwc(CSpiritRadio::LOAD_0_PF);
			}
		}

		uint8_t tmp = 0x11;
		m_interface.writeRegisters(0xa9, 1, &tmp); /* Enable VCO_L buffer */
		tmp = 0x20;
		m_interface.writeRegisters(PM_CONFIG1_BASE, 1, &tmp); /* Set SMPS switching frequency */
		m_spiritManagement.setCommunicationState(COMMUNICATION_STATE_TX);
	}
	m_spiritCommand.strobeCommand(CSpiritCommand::CMD_TX);
}

/*
 * this function receives the data
 */
void CSpiritUtil::getRxPacket(uint8_t *buffer, uint8_t *cRxData) {
	uint8_t noofbytes = 0;
	/* when rx data ready read the number of received bytes */
	*cRxData = m_spiritFifo.readNumElementsRxFifo();
	noofbytes = *cRxData;
	/* read the RX FIFO */
	m_interface.readFifo(noofbytes, buffer);
	m_spiritCommand.strobeCommand(CSpiritCommand::CMD_FLUSHRXFIFO);
}

/*
 * this function enables the Tx IRQ
 */
void CSpiritUtil::enableTxIrq(void) {
	/* Spirit IRQs enable */
	m_spiritIrq.irq(CSpiritIrq::TX_DATA_SENT, true);
#if defined(USE_STack_PROTOCOL)
	SpiritIrq(MAX_RE_TX_REACH, S_ENABLE);
#endif
}

/*
 * this function sets the destination address
 */
void CSpiritUtil::setDestinationAddress(uint8_t address) {
#if defined(USE_STack_PROTOCOL)
	/* Destination address */
	SpiritPktStackSetDestinationAddress(address);
#elif defined(USE_BASIC_PROTOCOL)
	/* destination address */
	m_spiritPktCommon.setDestinationAddress(address);
#endif
}

void CSpiritUtil::startTX(uint8_t *buffer, uint8_t size, FlagStatus* txDoneFlag) {
	/* fit the TX FIFO */
	m_spiritCommand.strobeCommand(CSpiritCommand::CMD_FLUSHTXFIFO);
	m_interface.writeFifo(size, buffer);
	/* send the TX command */
	strobeTx();
#if 0
	do {
		/* Delay for state transition */
		for(volatile uint8_t i=0; i!=0xFF; i++);

		/* Reads the MC_STATUS register */
		SpiritRefreshStatus();
	}
	while(g_xStatus.MC_STATE!=MC_STATE_TX);
#endif
#if defined(P2P_DEMO)
	/* wait for TX done */
	while (!(*txDoneFlag));
	*txDoneFlag = RESET;
#endif
}
