/*
 * CSpiritIrq.h
 *
 *  Created on: 25 lis 2015
 *      Author: marcin
 */

#ifndef CSPIRITIRQ_H_
#define CSPIRITIRQ_H_

#include "../Util/CSpiritInterface.h"

class CSpiritIrq {
public:
	/*
	 * IRQ bitfield structure for SPIRIT. This structure is used to read or write the single IRQ bit.
	 * During the initialization the user has to fill this structure setting to one the single field related
	 * to the IRQ he wants to enable, and to zero the single field related to all the IRQs he wants to disable.
	 * The same structure can be used to retrieve all the IRQ events from the IRQ registers IRQ_STATUS[3:0],
	 * and read if one or more specific IRQ raised.
	 */
	typedef struct
	{
	  CSpiritTypes::SpiritFlagStatus  IRQ_RX_DATA_READY:1;            /*!< IRQ: RX data ready */
	  CSpiritTypes::SpiritFlagStatus  IRQ_RX_DATA_DISC:1;             /*!< IRQ: RX data discarded (upon filtering) */
	  CSpiritTypes::SpiritFlagStatus  IRQ_TX_DATA_SENT:1;             /*!< IRQ: TX data sent */
	  CSpiritTypes::SpiritFlagStatus  IRQ_MAX_RE_TX_REACH:1;          /*!< IRQ: Max re-TX reached */
	  CSpiritTypes::SpiritFlagStatus  IRQ_CRC_ERROR:1;                /*!< IRQ: CRC error */
	  CSpiritTypes::SpiritFlagStatus  IRQ_TX_FIFO_ERROR:1;            /*!< IRQ: TX FIFO underflow/overflow error */
	  CSpiritTypes::SpiritFlagStatus  IRQ_RX_FIFO_ERROR:1;            /*!< IRQ: RX FIFO underflow/overflow error */
	  CSpiritTypes::SpiritFlagStatus  IRQ_TX_FIFO_ALMOST_FULL:1;      /*!< IRQ: TX FIFO almost full */

	  CSpiritTypes::SpiritFlagStatus  IRQ_TX_FIFO_ALMOST_EMPTY:1;     /*!< IRQ: TX FIFO almost empty */
	  CSpiritTypes::SpiritFlagStatus  IRQ_RX_FIFO_ALMOST_FULL:1;      /*!< IRQ: RX FIFO almost full */
	  CSpiritTypes::SpiritFlagStatus  IRQ_RX_FIFO_ALMOST_EMPTY:1;     /*!< IRQ: RX FIFO almost empty  */
	  CSpiritTypes::SpiritFlagStatus  IRQ_MAX_BO_CCA_REACH:1;         /*!< IRQ: Max number of back-off during CCA */
	  CSpiritTypes::SpiritFlagStatus  IRQ_VALID_PREAMBLE:1;           /*!< IRQ: Valid preamble detected */
	  CSpiritTypes::SpiritFlagStatus  IRQ_VALID_SYNC:1;               /*!< IRQ: Sync word detected */
	  CSpiritTypes::SpiritFlagStatus  IRQ_RSSI_ABOVE_TH:1;            /*!< IRQ: RSSI above threshold */
	  CSpiritTypes::SpiritFlagStatus  IRQ_WKUP_TOUT_LDC:1;            /*!< IRQ: Wake-up timeout in LDC mode */

	  CSpiritTypes::SpiritFlagStatus  IRQ_READY:1;                    /*!< IRQ: READY state */
	  CSpiritTypes::SpiritFlagStatus  IRQ_STANDBY_DELAYED:1;          /*!< IRQ: STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles */
	  CSpiritTypes::SpiritFlagStatus  IRQ_LOW_BATT_LVL:1;             /*!< IRQ: Battery level below threshold*/
	  CSpiritTypes::SpiritFlagStatus  IRQ_POR:1;                      /*!< IRQ: Power On Reset */
	  CSpiritTypes::SpiritFlagStatus  IRQ_BOR:1;                      /*!< IRQ: Brown out event (both accurate and inaccurate)*/
	  CSpiritTypes::SpiritFlagStatus  IRQ_PM_COUNT_EXPIRED:1;         /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
	  CSpiritTypes::SpiritFlagStatus  IRQ_XO_COUNT_EXPIRED:1;         /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */

	  CSpiritTypes::SpiritFlagStatus  IRQ_SYNTH_LOCK_TIMEOUT:1;       /*!< IRQ: only for debug; LOCK state timeout */
	  CSpiritTypes::SpiritFlagStatus  IRQ_SYNTH_LOCK_STARTUP:1;       /*!< IRQ: only for debug; see CALIBR_START_COUNTER */
	  CSpiritTypes::SpiritFlagStatus  IRQ_SYNTH_CAL_TIMEOUT:1;        /*!< IRQ: only for debug; SYNTH calibration timeout */
	  CSpiritTypes::SpiritFlagStatus  IRQ_TX_START_TIME:1;            /*!< IRQ: only for debug; TX circuitry startup time; see TX_START_COUNTER */
	  CSpiritTypes::SpiritFlagStatus  IRQ_RX_START_TIME:1;            /*!< IRQ: only for debug; RX circuitry startup time; see TX_START_COUNTER */
	  CSpiritTypes::SpiritFlagStatus  IRQ_RX_TIMEOUT:1;               /*!< IRQ: RX operation timeout */
	  CSpiritTypes::SpiritFlagStatus  IRQ_AES_END:1;                  /*!< IRQ: AES End of operation */
	  CSpiritTypes::SpiritFlagStatus  :1;                             /*!< Reserved bit */
	} SpiritIrqs;

