/*
 * CLcd.h
 *
 *  Created on: 18 gru 2015
 *      Author: Marcin
 */

#ifndef CLCD_H_
#define CLCD_H_

#include "stm32f7xx_hal.h"
#include "../inc/backgr.h"
#include "../inc/blend.h"

class CLcd {
public:
	static const uint16_t RK043FN48H_WIDTH = 480;          /* LCD PIXEL WIDTH            */
	static const uint16_t RK043FN48H_HEIGHT = 272;         /* LCD PIXEL HEIGHT           */

	static const uint16_t RK043FN48H_HSYNC = 41;			/* Horizontal synchronization */
	static const uint16_t RK043FN48H_HBP = /*1*/3;   			/* Horizontal back porch      */
	static const uint16_t RK043FN48H_HFP = 32;   			/* Horizontal front porch     */
	static const uint16_t RK043FN48H_VSYNC = 10;   		/* Vertical synchronization   */
	static const uint16_t RK043FN48H_VBP = 2;		    	/* Vertical back porch        */
	static const uint16_t RK043FN48H_VFP = 2;          	/* Vertical front porch       */

	void init(void);
	DMA2D_HandleTypeDef* getDma2DHandle(void) { return &m_dma2dHandle; }
	void drawBitmap(uint32_t xPos, uint32_t yPos, uint8_t* pbmp);
private:
	LTDC_HandleTypeDef m_hltdc;
	LTDC_LayerCfgTypeDef m_layer1;
	LTDC_LayerCfgTypeDef m_layer2;
	DMA2D_HandleTypeDef m_dma2dHandle;
	void pinsInit(void);
	void clockConfig(void);
	void layerConfig(int8_t numberLayer);
protected:
};

#endif /* CLCD_H_ */
