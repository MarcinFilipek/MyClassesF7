/*
 * CSpiritTimer.cpp
 *
 *  Created on: 27 lis 2015
 *      Author: marcin
 */

#include "CSpiritTimer.h"

/* init */
void CSpiritTimer::init(CSpiritInterface* driverSpi, uint32_t xtalFrequency) {
	m_driverSpi = driverSpi;
	m_xtalFrequency = xtalFrequency;
}

/* Enables or Disables the LDCR mode. */
void CSpiritTimer::ldcrMode(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
	/* Mask the read value to enable or disable the LDC mode */
	if (xNewState) {
		tempRegValue |= PROTOCOL2_LDC_MODE_MASK;
	} else {
		tempRegValue &= ~PROTOCOL2_LDC_MODE_MASK;
	}
	/* Writes the register to Enable or Disable the LDCR mode */
	m_driverSpi->writeRegisters(PROTOCOL2_BASE, 1, &tempRegValue);
}

/* Enables or Disables the LDCR timer reloading with the value stored in the LDCR_RELOAD registers. */
void CSpiritTimer::ldcrAutoReload(bool xNewState) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PROTOCOL1_BASE, 1, &tempRegValue);
	/* Mask te read value to enable or disable the reload on sync mode */
	if (xNewState) {
		tempRegValue |= PROTOCOL1_LDC_RELOAD_ON_SYNC_MASK;
	} else {
		tempRegValue &= ~PROTOCOL1_LDC_RELOAD_ON_SYNC_MASK;
	}
	/* Writes the register to Enable or Disable the Auto Reload */
	m_driverSpi->writeRegisters(PROTOCOL1_BASE, 1, &tempRegValue);
}

/* Returns the LDCR timer reload bit. */
bool CSpiritTimer::ldcrGetAutoReload(void) {
	uint8_t tempRegValue;
	/* Reads the register value */
	m_driverSpi->readRegisters(PROTOCOL1_BASE, 1, &tempRegValue);
	return (bool) (tempRegValue & 0x80);
}

/*
 * Sets the RX timeout timer initialization registers with the values of COUNTER and PRESCALER according to the formula: Trx=PRESCALER*COUNTER*Tck.
 * Remember that it is possible to have infinite RX_Timeout writing 0 in the RX_Timeout_Counter and/or RX_Timeout_Prescaler registers.
 */
void CSpiritTimer::setRxTimeout(uint8_t cCounter, uint8_t cPrescaler) {
	uint8_t tempRegValue[2] = { cPrescaler, cCounter };
	/* Writes the prescaler and counter value for RX timeout in the corresponding register */
	m_driverSpi->writeRegisters(TIMERS5_RX_TIMEOUT_PRESCALER_BASE, 2, tempRegValue);
}

/*
 * Sets the RX timeout timer counter and prescaler from the desired value in ms. it is possible to fix the RX_Timeout to
 * a minimum value of 50.417us to a maximum value of about 3.28 s.
 */
void CSpiritTimer::setRxTimeoutMs(float fDesiredMsec)
{
  uint8_t tempRegValue[2];
  /* Computes the counter and prescaler value */
  computeRxTimeoutValues(fDesiredMsec , &tempRegValue[1] , &tempRegValue[0]);
  /* Writes the prescaler and counter value for RX timeout in the corresponding register */
  m_driverSpi->writeRegisters(TIMERS5_RX_TIMEOUT_PRESCALER_BASE, 2, tempRegValue);
}

/* Sets the RX timeout timer counter. If it is equal to 0 the timeout is infinite. */
void CSpiritTimer::setRxTimeoutCounter(uint8_t cCounter) {
	/* Writes the counter value for RX timeout in the corresponding register */
	m_driverSpi->writeRegisters(TIMERS4_RX_TIMEOUT_COUNTER_BASE, 1, &cCounter);
}

/* Sets the RX timeout timer prescaler. If it is equal to 0 the timeout is infinite. */
void CSpiritTimer::setRxTimeoutPrescaler(uint8_t cPrescaler)
{
  /* Writes the prescaler value for RX timeout in the corresponding register */
	m_driverSpi->writeRegisters(TIMERS5_RX_TIMEOUT_PRESCALER_BASE, 1, &cPrescaler);
}

