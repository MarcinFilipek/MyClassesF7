/*
 * RadioConfParam.h
 *
 *  Created on: 19 lis 2015
 *      Author: Marcin
 */

#ifndef RADIOCONFPARAM_H_
#define RADIOCONFPARAM_H_


/*  Radio configuration parameters  */
#define XTAL_OFFSET_PPM             0
#define INFINITE_TIMEOUT            0.0

#ifdef USE_SPIRIT1_433MHz
#define BASE_FREQUENCY              433.0e6
#endif

#ifdef USE_SPIRIT1_868MHz
#define BASE_FREQUENCY              868.0e6
#endif

#ifdef USE_SPIRIT1_915MHz
#define BASE_FREQUENCY              915.0e6
#endif

#define CHANNEL_SPACE               20e3
#define CHANNEL_NUMBER              0
#define MODULATION_SELECT           0x00 	/* 0x00 FSK, 0x50 GFSK_BT05, 0x10 GFSK_BT1, 0x20 ASK_OOK, 0x30 MSK	*/
#define DATARATE                    38400
#define FREQ_DEVIATION              20e3
#define BANDWIDTH                   100E3

#define POWER_DBM                   11.6
#define POWER_INDEX                 7
#define RECEIVE_TIMEOUT             2000.0 /*change the value for required timeout period*/

#define RSSI_THRESHOLD              -120

/*  Packet configuration parameters  */
#define PREAMBLE_LENGTH             PKT_PREAMBLE_LENGTH_04BYTES
#define SYNC_LENGTH                 PKT_SYNC_LENGTH_4BYTES
#define SYNC_WORD                   0x1A2635A8
#define LENGTH_TYPE                 PKT_LENGTH_VAR
#define LENGTH_WIDTH                7
#define CRC_MODE                    PKT_CRC_MODE_8BITS
#define CONTROL_LENGTH              PKT_CONTROL_LENGTH_0BYTES
#define EN_ADDRESS                  false
#define EN_FEC                      false
#define EN_WHITENING                true

/*  Addresses configuration parameters  */
#define EN_FILT_MY_ADDRESS          false
#define MY_ADDRESS                  0x34
#define EN_FILT_MULTICAST_ADDRESS   false
#define MULTICAST_ADDRESS           0xEE
#define EN_FILT_BROADCAST_ADDRESS   false
#define BROADCAST_ADDRESS           0xFF
#define DESTINATION_ADDRESS         0x44
#define EN_FILT_SOURCE_ADDRESS      false
#define SOURCE_ADDR_MASK            0xf0
#define SOURCE_ADDR_REF             0x37

#define PAYLOAD_LEN                     25 /*20 bytes data+tag+cmd_type+cmd+cmdlen+datalen*/
#define APPLI_CMD                       0x11
#define NWK_CMD                         0x22
#define LED_TOGGLE                      0xff
#define ACK_OK                          0x01
#define MAX_BUFFER_LEN                  96
#define TIME_TO_EXIT_RX                 3000
#define DELAY_RX_LED_TOGGLE             200
#define DELAY_TX_LED_GLOW               1000
#define LPM_WAKEUP_TIME                 100
#define DATA_SEND_TIME                  30


#endif /* RADIOCONFPARAM_H_ */
