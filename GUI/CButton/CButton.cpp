/*
 * CButton.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: marcin
 */

#include "CButton.h"

CButton::CButton() {
	m_addressImg = 0;
	m_posX = 0;
	m_posY = 0;
}

void CButton::setImg(uint32_t addressImg) {
	m_addressImg = addressImg;
}

void CButton::draw(void) {
	uint32_t srcAddress2 = 0, destAddress = 0;
	srcAddress2 = (m_graphics.m_width * m_graphics.m_height * 4) + (m_graphics.m_width * m_posY + m_posX) * 4 + m_graphics.getDestStartAddress();
	destAddress = (m_graphics.m_width * m_posY + m_posX) * 4 + m_graphics.getDestStartAddress();
	m_graphics.drawBlending(m_addressImg, srcAddress2, destAddress, m_width, m_height, 255);
}