/* Returns the RX timeout timer. */
void CSpiritTimer::getRxTimeout(float* pfTimeoutMsec, uint8_t* pcCounter , uint8_t* pcPrescaler)
{
  uint8_t tempRegValue[2];
  /* Reads the RX timeout registers value */
  m_driverSpi->readRegisters(TIMERS5_RX_TIMEOUT_PRESCALER_BASE, 2, tempRegValue);
  /* Returns values */
  (*pcPrescaler) = tempRegValue[0];
  (*pcCounter) = tempRegValue[1];
  float nXtalFrequency = m_xtalFrequency;
  if(nXtalFrequency>26000000) {
    nXtalFrequency /= 2.0;
  }
  nXtalFrequency /= 1000.0;
  *pfTimeoutMsec = (float)((tempRegValue[0]+1)*tempRegValue[1]*(1210.0/nXtalFrequency));
}

/*
 * Sets the LDCR wake up timer initialization registers with the values of
 * COUNTER and PRESCALER according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where
 * Tck = 28.818 us. The minimum vale of the wakeup timeout is 28.818us (PRESCALER and
 * COUNTER equals to 0) and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals
 * to 255).
 */
void CSpiritTimer::setWakeUpTimer(uint8_t cCounter , uint8_t cPrescaler)
{
  uint8_t tempRegValue[2]={cPrescaler,cCounter};
  /* Writes the counter and prescaler value of wake-up timer in the corresponding register */
  m_driverSpi->writeRegisters(TIMERS3_LDC_PRESCALER_BASE, 2, tempRegValue);
}


/*
 * Sets the LDCR wake up timer counter and prescaler from the desired value in ms,
 * according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 * The minimum vale of the wakeup timeout is 28.818us (PRESCALER and COUNTER equals to 0)
 * and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals to 255).
 */
void CSpiritTimer::setWakeUpTimerMs(float fDesiredMsec)
{
  uint8_t tempRegValue[2];
  /* Computes counter and prescaler */
  computeWakeUpValues(fDesiredMsec , &tempRegValue[1] , &tempRegValue[0]);
  /* Writes the counter and prescaler value of wake-up timer in the corresponding register */
  m_driverSpi->writeRegisters(TIMERS3_LDC_PRESCALER_BASE, 2, tempRegValue);
}

/*
 * Sets the LDCR wake up timer counter. Remember that this value is incresead by one in the Twu calculation.
 * Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 */
void CSpiritTimer::setWakeUpTimerCounter(uint8_t cCounter) {
	/* Writes the counter value for Wake_Up timer in the corresponding register */
	m_driverSpi->writeRegisters(TIMERS2_LDC_COUNTER_BASE, 1, &cCounter);
}

/*
 * Sets the LDCR wake up timer prescaler. Remember that this value is incresead by one in the Twu calculation.
 * Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 */
void CSpiritTimer::setWakeUpTimerPrescaler(uint8_t cPrescaler) {
	/* Writes the prescaler value for Wake_Up timer in the corresponding register */
	m_driverSpi->writeRegisters(TIMERS3_LDC_PRESCALER_BASE, 1, &cPrescaler);
}

/*
 * Sets the LDCR wake up reload timer counter and prescaler from the desired value in ms,
 * according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 * The minimum vale of the wakeup timeout is 28.818us (PRESCALER and COUNTER equals to 0)
 * and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals to 255).
 */
void CSpiritTimer::setWakeUpTimerReloadMs(float fDesiredMsec) {
	uint8_t tempRegValue[2];
	/* Computes counter and prescaler */
	computeWakeUpValues(fDesiredMsec, &tempRegValue[1], &tempRegValue[0]);
	/* Writes the counter and prescaler value of reload wake-up timer in the corresponding register */
	m_driverSpi->writeRegisters(TIMERS1_LDC_RELOAD_PRESCALER_BASE, 2, tempRegValue);
}

/*
 * Returns the LDCR wake up timer, according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 */
void CSpiritTimer::getWakeUpTimer(float* pfWakeUpMsec, uint8_t* pcCounter, uint8_t* pcPrescaler) {
	uint8_t tempRegValue[2];
	/* Reads the Wake_Up timer registers value */
	m_driverSpi->readRegisters(TIMERS3_LDC_PRESCALER_BASE, 2, tempRegValue);
	/* Returns values */
	(*pcPrescaler) = tempRegValue[0];
	(*pcCounter) = tempRegValue[1];
	*pfWakeUpMsec = (float) ((((*pcPrescaler) + 1) * ((*pcCounter) + 1) * (1000.0 / 34.7)));
}

