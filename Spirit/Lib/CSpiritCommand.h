/*
 * CSpiritCommand.h
 *
 *  Created on: 20 lis 2015
 *      Author: Marcin
 */

#ifndef CSPIRITCOMMAND_H_
#define CSPIRITCOMMAND_H_

#include "SpiritRegs.h"
#include "../Util/CSpiritInterface.h"
class CSpiritCommand {
public:
	typedef enum
	{
	  CMD_TX =  COMMAND_TX,                           /*!< Start to transmit; valid only from READY */
	  CMD_RX =  COMMAND_RX,                           /*!< Start to receive; valid only from READY */
	  CMD_READY =  COMMAND_READY,                     /*!< Go to READY; valid only from STANDBY or SLEEP or LOCK */
	  CMD_STANDBY =  COMMAND_STANDBY,                 /*!< Go to STANDBY; valid only from READY */
	  CMD_SLEEP = COMMAND_SLEEP,                      /*!< Go to SLEEP; valid only from READY */
	  CMD_LOCKRX = COMMAND_LOCKRX,                    /*!< Go to LOCK state by using the RX configuration of the synth; valid only from READY */
	  CMD_LOCKTX = COMMAND_LOCKTX,                    /*!< Go to LOCK state by using the TX configuration of the synth; valid only from READY */
	  CMD_SABORT = COMMAND_SABORT,                    /*!< Force exit form TX or RX states and go to READY state; valid only from TX or RX */
	  CMD_LDC_RELOAD = COMMAND_LDC_RELOAD,            /*!< LDC Mode: Reload the LDC timer with the value stored in the  LDC_PRESCALER / COUNTER  registers; valid from all states  */
	  CMD_SEQUENCE_UPDATE =  COMMAND_SEQUENCE_UPDATE, /*!< Autoretransmission: Reload the Packet sequence counter with the value stored in the PROTOCOL[2] register valid from all states */
	  CMD_AES_ENC = COMMAND_AES_ENC,                  /*!< Commands: Start the encryption routine; valid from all states; valid from all states */
	  CMD_AES_KEY = COMMAND_AES_KEY,                  /*!< Commands: Start the procedure to compute the key for the decryption; valid from all states */
	  CMD_AES_DEC = COMMAND_AES_DEC,                  /*!< Commands: Start the decryption routine using the current key; valid from all states */
	  CMD_AES_KEY_DEC = COMMAND_AES_KEY_DEC,          /*!< Commands: Compute the key and start the decryption; valid from all states */
	  CMD_SRES = COMMAND_SRES,                        /*!< Reset of all digital part, except SPI registers */
	  CMD_FLUSHRXFIFO = COMMAND_FLUSHRXFIFO,          /*!< Clean the RX FIFO; valid from all states */
	  CMD_FLUSHTXFIFO = COMMAND_FLUSHTXFIFO,          /*!< Clean the TX FIFO; valid from all states */
	} SpiritCmd;
	void init(CSpiritInterface* driverSpi);
	void strobeCommand(SpiritCmd commandCode);
private:
	CSpiritInterface* m_driverSpi;
protected:
};

#endif /* CSPIRITCOMMAND_H_ */
