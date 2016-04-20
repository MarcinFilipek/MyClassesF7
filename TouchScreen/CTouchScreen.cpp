/*
 * CTouchScreen.cpp
 *
 *  Created on: 26 sty 2016
 *      Author: Marcin
 */

#include "CTouchScreen.h"
#include "../FT5336/reg.h"

void CTouchScreen::init(uint16_t sizeX, uint16_t sizeY){
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_ft5336Driver.init(TS_I2C_ADDRESS);
	if(m_ft5336Driver.readID() == FT5336_ID_VALUE){
		m_orientation = TS_SWAP_XY;
		m_ft5336Driver.start();
	}
}

void CTouchScreen::itConfig(){
	GPIO_InitTypeDef gpioInit;
	gpioInit.Pin = GPIO_PIN_13;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed	= GPIO_SPEED_FAST;
	gpioInit.Mode = GPIO_MODE_IT_RISING;
	HAL_GPIO_Init(GPIOI, &gpioInit);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0x0F, 0x00);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	m_ft5336Driver.enableIT();
}

void CTouchScreen::getState()
{
  static uint32_t _x[FT5336::m_maxDetectableTouch] = {0, 0};
  static uint32_t _y[FT5336::m_maxDetectableTouch] = {0, 0};
  uint16_t x[FT5336::m_maxDetectableTouch];
  uint16_t y[FT5336::m_maxDetectableTouch];
  uint16_t brute_x[FT5336::m_maxDetectableTouch];
  uint16_t brute_y[FT5336::m_maxDetectableTouch];
  uint16_t x_diff;
  uint16_t y_diff;
  uint32_t index;
  uint32_t weight = 0;
  uint32_t area = 0;
  uint32_t event = 0;

  /* Check and update the number of touches active detected */
  m_tsState.touchDetected = m_ft5336Driver.detectTouch();

  if(m_tsState.touchDetected)
  {
    for(index=0; index < m_tsState.touchDetected; index++)
    {
      /* Get each touch coordinates */
      m_ft5336Driver.getXY(&(brute_x[index]), &(brute_y[index]));

      if(m_orientation == TS_SWAP_NONE)
      {
        x[index] = brute_x[index];
        y[index] = brute_y[index];
      }

      if(m_orientation & TS_SWAP_X)
      {
        x[index] = 4096 - brute_x[index];
      }

      if(m_orientation & TS_SWAP_Y)
      {
        y[index] = 4096 - brute_y[index];
      }

      if(m_orientation & TS_SWAP_XY)
      {
        y[index] = brute_x[index];
        x[index] = brute_y[index];
      }

      x_diff = x[index] > _x[index]? (x[index] - _x[index]): (_x[index] - x[index]);
      y_diff = y[index] > _y[index]? (y[index] - _y[index]): (_y[index] - y[index]);

      if ((x_diff + y_diff) > 5)
      {
        _x[index] = x[index];
        _y[index] = y[index];
      }

      m_tsState.touchX[index] = x[index];
      m_tsState.touchY[index] = y[index];

      /* Get touch info related to the current touch */
      m_ft5336Driver.getTouchInfo(index, &weight, &area, &event);

      /* Update TS_State structure */
      m_tsState.touchWeight[index] = weight;
      m_tsState.touchArea[index]   = area;

      /* Remap touch event */
      switch(event)
      {
        case FT5336_TOUCH_EVT_FLAG_PRESS_DOWN	:
          m_tsState.touchEventId[index] = TOUCH_EVENT_PRESS_DOWN;
          break;
        case FT5336_TOUCH_EVT_FLAG_LIFT_UP :
          m_tsState.touchEventId[index] = TOUCH_EVENT_LIFT_UP;
          break;
        case FT5336_TOUCH_EVT_FLAG_CONTACT :
          m_tsState.touchEventId[index] = TOUCH_EVENT_CONTACT;
          break;
        case FT5336_TOUCH_EVT_FLAG_NO_EVENT :
          m_tsState.touchEventId[index] = TOUCH_EVENT_NO_EVT;
          break;
        default :
          while(1){}
          break;
      } /* of switch(event) */

    } /* of for(index=0; index < TS_State->touchDetected; index++) */

    /* Get gesture Id */
    getGestureId();

  } /* end of if(TS_State->touchDetected != 0) */
}

void CTouchScreen::getGestureId() {
	uint32_t gestureId = 0;

	/* Get gesture Id */
	m_ft5336Driver.getGestureID(&gestureId);

	/* Remap gesture Id to a TS_GestureIdTypeDef value */
	switch (gestureId) {
	case FT5336_GEST_ID_NO_GESTURE:
		m_tsState.gestureId = GEST_ID_NO_GESTURE;
		break;
	case FT5336_GEST_ID_MOVE_UP:
		m_tsState.gestureId = GEST_ID_MOVE_UP;
		break;
	case FT5336_GEST_ID_MOVE_RIGHT:
		m_tsState.gestureId = GEST_ID_MOVE_RIGHT;
		break;
	case FT5336_GEST_ID_MOVE_DOWN:
		m_tsState.gestureId = GEST_ID_MOVE_DOWN;
		break;
	case FT5336_GEST_ID_MOVE_LEFT:
		m_tsState.gestureId = GEST_ID_MOVE_LEFT;
		break;
	case FT5336_GEST_ID_ZOOM_IN:
		m_tsState.gestureId = GEST_ID_ZOOM_IN;
		break;
	case FT5336_GEST_ID_ZOOM_OUT:
		m_tsState.gestureId = GEST_ID_ZOOM_OUT;
		break;
	default:
		while (1) {
		}
		break;
	} /* of switch(gestureId) */
}


void CTouchScreen::resetTouchData() {
	uint32_t index;
	m_tsState.gestureId = GEST_ID_NO_GESTURE;
	m_tsState.touchDetected = 0;

	for (index = 0; index < FT5336::m_maxDetectableTouch; index++) {
		m_tsState.touchX[index] = 0;
		m_tsState.touchY[index] = 0;
		m_tsState.touchArea[index] = 0;
		m_tsState.touchEventId[index] = TOUCH_EVENT_NO_EVT;
		m_tsState.touchWeight[index] = 0;
	}
}