/*
 * Sets the LDCR wake up timer reloading registers with the values of
 * COUNTER and PRESCALER according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where
 * Tck = 28.818 us. The minimum vale of the wakeup timeout is 28.818us (PRESCALER and
 * COUNTER equals to 0) and the maximum value is about 1.89 s (PRESCALER anc COUNTER equals
 * to 255).
 */
void CSpiritTimer::setWakeUpTimerReload(uint8_t cCounter, uint8_t cPrescaler) {
	uint8_t tempRegValue[2] = { cPrescaler, cCounter };
	/* Writes the counter and prescaler value of reload wake-up timer in the corresponding register */
	m_driverSpi->writeRegisters(TIMERS1_LDC_RELOAD_PRESCALER_BASE, 2, tempRegValue);
}


/*
 * Sets the LDCR wake up timer reload counter. Remember that this value is incresead by one in the Twu calculation.
 * Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 */
void CSpiritTimer::setWakeUpTimerReloadCounter(uint8_t cCounter) {
	/* Writes the counter value for reload Wake_Up timer in the corresponding register */
	m_driverSpi->writeRegisters(TIMERS0_LDC_RELOAD_COUNTER_BASE, 1, &cCounter);
}

/*
 * Sets the LDCR wake up timer reload prescaler. Remember that this value is incresead by one in the Twu calculation.
 * Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us
 */
void CSpiritTimer::setWakeUpTimerReloadPrescaler(uint8_t cPrescaler) {
	/* Writes the prescaler value for reload Wake_Up timer in the corresponding register */
	m_driverSpi->writeRegisters(TIMERS1_LDC_RELOAD_PRESCALER_BASE, 1, &cPrescaler);
}


/*
 * Returns the LDCR wake up reload timer, according to the formula: Twu=(PRESCALER +1)*(COUNTER+1)*Tck, where Tck = 28.818 us.
 */
void CSpiritTimer::getWakeUpTimerReload(float* pfWakeUpReloadMsec, uint8_t* pcCounter , uint8_t* pcPrescaler){
	uint8_t tempRegValue[2];
	/* Reads the reload Wake_Up timer registers value */
	m_driverSpi->readRegisters(TIMERS1_LDC_RELOAD_PRESCALER_BASE, 2, tempRegValue);
	/* Returns values */
	(*pcPrescaler) = tempRegValue[0];
	(*pcCounter) = tempRegValue[1];
	*pfWakeUpReloadMsec = (float) ((((*pcPrescaler) + 2) * ((*pcCounter) + 2) * (1000.0 / 34.7)));
}



/*
 * Computes the values of the wakeup timer counter and prescaler from the user time expressed in millisecond.
 * The prescaler and the counter values are computed maintaining the prescaler value as
 * small as possible in order to obtain the best resolution, and in the meantime minimizing the error.
 */
void CSpiritTimer::computeWakeUpValues(float fDesiredMsec, uint8_t* pcCounter, uint8_t* pcPrescaler) {
	uint8_t b0, a0;
	uint32_t n;
	int32_t err, err_min;

	/* If the desired value is over the maximum limit, the counter and the
	 prescaler are settled to their maximum values, and doesn't execute the routine */
	if (fDesiredMsec > 1903.0) {
		*pcCounter = 0xFF;
		*pcPrescaler = 0xFF;
		return;
	} else {
		n = (uint32_t) (fDesiredMsec * 34.7);
		err_min = n;
		/* These are the initial values for the prescaler and the counter, where the prescaler
		 is settled to the minimum value and the counter accordingly. In order to avoid a zero
		 division for the counter the prescaler is increased by one. Then because the wakeup timeout
		 is calculated as: Twu=(PRESCALER +1)*(COUNTER+1)*Tck the counter and the prescaler are decreased by one.*/
		*pcPrescaler = a0 = (n / 0xFF);
		if (a0 == 0)
			*pcCounter = b0 = 0;
		else
			*pcCounter = b0 = (n / *pcPrescaler) - 2;

		/* Iterative cycle to minimize the error */
		for (;; (*pcPrescaler)++) {
			*pcCounter = ((n / (*pcPrescaler + 2)) - 2);
			err =
					(((uint32_t) (*pcPrescaler) + 0) * ((uint32_t) *pcCounter)
							+ 0) - (uint32_t) n;
			if ((uint32_t) S_ABS(err) > (uint32_t) (*pcPrescaler / 2)) {
				(*pcCounter)++;
				err = (((uint32_t) (*pcPrescaler) + 0) * ((uint32_t) *pcCounter)
						+ 0) - (uint32_t) n;
			}
			if (S_ABS(err) < S_ABS(err_min)) {
				err_min = err;
				a0 = *pcPrescaler;
				b0 = *pcCounter;
				if (err == 0) {
					break;
				}
			}
			if (*pcPrescaler == 0xFF) {
				break;
			}
		}
		if (a0 == 0)
			a0 = 1;
		if (b0 == 0 || b0 == 1)
			b0 = 2;

		*pcPrescaler = a0;
		*pcCounter = b0 - 1;
	}
}
/*
 * Computes the values of the rx_timeout timer counter and prescaler from the user time expressed in millisecond.
 * The prescaler and the counter values are computed maintaining the prescaler value as
 * small as possible in order to obtain the best resolution, and in the meantime minimizing the error.
 */
