/*
 * CLcd.cpp
 *
 *  Created on: 18 gru 2015
 *      Author: Marcin
 */

#include "CLcd.h"

/*
 * Inicjalizacja zegara ltdc
 */
void CLcd::clockConfig(){
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
	/* LCD clock configuration */
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
	/* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
	HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct);
}


void CLcd::init(){
	clockConfig();
	pinsInit();
	/* LTDC Initialization -------------------------------------------------------*/

	  /* Polarity configuration */
	  /* Initialize the horizontal synchronization polarity as active low */
	m_hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
	  /* Initialize the vertical synchronization polarity as active low */
	m_hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
	  /* Initialize the data enable polarity as active low */
	m_hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
	  /* Initialize the pixel clock polarity as input pixel clock */
	m_hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

	  /* The RK043FN48H LCD 480x272 is selected */
	  /* Timing Configuration */
	m_hltdc.Init.HorizontalSync = (RK043FN48H_HSYNC - 1);
	m_hltdc.Init.VerticalSync = (RK043FN48H_VSYNC - 1);
	m_hltdc.Init.AccumulatedHBP = (RK043FN48H_HSYNC + RK043FN48H_HBP - 1);
	m_hltdc.Init.AccumulatedVBP = (RK043FN48H_VSYNC + RK043FN48H_VBP - 1);
	m_hltdc.Init.AccumulatedActiveH = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP - 1);
	m_hltdc.Init.AccumulatedActiveW = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + RK043FN48H_HBP - 1);
	m_hltdc.Init.TotalHeigh = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP + RK043FN48H_VFP - 1);
	m_hltdc.Init.TotalWidth = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + RK043FN48H_HBP + RK043FN48H_HFP - 1);

	  /* Configure R,G,B component values for LCD background color : all black background */
	m_hltdc.Init.Backcolor.Blue = 0;
	m_hltdc.Init.Backcolor.Green = 0;
	m_hltdc.Init.Backcolor.Red = 0;

	m_hltdc.Instance = LTDC;

	  /* Configure the LTDC */
	if (HAL_LTDC_Init(&m_hltdc) != HAL_OK) {
		/* Initialization Error */
		while (1) {
		}
	}
	/* Enable peripherals and GPIO Clocks */
	__HAL_RCC_DMA2D_CLK_ENABLE();
	layerConfig(1);
//	layerConfig(2);
}


void CLcd::layerConfig(int8_t numberLayer) {
	switch (numberLayer) {
	case 1:
		/* Layer1 Configuration ------------------------------------------------------*/
		/* Windowing configuration */
		/* In this case all the active display area is used to display a picture then :
		 Horizontal start = horizontal synchronization + Horizontal back porch = 43
		 Vertical start   = vertical synchronization + vertical back porch     = 12
		 Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1
		 Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */
		m_layer1.WindowX0 = 0;
		m_layer1.WindowX1 = 480;
		m_layer1.WindowY0 = 0;
		m_layer1.WindowY1 = 272;

		/* Pixel Format configuration*/
		m_layer1.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;

		/* Start Address configuration : frame buffer is located at FLASH memory */
		m_layer1.FBStartAdress = 0xC0000000;

		/* Alpha constant (255 == totally opaque) */
		m_layer1.Alpha = 255;

		/* Default Color configuration (configure A,R,G,B component values) : no background color */
		m_layer1.Alpha0 = 0; /* fully transparent */
		m_layer1.Backcolor.Blue = 0;
		m_layer1.Backcolor.Green = 0;
		m_layer1.Backcolor.Red = 0;

		/* Configure blending factors */
		m_layer1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
		m_layer1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

		/* Configure the number of lines and number of pixels per line */
		m_layer1.ImageWidth = 480;	//480
		m_layer1.ImageHeight = 272;	//272

		/* Configure the Layer*/
		if (HAL_LTDC_ConfigLayer(&m_hltdc, &m_layer1, numberLayer-1) != HAL_OK) {
			/* Initialization Error */
			while (1) {
			}
		}
		break;
	case 2:
		/* Layer1 Configuration ------------------------------------------------------*/
		/* Windowing configuration */
		/* In this case all the active display area is used to display a picture then :
		 Horizontal start = horizontal synchronization + Horizontal back porch = 43
		 Vertical start   = vertical synchronization + vertical back porch     = 12
		 Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1
		 Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */
		m_layer2.WindowX0 = 120;
		m_layer2.WindowX1 = 480;
		m_layer2.WindowY0 = 70;
		m_layer2.WindowY1 = 272;

		/* Pixel Format configuration*/
		m_layer2.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;

		/* Start Address configuration : frame buffer is located at FLASH memory */
		m_layer2.FBStartAdress = 0xC0000000 + (480*272*4);

		/* Alpha constant (255 == totally opaque) */
		m_layer2.Alpha = 255;

		/* Default Color configuration (configure A,R,G,B component values) : no background color */
		m_layer2.Alpha0 = 0; /* fully transparent */
		m_layer2.Backcolor.Blue = 0;
		m_layer2.Backcolor.Green = 0;
		m_layer2.Backcolor.Red = 0;

		/* Configure blending factors */
		m_layer2.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
		m_layer2.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;

		/* Configure the number of lines and number of pixels per line */
		m_layer2.ImageWidth = 480;
		m_layer2.ImageHeight = 272;

		/* Configure the Layer*/
		if (HAL_LTDC_ConfigLayer(&m_hltdc, &m_layer2, numberLayer-1) != HAL_OK) {
			/* Initialization Error */
			while (1) {
			}
		}
		break;
	default:
		break;
	}
}

