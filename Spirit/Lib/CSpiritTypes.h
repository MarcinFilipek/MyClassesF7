/*
 * CSpiritTypes.h
 *
 *  Created on: 12 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITTYPES_H_
#define CSPIRITTYPES_H_

#include "../Util/CSpiritSpi.h"
#include "SpiritRegs.h"

class CSpiritTypes {
public:
	/* pirit Flag status. Used to control the state of a flag. */
	typedef enum
	{
	  S_RESET = 0,
	  S_SET = !S_RESET
	} SpiritFlagStatus;
	/* SPIRIT States enumeration. */
	typedef enum {
		MC_STATE_STANDBY = 0x40, 			/*!< STANDBY */
		MC_STATE_SLEEP = 0x36, 				/*!< SLEEP */
		MC_STATE_READY = 0x03, 				/*!< READY */
		MC_STATE_PM_SETUP = 0x3D, 			/*!< PM_SETUP */
		MC_STATE_XO_SETTLING = 0x23, 		/*!< XO_SETTLING */
		MC_STATE_SYNTH_SETUP = 0x53, 		/*!< SYNT_SETUP */
		MC_STATE_PROTOCOL = 0x1F, 			/*!< PROTOCOL */
		MC_STATE_SYNTH_CALIBRATION = 0x4F, 	/*!< SYNTH */
		MC_STATE_LOCK = 0x0F, 				/*!< LOCK */
		MC_STATE_RX = 0x33, 				/*!< RX */
		MC_STATE_TX = 0x5F 					/*!< TX */
	} SpiritState;
	typedef struct {
		uint8_t XO_ON :1; 				/*!< This one bit field notifies if XO is operating (XO_ON is 1) or not (XO_On is 0) */
		SpiritState MC_STATE :7; 		/*!< This 7 bits field indicates the state of the Main Controller of SPIRIT. The possible states
		 	 	 	 	 	 	 	 	 and their corresponding values are defined in SpiritState */
		uint8_t ERROR_LOCK :1; 			/*!< This one bit field notifies if there is an
		 	 	 	 	 	 	 	 	 error on RCO calibration (ERROR_LOCK is 1) or
		 	 	 	 	 	 	 	 	 not (ERROR_LOCK is 0) */
		uint8_t RX_FIFO_EMPTY :1; 		/*!< This one bit field notifies if RX FIFO is empty
		 	 	 	 	 	 	 	 	 (RX_FIFO_EMPTY is 1) or not (RX_FIFO_EMPTY is 0) */
		uint8_t TX_FIFO_FULL :1; 		/*!< This one bit field notifies if TX FIFO is full
		 	 	 	 	 	 	 	 	 (TX_FIFO_FULL is 1) or not (TX_FIFO_FULL is 0) */
		uint8_t ANT_SELECT :1; 			/*!< This one bit field notifies the currently selected antenna */
		uint8_t :4; 					/*!< This 4 bits field are reserved and equal to 5 */

	} SpiritStatus;
	void init(CSpiritSpi *driverSpi);
private:
	SpiritStatus m_status;
	CSpiritSpi *m_driverSpi;
public:
	inline SpiritStatus getSpiritStatus(void){ return m_status; }
	void refreshStatus(void);
};

#endif /* CSPIRITTYPES_H_ */
