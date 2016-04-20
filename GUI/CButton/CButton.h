/*
 * CButton.h
 *
 *  Created on: 10 mar 2016
 *      Author: marcin
 */

#ifndef CBUTTON_H_
#define CBUTTON_H_

#include "../Interfaces/IItem.h"

class CButton: public IItem {
public:
	CButton();
	void draw(void);
	void setImg(uint32_t addressImg);
private:
	uint32_t m_addressImg;
protected:
};

#endif /* CBUTTON_H_ */
