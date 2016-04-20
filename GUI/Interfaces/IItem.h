/*
 * IItem.h
 *
 *  Created on: 10 mar 2016
 *      Author: marcin
 */

#ifndef IITEM_H_
#define IITEM_H_

#include "stdint.h"
#include "../CGraphics/CGraphics.h"

class IItem{
public:
	virtual ~IItem(void){}
	virtual void draw(void)=0;
	void setPosition(uint16_t x, uint16_t y){
		m_posX = x;
		m_posY = y;
	}
	void setSize(uint16_t w, uint16_t h){
		m_width = w;
		m_height = h;
	}
protected:
	CGraphics m_graphics;
	uint16_t m_width;
	uint16_t m_height;
	uint16_t m_posX;
	uint16_t m_posY;
};



#endif /* IITEM_H_ */