void CLcd::pinsInit() {
	GPIO_InitTypeDef GPIO_Init_Structure;
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable the LTDC Clock */
	__HAL_RCC_LTDC_CLK_ENABLE()
	;

	/*** LTDC Pins configuration ***/
	/* GPIOE configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_4;
	GPIO_Init_Structure.Mode = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Pull = GPIO_NOPULL;
	GPIO_Init_Structure.Speed = GPIO_SPEED_FAST;
	GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

	/* GPIOG configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_12;
	GPIO_Init_Structure.Mode = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Alternate = GPIO_AF9_LTDC;
	HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

	/* GPIOI LTDC alternate configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
	GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_Init_Structure.Mode = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOI, &GPIO_Init_Structure);

	/* GPIOJ configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
			|GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
			GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 |
			GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_Init_Structure.Mode = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);

	/* GPIOK configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4
			|
			GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_Init_Structure.Mode = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOK, &GPIO_Init_Structure);

	/* LCD_DISP GPIO configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_12; /* LCD_DISP pin has to be manually controlled */
	GPIO_Init_Structure.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOI, &GPIO_Init_Structure);

	/* LCD_BL_CTRL GPIO configuration */
	GPIO_Init_Structure.Pin = GPIO_PIN_3; /* LCD_BL_CTRL pin has to be manually controlled */
	GPIO_Init_Structure.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOK, &GPIO_Init_Structure);

	/* Assert display enable LCD_DISP pin */
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12, GPIO_PIN_SET);

	/* Assert backlight LCD_BL_CTRL pin */
	HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_SET);
}

void CLcd::drawBitmap(uint32_t xPos, uint32_t yPos, uint8_t* pbmp){
	m_dma2dHandle.Instance = DMA2D;
	m_dma2dHandle.Init.Mode = DMA2D_M2M;
	m_dma2dHandle.Init.ColorMode = DMA2D_ARGB8888;
	m_dma2dHandle.Init.OutputOffset = 480-64;
	m_dma2dHandle.LayerCfg[1].InputOffset = 0;
	m_dma2dHandle.LayerCfg[1].InputColorMode = CM_ARGB8888;
	m_dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_COMBINE_ALPHA;
	m_dma2dHandle.LayerCfg[1].InputAlpha = 0;
	HAL_DMA2D_Init(&m_dma2dHandle);
	HAL_DMA2D_ConfigLayer(&m_dma2dHandle, 1);
}
