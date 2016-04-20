/*
 * CScreen.h
 *
 *  Created on: 10 mar 2016
 *      Author: marcin
 */

#ifndef CSCREEN_H_
#define CSCREEN_H_

#include "../Interfaces/IScreen.h"
#include "../Interfaces/IItem.h"

class CScreen: private IScreen {
public:
private:
	IItem* m_itemsTab[5];
protected:
};

#endif /* CSCREEN_H_ */
