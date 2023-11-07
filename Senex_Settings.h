 /* SENEX VR CONFIDENTIAL
 * 
 *  [2020-2023] Senex VR, LLC 
 *  All Rights Reserved.
 * 
 * NOTICE:  All information contained herein is, and remains
 * the property of Senex VR, LLC and its suppliers,
 * if any. The intellectual and technical concepts contained
 * herein are proprietary to Senex VR, LLC and its suppliers and may 
 * be covered by U.S. and Foreign Patents, patents in process, and 
 * are protected by trade secret or copyright law. Dissemination of 
 * this information or reproduction of this material is strictly 
 * forbidden unless prior written permission is obtained
 * from Senex VR, LLC.
 */

#ifndef _SENEX_SETTINGS_H
#define _SENEX_SETTINGS_H

	#define CONTROLLER_CHIPS 10
	#define CORE_CHIPS 15

	//Enable for CORE Program
	// #define CORE 

	//Enable for CONTROLLER program
	#define LEFT_HAND
	#define L_CTRL_ADDR 5

	// #define RIGHT_HAND
	#define R_CTRL_ADDR 4

	#ifdef CORE
		#define PRINT(text) logPrint(wirelessIO, String(text));
		#define PRINTHEX(text) logPrint(wirelessIO, String(text, HEX));
		#define PRINTLN(text) logPrintln(wirelessIO, String(text));
		#define PRINTLNHEX(text) logPrintln(wirelessIO, String(text, HEX));
		#define SEPARATORLINE() printSeparatorLine(wirelessIO);
	#else
		#define PRINT(text) Serial.print(text);
		#define PRINTHEX(text) Serial.print(text, HEX);
		#define PRINTLN(text) Serial.println(text);
		#define PRINTLNHEX(text) Serial.println(text, HEX);
		#define SEPARATORLINE() printSeparatorLine();
	#endif

	//1 for QUAT9 or 0 for QUAT6 output
	#define QUAT_ALGO 1

	//1 for 28fps, 0 for 56 fps
	#define ODR_LIMITER 0

	//Adds User-readable statuses to UART output
	#define USE_TEXT

	//Comment in for ICM20948 Calibration Sequence
	#define ICM20948_CALIBRATION

	//Output Calibration data over debug
	#define DO_CALIBRATION_TEXT

	//Comment in for raw packet read of ICM20948
	//#define PACKET_READ

	//Comment in for packet length (When PACKET_READ is too much data over serial)
	// #define PACKET_SHORT_LENGTH

	//Enables more verbatim status reports (often these make raw data hard to read, or are part of functions that are called often) [SPAMMMMMY]
	//THIS WILL SLOW DOWN THE ODR OF THE SUIT BY A LOT!! (BE WARNED)
	// #define FULL_DATA

	//When Enabled, it will reset the EEPROM used and any sensor biases written will be cleared
	// #define RESET_EEPROM

	#ifdef CORE
		//Whether to add a delay in the main loop
		// #define MAINLOOP_DEBUG_DELAY
		#define MAINLOOP_DEBUG_DELAY_TIME 8
		//Enables OTA communication
		#define WIFI

		//Allow ESP32 Over-the-Air updates
		#define OTA_UPDATE
		#define SKETCH_UPLOAD_PASSWORD "SENEXFTW"

		//Enables/disables I2C type sensors for this microcontroller
		#define IS_I2C

		//Custom UID for each suit produced
		#define UID 0x53583031
		#define TEXT_UID "SX01"

		//The amount of EEPROM storage allocated to IMU bias storage ([36 * 15]B for actual data, 2B for IMU bitmask)
		#define IMU_EEPROM_SIZE 36 * 15 + 2

		//Secondary I2C Pins for Lower body + haptic motors
		#define SDA_2 33
		#define SCL_2 32
	#endif


	//For both Right and Left hand init
	#if defined LEFT_HAND || defined RIGHT_HAND 
		//Enables/disables SPI type sensors for this microcontroller
		#define IS_SPI

		//The amount of EEPROM storage allocated to IMU bias storage
		#define IMU_EEPROM_SIZE 36 * 10 + 2

		//#define USE_LRA

		// #define MIN_TEXT

		// #define FULL_HAND_I2C_DATA
	#endif


	//Only Right hand
	#ifdef RIGHT_HAND
		#define CTRL_ADDR R_CTRL_ADDR
		#define SPI_CH_OFFSET 10
	#endif

	//Only Left hand
	#ifdef LEFT_HAND
		//should be 5
		#define CTRL_ADDR L_CTRL_ADDR
		#define SPI_CH_OFFSET 0
	#endif

	 struct I2CBank
	{
		unsigned char id;
		//Control byte for input packet length and function
		unsigned char ctrl;

		unsigned char muxAddr;
		unsigned char isSecondaryI2C;

		//Two variables used for read packet/length control
		unsigned char readCounter;
		unsigned char TotalReadCounter;

		//A combined tally of every chip that is active, whether it be sending data, ready to send, or resetting
		unsigned short chipsAlive;

		//Bitmask of which chips are alive
		unsigned short chipBitmask;

		//RGB Channels (R, G, B)
		unsigned char LEDRed;
		unsigned char LEDGreen;
		unsigned char LEDBlue;

		//Hand sensor enable bytes
		unsigned char chip_en_1;
		unsigned char chip_en_2;

		//Hand sensor reset bytes
		unsigned char chip_rst_1;
		unsigned char chip_rst_2;

		//Hand sensor cal bytes
		unsigned char chip_cal_1;
		unsigned char chip_cal_2;

		//Buffer register for the DRV2605 sequencer
		unsigned char wave[8];

		//Where in the chain of sensors this controller begins
		unsigned char bodyArrayStart;

		//set to fire the DRV2605 sequencer
		bool goVibe;
	};

#endif 
