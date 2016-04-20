/*
 * CGraphics.h
 *
 *  Created on: 13 kwi 2016
 *      Author: Marcin
 */

#ifndef CGRAPHICS_H_
#define CGRAPHICS_H_

#include "stm32f7xx_hal.h"

class CGraphics {
public:
	static const uint16_t m_width = 480;
	static const uint16_t m_height = 272;

	DMA2D_HandleTypeDef* getDma2DHandle(void) {
		return &m_dma2dHandle;
	}
	void setDestStartAddress(uint32_t address);
	uint32_t getDestStartAddress(void) {
		return m_destStartAddress;
	}
	void drawPixel(uint16_t x, uint16_t y, uint32_t rgb);
	void drawBlending(uint32_t srcAddress1, uint32_t srcAddress2,
			uint32_t destAddress, uint32_t width, uint32_t height,
			uint32_t alpha);
private:
	DMA2D_HandleTypeDef m_dma2dHandle;
	static uint32_t m_destStartAddress;
protected:
};

#endif /* CGRAPHICS_H_ */