	/*
	 * IRQ list enumeration for SPIRIT. This enumeration type can be used to address a
	 * specific IRQ.
	 */
	typedef enum
	{
	  RX_DATA_READY = 0x00000001,           /*!< IRQ: RX data ready */
	  RX_DATA_DISC = 0x00000002,            /*!< IRQ: RX data discarded (upon filtering) */
	  TX_DATA_SENT = 0x00000004,            /*!< IRQ: TX data sent */
	  MAX_RE_TX_REACH = 0x00000008,         /*!< IRQ: Max re-TX reached */
	  CRC_ERROR = 0x00000010,               /*!< IRQ: CRC error */
	  TX_FIFO_ERROR = 0x00000020,           /*!< IRQ: TX FIFO underflow/overflow error */
	  RX_FIFO_ERROR = 0x00000040,           /*!< IRQ: RX FIFO underflow/overflow error */
	  TX_FIFO_ALMOST_FULL = 0x00000080,     /*!< IRQ: TX FIFO almost full */
	  TX_FIFO_ALMOST_EMPTY = 0x00000100,    /*!< IRQ: TX FIFO almost empty */
	  RX_FIFO_ALMOST_FULL = 0x00000200,     /*!< IRQ: RX FIFO almost full */
	  RX_FIFO_ALMOST_EMPTY = 0x00000400,    /*!< IRQ: RX FIFO almost empty  */
	  MAX_BO_CCA_REACH = 0x00000800,        /*!< IRQ: Max number of back-off during CCA */
	  VALID_PREAMBLE = 0x00001000,          /*!< IRQ: Valid preamble detected */
	  VALID_SYNC = 0x00002000,              /*!< IRQ: Sync word detected */
	  RSSI_ABOVE_TH = 0x00004000,           /*!< IRQ: RSSI above threshold */
	  WKUP_TOUT_LDC = 0x00008000,           /*!< IRQ: Wake-up timeout in LDC mode */
	  READY = 0x00010000,                   /*!< IRQ: READY state */
	  STANDBY_DELAYED = 0x00020000,         /*!< IRQ: STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles */
	  LOW_BATT_LVL = 0x00040000,            /*!< IRQ: Battery level below threshold*/
	  POR = 0x00080000,                     /*!< IRQ: Power On Reset */
	  BOR = 0x00100000,                     /*!< IRQ: Brown out event (both accurate and inaccurate)*/
	  LOCK = 0x00200000,                    /*!< IRQ: LOCK state */
	  PM_COUNT_EXPIRED = 0x00400000,        /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
	  XO_COUNT_EXPIRED = 0x00800000,        /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */
	  SYNTH_LOCK_TIMEOUT = 0x01000000,      /*!< IRQ: only for debug; LOCK state timeout */
	  SYNTH_LOCK_STARTUP = 0x02000000,      /*!< IRQ: only for debug; see CALIBR_START_COUNTER */
	  SYNTH_CAL_TIMEOUT = 0x04000000,       /*!< IRQ: only for debug; SYNTH calibration timeout */
	  TX_START_TIME = 0x08000000,	        /*!< IRQ: only for debug; TX circuitry startup time; see TX_START_COUNTER */
	  RX_START_TIME = 0x10000000,	        /*!< IRQ: only for debug; RX circuitry startup time; see TX_START_COUNTER */
	  RX_TIMEOUT = 0x20000000,	        /*!< IRQ: RX operation timeout */
	  AES_END = 0x40000000,                 /*!< IRQ: AES End of operation */
	  ALL_IRQ = 0x7FFFFFFF			/*!< All the above mentioned IRQs */

	} IrqList;
	void init(CSpiritInterface* driverSpi);
	void deInit(SpiritIrqs* pxIrqInit);
	void irq(IrqList xIrq, bool xNewState);
	void clearStatus(void);
	void getStatus(SpiritIrqs* pxIrqStatus);
private:
	CSpiritInterface* m_driverSpi;
	void init(SpiritIrqs* pxIrqInit);
	void getMask(SpiritIrqs* pxIrqMask);
	bool checkFlag(IrqList xFlag);
protected:
};

#endif /* CSPIRITIRQ_H_ */
