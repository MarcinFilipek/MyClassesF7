/*
 * CFmcSdram.cpp
 *
 *  Created on: 4 sty 2016
 *      Author: marcin
 */

#include "CFmcSdram.h"

CFmcSdram::CFmcSdram(){
	  pinInit();
	  /*##-1- Configure the SDRAM device #########################################*/
	  /* SDRAM device configuration */
	  m_hsdram.Instance = FMC_SDRAM_DEVICE;

	  m_sdramTiming.LoadToActiveDelay    = 2;
	  m_sdramTiming.ExitSelfRefreshDelay = 6;
	  m_sdramTiming.SelfRefreshTime      = 4;
	  m_sdramTiming.RowCycleDelay        = 6;
	  m_sdramTiming.WriteRecoveryTime    = 2;
	  m_sdramTiming.RPDelay              = 2;
	  m_sdramTiming.RCDDelay             = 2;

	  m_hsdram.Init.SDBank             = FMC_SDRAM_BANK1;
	  m_hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;
	  m_hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
	  m_hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;
	  m_hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	  m_hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;
	  m_hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	  m_hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
	  m_hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
	  m_hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;
	  /* Initialize the SDRAM controller */
	  if(HAL_SDRAM_Init(&m_hsdram, &m_sdramTiming) != HAL_OK)
	  {
		  while(1){}
	  }

	  /* Program the SDRAM external device */
	  initializationSequence(&m_hsdram, &m_command);
}

void CFmcSdram::initializationSequence(SDRAM_HandleTypeDef* hsdram, FMC_SDRAM_CommandTypeDef* command){
	  __IO uint32_t tmpmrd = 0;

	  /* Step 1: Configure a clock configuration enable command */
	  command->CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
	  command->CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
	  command->AutoRefreshNumber      = 1;
	  command->ModeRegisterDefinition = 0;

	  /* Send the command */
	  HAL_SDRAM_SendCommand(hsdram, command, SDRAM_TIMEOUT);

	  /* Step 2: Insert 100 us minimum delay */
	  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
	  HAL_Delay(1);

	  /* Step 3: Configure a PALL (precharge all) command */
	  command->CommandMode            = FMC_SDRAM_CMD_PALL;
	  command->CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
	  command->AutoRefreshNumber      = 1;
	  command->ModeRegisterDefinition = 0;

	  /* Send the command */
	  HAL_SDRAM_SendCommand(hsdram, command, SDRAM_TIMEOUT);

	  /* Step 4: Configure an Auto Refresh command */
	  command->CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	  command->CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
	  command->AutoRefreshNumber      = 8;
	  command->ModeRegisterDefinition = 0;

	  /* Send the command */
	  HAL_SDRAM_SendCommand(hsdram, command, SDRAM_TIMEOUT);

	  /* Step 5: Program the external memory mode register */
	  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |\
	                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |\
	                     SDRAM_MODEREG_CAS_LATENCY_2           |\
	                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |\
	                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	  command->CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
	  command->CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
	  command->AutoRefreshNumber      = 1;
	  command->ModeRegisterDefinition = tmpmrd;

	  /* Send the command */
	  HAL_SDRAM_SendCommand(hsdram, command, SDRAM_TIMEOUT);

	  /* Step 6: Set the refresh rate counter */
	  /* Set the device refresh rate */
	  HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);
}

void CFmcSdram::pinInit() {
	GPIO_InitTypeDef GPIO_Init_Structure;
	/* Enable FMC clock */
	__HAL_RCC_FMC_CLK_ENABLE();

	/* Common GPIO configuration */
	GPIO_Init_Structure.Mode = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Pull = GPIO_PULLUP;
	GPIO_Init_Structure.Speed = GPIO_SPEED_FAST;
	GPIO_Init_Structure.Alternate = GPIO_AF12_FMC;

	/* GPIOC configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

	/* GPIOD configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_8
			| GPIO_PIN_9 |
			GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

	/* GPIOE configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8
			| GPIO_PIN_9 |
			GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
			|
			GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

	/* GPIOF configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
			| GPIO_PIN_4 |
			GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |
			GPIO_PIN_15;
	HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

	/* GPIOG configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5
			| GPIO_PIN_8 |
			GPIO_PIN_15;
	HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

	/* GPIOH configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_3 | GPIO_PIN_5;
	HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);
}

void CFmcSdram::write32(uint32_t* pBuffer, uint32_t writeAddress, uint32_t bufferSize){
	uint32_t write_pointer = SDRAM_BANK_ADDR + writeAddress;
	HAL_SDRAM_WriteProtection_Disable(&m_hsdram);
	while(HAL_SDRAM_GetState(&m_hsdram)==HAL_SDRAM_STATE_RESET){
        }
	for(;bufferSize !=0; bufferSize--){
             HAL_SDRAM_Write_32b(&m_hsdram, (uint32_t *)write_pointer, pBuffer++, 1/*bufferSize*/);
             write_pointer += 4;
	}
}

void CFmcSdram::read32(uint32_t* pBuffer, uint32_t readAddress, uint32_t bufferSize){
        uint32_t read_pointer = SDRAM_BANK_ADDR + readAddress;
        while(HAL_SDRAM_GetState(&m_hsdram)==HAL_SDRAM_STATE_RESET){};
        for(; bufferSize != 0x00; bufferSize--){
             HAL_SDRAM_Read_32b(&m_hsdram, (uint32_t *)read_pointer, pBuffer++, 1/*bufferSize*/);
            read_pointer += 4;
        }
}