void CSpiritTimer::computeRxTimeoutValues(float fDesiredMsec,uint8_t* pcCounter, uint8_t* pcPrescaler) {
	if (m_xtalFrequency > 26000000) {
		m_xtalFrequency >>= 1;
	}
	/* If the desired value is over the maximum limit, the counter and the
	 prescaler are settled to their maximum values, and doesn't execute the routine */
	if ((fDesiredMsec > 3291.0 && m_xtalFrequency == 24000000)
			|| (fDesiredMsec > 3159.0 && m_xtalFrequency == 25000000)
			|| (fDesiredMsec > 3038.0 && m_xtalFrequency == 26000000)) {
		*pcCounter = 0xFF;
		*pcPrescaler = 0xFF;
		return;
	} else {
		float FPeriod = 1210.0 / (m_xtalFrequency / 1000000);
		uint8_t b0, a0;
		uint32_t n = (uint32_t) ((fDesiredMsec * 1000) / FPeriod);
		int32_t err, err_min;
		err_min = n;
		/* These are the initial values for the prescaler and the counter, where the prescaler
		 is settled to the minimum value and the counter accordingly. In order to avoid a zero
		 division for the counter the prescaler is increased by one.*/
		*pcPrescaler = a0 = (uint8_t) ((n - 1) / 0xFF);
		if (a0 == 0)
			*pcCounter = b0 = 0;
		else
			*pcCounter = b0 = (uint8_t) (n / *pcPrescaler) - 1;

		for (;; (*pcPrescaler)++) {
			*pcCounter = (uint8_t) (n / *pcPrescaler) - 1;
			err = (((uint32_t) (*pcPrescaler) + 1) * ((uint32_t) *pcCounter))
					- (uint32_t) n;

			if ((uint32_t) S_ABS(err) > (uint32_t) (*pcPrescaler / 2)) {
				(*pcCounter)++;
				err =
						(((uint32_t) (*pcPrescaler) + 1)
								* ((uint32_t) *pcCounter)) - (uint32_t) n;
			}
			if (S_ABS(err) < S_ABS(err_min)) {
				err_min = err;
				a0 = *pcPrescaler;
				b0 = *pcCounter;
				if (err_min == 0) {
					break;
				}
			}
			if (*pcPrescaler == (0xFF - 1)) {
				break;
			}
		}

		if (a0 == 0)
			a0 = 1;
		if (b0 == 0)
			b0 = 1;

		*pcPrescaler = a0;
		*pcCounter = b0;
	}
}

/*
 * Sets the RX timeout stop conditions.
 */
void CSpiritTimer::setRxTimeoutStopCondition(RxTimeoutStopCondition xStopCondition) {
	uint8_t tempRegValue[2];
	/* Reads value on the PKT_FLT_OPTIONS and PROTOCOL2 register */
	m_driverSpi->readRegisters(PCKT_FLT_OPTIONS_BASE, 2, tempRegValue);
	tempRegValue[0] &= 0xBF;
	tempRegValue[0] |= ((xStopCondition & 0x08) << 3);
	tempRegValue[1] &= 0x1F;
	tempRegValue[1] |= (xStopCondition << 5);
	/* Writes value on the PKT_FLT_OPTIONS and PROTOCOL2 register */
	m_driverSpi->writeRegisters(PCKT_FLT_OPTIONS_BASE, 2, tempRegValue);
}

/*
 * Sends the LDC_RELOAD command to SPIRIT. Reload the LDC timer with the value stored in the LDC_PRESCALER / COUNTER registers.
 */
void CSpiritTimer::reloadStrobe(void) {
	/* Sends the CMD_LDC_RELOAD command */
	m_driverSpi->commandStrobes(COMMAND_LDC_RELOAD);
}
