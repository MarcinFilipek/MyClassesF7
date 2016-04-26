/*
 * CMathFunction.h
 *
 *  Created on: 26 kwi 2016
 *      Author: Marcin
 */

#ifndef CMATHFUNCTION_H_
#define CMATHFUNCTION_H_

#include "stdio.h"

class CMathFunction {
public:
	float getSin(uint16_t alfa);
private:
	static const float m_sinTab[91];
protected:
};

#endif /* CMATHFUNCTION_H_ */
