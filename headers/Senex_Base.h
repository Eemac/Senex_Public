#ifndef _SENEX_BASE_H
#define _SENEX_BASE_H

#include "Arduino.h"

#include "Senex_Settings.h"

#include "Wire.h"

#include <EEPROM.h>

//Enables/disables SPI type sensors for this microcontroller
#ifdef IS_SPI
	#include <SPI.h>
	#include <avr/wdt.h>
#endif

#include "Senex_IMU.h"
#include "Senex_Photonics.h"
#include "Senex_Vibe.h"

#ifdef CORE
	#include "Senex_AltCore.h"
#endif


class SENEX
{
	public:
		SENEX(void);
		#ifdef CORE
			void beginCore();
			void coreScheduler();

		#endif


		#ifndef CORE
			void beginController();
			void controllerScheduler();
		#endif

	private:

};


#ifndef CORE
	/*
	@name:	receiveEvent
	@brief: Master/Slave I2C data recieve interrupt
	@param: int howMany 		 	== # of bytes expected
	*/
	void receiveEvent(int howMany);


	/*
	@name:	requestEvent
	@brief: Master/Slave I2C data request interrupt
	*/
	void requestEvent();

	/*
	@name:	doControlUpdate
	@brief: Updates control preferances with respect to latest input packet form Core
	*/
	void doControlUpdate();

#endif

#ifdef CORE

	void pushHandUpdates(I2CBank &i2c);

	void updateHand(bool hand, I2CBank &i2c, S_IO &wirelessIO);

	void doSuitSettingsUpdate(S_IO &wirelessIO, I2CBank &right_i2c, I2CBank &left_i2c);

	void printSeparatorLine(S_IO &wirelessIO);

#endif

#ifndef CORE
	void printSeparatorLine();
#endif

#endif 
