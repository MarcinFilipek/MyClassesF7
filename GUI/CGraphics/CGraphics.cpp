/*
 * CGraphics.cpp
 *
 *  Created on: 13 kwi 2016
 *      Author: Marcin
 */

#include "CGraphics.h"
#include "CMathFunction.h"

uint32_t CGraphics::m_destStartAddress;

void CGraphics::setDestStartAddress(uint32_t address) {
	m_destStartAddress = address;
}
void CGraphics::drawPixel(uint16_t x, uint16_t y, uint32_t rgb) {
	*(__IO uint32_t*) (m_destStartAddress + (4 * (y * m_width + x))) = rgb;
}

void CGraphics::drawBlending(uint32_t srcAddress1, uint32_t srcAddress2,
		uint32_t destAddress, uint32_t width, uint32_t height, uint32_t alpha) {
	//dest
	m_dma2dHandle.Instance = DMA2D;
	m_dma2dHandle.Init.Mode = DMA2D_M2M_BLEND;
	m_dma2dHandle.Init.ColorMode = DMA2D_ARGB8888;
	m_dma2dHandle.Init.OutputOffset = 480 - width;

	//source 2
	m_dma2dHandle.LayerCfg[0].InputOffset = 480 - width;
	m_dma2dHandle.LayerCfg[0].InputColorMode = CM_ARGB8888;
	m_dma2dHandle.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	m_dma2dHandle.LayerCfg[0].InputAlpha = 0;

	//source 1
	m_dma2dHandle.LayerCfg[1].InputOffset = 0;
	m_dma2dHandle.LayerCfg[1].InputColorMode = CM_ARGB8888;
	m_dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_COMBINE_ALPHA;
	m_dma2dHandle.LayerCfg[1].InputAlpha = alpha;

	HAL_DMA2D_Init(&m_dma2dHandle);
	HAL_DMA2D_ConfigLayer(&m_dma2dHandle, 0);
	HAL_DMA2D_ConfigLayer(&m_dma2dHandle, 1);

	HAL_DMA2D_BlendingStart(&m_dma2dHandle, srcAddress1, srcAddress2,
			destAddress, width, height);
	HAL_DMA2D_PollForTransfer(&m_dma2dHandle, 1000);
}

/*
 *	xPixel 		- wspolrzedna x punktu obracanego
 *	yPixel 		- wspolrzedna y punktu obracanego
 *	x			- wspolrzedna x punktu obrotu
 *	y			- wspolrzenda y punktu obrotu
 *	alfa		- kat obrotu
 *	xPixelPrim	- wspolrzena x punktu obracanego po obrocie
 *	yPixelPrim	- wspolrzena y punktu obracanego po obrocie
 */
void CGraphics::rotationPixel(uint16_t xPixel, uint16_t yPixel, uint16_t x,
		uint16_t y, uint16_t alfa, uint16_t* xPixelPrim, uint16_t* yPixelPrim) {

	*xPixelPrim = (xPixel - x) * CMathFunction::getCos(alfa)
			+ (yPixel - y) * CMathFunction::getSin(alfa) + x;
	*yPixelPrim = (xPixel - x) * CMathFunction::getSin(alfa)
			+ (yPixel - y) * CMathFunction::getCos(alfa) + y;
}
