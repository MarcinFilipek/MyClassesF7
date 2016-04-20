/*
 * CTouchScreen.h
 *
 *  Created on: 26 sty 2016
 *      Author: Marcin
 */

#ifndef CTOUCHSCREEN_H_
#define CTOUCHSCREEN_H_

#include "../FT5336/FT5336.h"

class CTouchScreen {
public:
	typedef enum {
		TS_SWAP_NONE = 1,
		TS_SWAP_X = 2,
		TS_SWAP_Y = 4,
		TS_SWAP_XY = 8
	}TOrientation;

	typedef enum
	{
	  TOUCH_EVENT_NO_EVT        = 0x00, /*!< Touch Event : undetermined */
	  TOUCH_EVENT_PRESS_DOWN    = 0x01, /*!< Touch Event Press Down */
	  TOUCH_EVENT_LIFT_UP       = 0x02, /*!< Touch Event Lift Up */
	  TOUCH_EVENT_CONTACT       = 0x03, /*!< Touch Event Contact */
	  TOUCH_EVENT_NB_MAX        = 0x04  /*!< max number of touch events kind */
	} TS_TouchEvent;

	typedef enum
	{
	  GEST_ID_NO_GESTURE 		= 0x00, /*!< Gesture not defined / recognized */
	  GEST_ID_MOVE_UP    		= 0x01, /*!< Gesture Move Up */
	  GEST_ID_MOVE_RIGHT 		= 0x02, /*!< Gesture Move Right */
	  GEST_ID_MOVE_DOWN  		= 0x03, /*!< Gesture Move Down */
	  GEST_ID_MOVE_LEFT  		= 0x04, /*!< Gesture Move Left */
	  GEST_ID_ZOOM_IN    		= 0x05, /*!< Gesture Zoom In */
	  GEST_ID_ZOOM_OUT   		= 0x06, /*!< Gesture Zoom Out */
	  GEST_ID_NB_MAX    		= 0x07  /*!< max number of gesture id */
	} TS_GestureId;

	typedef struct {
		uint8_t touchDetected; 											/*!< Total number of active touches detected at last scan */
		uint16_t touchX[FT5336::m_maxDetectableTouch]; 					/*!< Touch X[0], X[1] coordinates on 12 bits */
		uint16_t touchY[FT5336::m_maxDetectableTouch]; 					/*!< Touch Y[0], Y[1] coordinates on 12 bits */
		uint8_t touchWeight[FT5336::m_maxDetectableTouch]; 				/*!< Touch_Weight[0], Touch_Weight[1] : weight property of touches */
		TS_TouchEvent touchEventId[FT5336::m_maxDetectableTouch]; 		/*!< Touch_EventId[0], Touch_EventId[1] : take value of type @ref TS_TouchEventTypeDef */
		uint8_t touchArea[FT5336::m_maxDetectableTouch]; 				/*!< Touch_Area[0], Touch_Area[1] : touch area of each touch */
		TS_GestureId gestureId; 										/*!< type of gesture detected : take value of type @ref TS_GestureIdTypeDef */
	} TS_State;

	FT5336 m_ft5336Driver;
	void init(uint16_t sizeX, uint16_t sizeY);
	void itConfig(void);
	void getState(void);
	TS_State getTouchScreenState(void){ return m_tsState; }
	void getGestureId(void);
private:
	static const uint16_t TS_I2C_ADDRESS = 0x70;
	uint16_t m_sizeX;
	uint16_t m_sizeY;
	TOrientation m_orientation;
	TS_State m_tsState;
	void resetTouchData(void);
protected:
};

#endif /* CTOUCHSCREEN_H_ */
