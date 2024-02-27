/* Comment Syntax:
* -------------------------------------------------------------------------------------------
* |   Title   |   Meaning                                                                   |
* -------------------------------------------------------------------------------------------
* |   @name   |   The name of the function being defined.                                   |
* |   @brief  |   A quick definition of what the function does                              |
* |   @param  |   A parameter in the function, and a simple description of what it is.      |
* |   @type   |   Whether the function is used in the CONTROLLER, the CORE, or BOTH.        |
* |   @note   |   An additional piece of information, usually when the function was tested. |
* |   @return |   Possible values the function returns, if any.	                            |
* -------------------------------------------------------------------------------------------
*/

#ifndef _SENEX_IMU_H
#define _SENEX_IMU_H

#include "Arduino.h"

#include "Senex_Settings.h"

#include "Math.h"

#include "Wire.h"

#include <EEPROM.h>

#include "Senex_Base.h"

#ifdef CORE
	#include "Senex_AltCore.h"
#endif

//Enables/disables SPI type sensors for this microcontroller
#ifdef IS_SPI
    #include <SPI.h>
    #include <avr/wdt.h>
#endif

//This is a general struct for each sensor.
//It contains all relevant data needed for sensor operation.
struct ICM20948_BASE
{
	///////////UNIVERSAL SENSOR VARIABLES//////////
	//integer between 1 and 35 that describes sensor position
	unsigned char channel;

	//this is used in the setBank for remembering which bank the ICM20948 chip is currently viewing
	unsigned char lastBank;

	//set when a new data packet is ready for transfer to the large packet storage registers
	bool newData;

	//this is used when the DMP image is being loaded
	unsigned char lastDMPBank;

	//Storage space for Accel and Gyro FSR store settings
	unsigned char accel_FSR;
	unsigned char gyro_FSR;

	//Bias set
	unsigned char hwAGBias[12];
	uint32_t magBias[3];

	//Timebase Correction register (signed char) (this tells us how far off the PLL is from the factory expected value)
	unsigned char timebase_correction;

	//Enable/Disable for core DMP Loop
	bool DMPLP;

	//Enable/Disable IMU sleep
	bool asleep;

	//Enable/Disable DMP/FIFO Data Collection (Effectively makes the chip dissapear - it won't get read from)
	bool isOn;

	//Time when the chip was reset last
	long lastReset;

	//Easy Access for which place in the bitmask this chip takes up
	unsigned short maskPlace;
	unsigned char chipNum;

	//Accel/gyro/mag calibration status
	unsigned char CalAccelStat;
	unsigned char CalGyroStat;
	unsigned char CalMagStat;

	unsigned long firstIMUBiasTime;

	//////////I2C SPECIFIC VARIABLES//////////

	//address of the multiplexer: 0x70 to 0x73, 0x00 for no Mux
	unsigned char muxAddr;
	//determines whether sensor is on Mux board or solitary unit
	//FALSE for on Mux board, TRUE for standalone
	bool isSecondaryI2C;

	//I2C adress if the actual device 
	unsigned char addr;

	//////////SPI SPECIFIC VARIABLES//////////

	//Which physical pin to use for SPI CS on hands
	unsigned char CSPin;

	//Raw DMP3 output (QUAT9)
	long sensorOutput[3];
};

bool setChipBiases(ICM20948_BASE &chip);


/////////////////////////////////////////////////////////////////////////////////////////////////
//											SUIT INIT										   //
/////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	* @name:	Start
	* @brief:	Sets up ICM20948 struct
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @param:	unsigned char sensNum			== Sets specific device parameters. Each sensor must have a unique ID
	* @return:	void
	* @type: 	BOTH
	*/
	void Start(ICM20948_BASE &chip, unsigned char sensNum);

	/*
	* @name:	resetMux
	* @brief: 	Resets I2C sensor matrix
	* @return:	void
	* @type: 	CORE
	*/
	void resetMux();

	/*
	* @name:	selectMux
	* @brief:	Selects a mux on I2C level
	* @param:	unsigned char muxAddr 			== I2C address of the mux in question
	* @param:	bool isSecondaryI2C 			== Figure out chich channel to pass through to
	* @return:	bool check						== True if error, false if OK
	* @type: 	CORE
	*/
	bool selectMux(unsigned char muxAddr, bool isSecondaryI2C);

	/*
	* @name:	resetCS
	* @brief: 	Inits chip select (CS) lines for hand sensors
	* @return:	void
	* @type: 	CONTROLLER
	*/
	void resetCS();


/////////////////////////////////////////////////////////////////////////////////////////////////
//											DMP INIT										   //
/////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	* @name:	setSensor
	* @brief: 	Init ICM20948 9-Axis IMU to QUAT9 DMP Output
	* @param: 	ICM20948 &chip 					== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	* @note:	TESTED GOOD AS OF: JAN 27 2021
	*/
	bool setSensor(ICM20948_BASE &chip, bool doRetry = false);

	/*
	* @name:	setSensorHelper
	* @brief: 	Init ICM20948 9-Axis IMU to QUAT9 DMP Output
	* @param: 	ICM20948 &chip 					== Core IMU struct
	* @param: 	bool doRetry = false 			== Do reset untik the chip gives a valid FIFO output
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	* @note:	TESTED GOOD AS OF: JAN 27 2021
	*/
	bool setSensorHelper(ICM20948_BASE &chip);

	/*
	* @name:	setMag
	* @brief:	Set up the magnometer when initing the DMP
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool setMag(ICM20948_BASE &chip);

	/*
	* @name:	setAuxI2CBus
	* @brief:	Setup the external I2C Bus on the ICM20948 for AK09916 magnometer
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool setAuxI2CBus(ICM20948_BASE &chip);

	/*
	* @name:	dmp_fifo_enable
	* @brief:	Enable DMP and FIFO functionality
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @param:	bool state 						== Function Enable/Disable
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool dmp_fifo_enable(ICM20948_BASE &chip, bool state);

	/*
	* @name:	fifo_reset
	* @brief:	Reset FIFO array
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool fifo_reset(ICM20948_BASE &chip);

	/*
	* @name:	dmp_reset
	* @brief:	Reset DMP coprocessor
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool dmp_reset(ICM20948_BASE &chip);

	/*
	* @name:	disableInterruptsAndData
	* @brief:	Disables all functionality of the INT1 Pin
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool disableInterruptsAndData(ICM20948_BASE &chip);

	/*
	* @name:	setAccGyroFSR
	* @brief:	Set the sensitifity of the Accelerometer and Gyro, and some other init feaures for those two as well
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @param:	unsigned char accelFSR 			== Set Full Scale Range of accelerometer to ±4g
	* @param:	unsigned char gyroFSR 			== Set Full Scale Range of gyroscope to ±250dps
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool setAccGyroFSR(ICM20948_BASE &chip, unsigned char accelFSR = 1, unsigned char gyroFSR = 3);

	/*
	* @name:	DMPFlash
	* @brief:	Flashes the DMP and sets up some DMP and FIFO registers
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type:	BOTH
	* @note:	Deprecated and the function no longer exists... at least that I've found
	*/
	bool DMPFlash(ICM20948_BASE &chip);

	/*
	* @name:	DMPFlashImage
	* @brief:	Flashes the DMP ONLY
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @param:	int toReg 						== How far to read through the image (1-894 lines * 16 bytes) (Total img size = 14301B)
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool DMPFlashImage(ICM20948_BASE &chip, int toReg = 0);


	/*
	* @name:	clearDMP
	* @brief:	Pre-Clears the DMP in the case of system reset
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool clearDMP(ICM20948_BASE &chip);

	/*
	* @name:	readImage
	* @brief:	Read DMP image off of chip
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool readImage(ICM20948_BASE &chip);

	/*
	* @name:	initChipMatricies
	* @brief:	Send DMP mounting matricies of the compass and the B2S feature
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool initChipMatricies(ICM20948_BASE &chip);

	/*
	* @name:	DMPGetSF
	* @brief:	Set custom gyro SF based on MEMS clock offset value
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool DMPGetSF(ICM20948_BASE &chip);

	/*
	* @name:	testFifo
	* @brief:	Make sure the DMP is outputting data
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool testFifo(ICM20948_BASE &chip);


/////////////////////////////////////////////////////////////////////////////////////////////////
//										CHIP CALIBRATION									   //
/////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	* @name:	getDMPBiases
	* @brief:	Part of one-time chip calibration sequence. Gets the DMP Biases and gets the Accuracy flags of each sensor
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @param:	bool printResults 				== Choose whether to print the results of the calibration
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool getDMPBiases(ICM20948_BASE &chip, bool printResults);

	/*
	* @name:	setChipBiases
	* @brief:	Input custom bias data for accelerometer, gyroscope, and magnometer into the hardware registers and the DMP
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool setChipBiases(ICM20948_BASE &chip);

	/*
	* @name:	debugBiasEEPROM
	* @brief:	Debug readout of the entire EEPROM space used for bias corrections/saves on power off
	* @return:	void
	* @type		BOTH
	*/
	void debugBiasEEPROM();


/////////////////////////////////////////////////////////////////////////////////////////////////
//											CHIP I/O										   //
/////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	* @name:	write_reg
	* @brief: 	Base I2C write to ICM20948
	* @param: 	ICM20948 &chip 					== Core IMU struct
	* @param: 	unsigned char reg 				== Hardware register address to start write
	* @param: 	uint32_t len 					== Length of data to be written
	* @param: 	const unsigned char *data 		== Pointer to char array to write data from
	* @param: 	bool isDMP = false 				== Whether the pointer is to a location in ARDX ROM (true) or in system RAM (false)
	* @return:	bool check						== True if error, false if OK
	* @type: 	BOTH
	* @note:	TESTED GOOD AS OF: DEC 19 2020
	*/
	bool write_reg(ICM20948_BASE &chip, unsigned char reg, uint32_t len, const unsigned char *data, bool isDMP = false);

	/*
	* @name:	read_reg
	* @brief: 	Base I2C read to ICM20948
	* @param: 	ICM20948 &chip 					== Core IMU struct
	* @param: 	unsigned char reg 				== Start address in DMP to read from
	* @param: 	uint32_t length 				== Length of data to be read (>1 for burst write)
	* @param: 	unsigned char *buff 			== Pointer to char array for data output
	* @return:	bool check						== True if error, false if OK
	* @type: 	BOTH
	* @note:	TESTED GOOD AS OF: DEC 19 2020
	*/
	bool read_reg(ICM20948_BASE &chip, unsigned char reg, uint32_t len, unsigned char *buff);

	/*
	* @name:	write_mems
	* @brief: 	Comms sequence for write to a DMP DRAM address
	* @param: 	ICM20948 &chip 					== Core IMU struct
	* @param: 	unsigned short reg 				== Start address in DMP to write to
	* @param: 	int32_t length 					== Length of data to be written (>1 for burst write)
	* @param: 	const unsigned char *data 		== Pointer to char array to be written to
	* @param: 	bool isImage			 		== Whether the pointer is to a location in ARDX ROM (true) or in system RAM (false)
	* @return:	bool check						== True if error, false if OK
	* @type: 	BOTH
	* @note:	TESTED GOOD AS OF: JAN 23 2021
	*/
	bool write_mems(ICM20948_BASE &chip, unsigned short reg, int32_t length, const unsigned char *data, bool isImage = false);

	/*
	* @name:	read_mems
	* @brief: 	Comms sequence for read from a DMP DRAM address
	* @param: 	ICM20948 &chip 					== Core IMU struct
	* @param: 	unsigned short reg 				== Start address in DMP to read from
	* @param: 	int32_t length 					== Length of data to be read (>1 for burst write)
	* @param: 	unsigned char 					== Pointer to char array for data output
	* @return:	bool check						== True if error, false if OK
	* @type: 	BOTH
	* @note:	TESTED GOOD AS OF: JAN 23 2021
	*/
	bool read_mems(ICM20948_BASE &chip, unsigned short reg, int32_t length, unsigned char *data);

	/*
	* @name:	write_mag
	* @brief: 	Comms sequence for write to a magnometer register
	* @param: 	ICM20948 &chip 					== Core IMU struct
	* @param: 	unsigned char addr 				== Address of the mag chip
	* @param: 	unsigned char reg 				== Register to write to
	* @param: 	unsigned char data 				== Single char of data to write
	* @return:	bool check						== True if error, false if OK
	* @type: 	BOTH
	* @note:	TESTED GOOD AS OF: JAN 24 2021
	*/
	bool write_mag(ICM20948_BASE &chip, unsigned char addr, unsigned char reg, unsigned char data);

	/*
	* @name:	read_mag
	* @brief: 	Comms sequence for read to magnometer registers
	* @param: 	ICM20948 &chip 					== Core IMU struct
	* @param: 	unsigned char addr 				== Address of the mag chip
	* @param: 	unsigned char reg 				== Register to start read from
	* @param: 	unsigned char len 				== Length of data to be read
	* @param: 	unsigned char *data 			== Pointer to char array for data output
	* @return:	bool check						== True if error, false if OK
	* @type: 	BOTH
	* @note:	TESTED GOOD AS OF: JAN 24 2021
	*/
	bool read_mag(ICM20948_BASE &chip, unsigned char addr, unsigned char reg, unsigned char len, unsigned char *data);


/////////////////////////////////////////////////////////////////////////////////////////////////
//											CHIP CTRL										   //
/////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	* @name:	fullPower
	* @brief: 	Control LP feature for digital circuitry
	* @param: 	ICM20948 &chip 					== Core IMU struct
	* @param: 	bool state						== State of LP bit (true = full power to digital, false = digital circuitry disable)
	* @return:	bool check						== True if error, false if OK
	* @type: 	BOTH
	* @note:	TESTED GOOD AS OF: JAN 20 2021
	*/
	bool fullPower(ICM20948_BASE &chip, bool state);

	/*
	* @name:	IMUSleep
	* @brief: 	Control sleep feature for analog/highPower circuitry
	* @param: 	ICM20948 &chip 					== Core IMU struct
	* @param: 	bool state						== State of sleep bit (true = full power to chip, false = circuitry disable)
	* @return:	bool check						== True if error, false if OK
	* @type: 	BOTH
	* @note:	TESTED GOOD AS OF: JAN 27 2021
	*/
	bool IMUSleep(ICM20948_BASE &chip, bool state);

	/*
	* @name:	setBank
	* @brief:	Set the ICM20948's bank (0-3)
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @param:	unsigned char bank 				== The bank to switch to
	* @return:	bool check						== True if error, false if OK
	* @type: 	BOTH
	*/
	bool setBank(ICM20948_BASE &chip, unsigned char bank);

	/*
	* @name:	getBankContents
	* @brief:	Get a printout of an entire ICM20948 bank
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @param:	int bank 						== Bank (0-3) to page through
	* @return:	bool check						== True if error, false if OK
	* @type: 	BOTH
	*/
	bool getBankContents(ICM20948_BASE &chip, int bank = 0);

	/*
	* @name:	updateCoreChipReset
	* @brief:	Check if any chips need to be reset, and if so, reset them (if core chip), set reset bytes (if controller chip), and update Wifi registers
	* @param:	struct ICM20948_BASE chips[10]	== Array of Core IMU structs
	* @param:	uint64_t &chipsReset 			== Reset bitmap
	* @param:	uint64_t &chipsReady			== Ready bitmap
	* @return:	bool check						== True if error, false if OK
	* @type		CORE
	*/
	bool updateCoreChipReset(struct ICM20948_BASE chips[CORE_CHIPS], uint64_t &chipsReset, uint64_t &chipsReady, uint64_t &chipsErrored);


	/*
	* @name:	updateControllerChipReset
	* @brief:	Check if any chips need to be reset, and if so, reset them
	* @param:	struct ICM20948_BASE chips[10]	== Array of Controller IMU structs
	* @param:	unsigned short &chipsReset		== Bitmask for the chips that need to get reset this round
	* @param: 	unsigned short &chipsReady		== The chips that have been reset now get initialized as ready
	* @param:	unsigned short &chipsError		== Bit set if chip fails reset
	* @return:	bool check						== True if error, false if OK
	* @type		CONTROLLER
	*/
	bool updateControllerChipReset(struct ICM20948_BASE chips[CONTROLLER_CHIPS], unsigned short &chipsReset, unsigned short &chipsReady, unsigned short &chipsError);


/////////////////////////////////////////////////////////////////////////////////////////////////
//										  IVORY PACKET										   //
/////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	* @name:	viewPacket
	* @brief:	Take a look at the 3-axis quat ouput table
	* @param:	long * finalBodyArray 			== Public array which holds ouput data
	* @param:	int getChip						== **optional** Get data from a specific sensor. If not set, defaults to show all sensor data
	* @return:	void
	* @type		CORE
	*/
	void viewPacket(long * finalBodyArray, int getChip = -1);

	/*
	* @name:	updateHandPacket
	* @brief: 	Get Chip Updates for SPI-based array and format them in a master-reacable packet
	* @param: 	struct ICM20948_BASE chips[10] 	== Array of Core IMU struct
	* @param:	unsigned char * HandArray		== Indevidual IMU outputs populate this array
	* @param:	unsigned short chipsEnabled		== Which chips are currently enebled by the master
	* @param:	unsigned short chipsReset		== Which chips are being reset
	* @param:	unsigned short chipsReady		== Which chips are ready
	* @param:	unsigned short chipsErrored		== Chips that have reported and error condition
	* @return:  int numChips					== The number of sensors that have updated in the ∆t 
	* @type		CONTROLLER
	*/
	int updateHandPacket(struct ICM20948_BASE chips[CONTROLLER_CHIPS], unsigned char * HandArray, unsigned short chipsEnabled, unsigned short chipsReset, unsigned short chipsReady, unsigned short chipsErrored);

	/*
	* @name:	updateSPIChip
	* @brief: 	Copy data from indevidual sensor register to body array 
	* @param: 	ICM20948_BASE &chip 			== Core IMU struct
	* @param: 	int quePos						== Body array sensor start position
	* @param:	unsigned char * HandArray		== Indevidual IMU outputs populate this array
	* @param:	unsigned char state				== Tells what to do (0 = quat data, 1 = send reset flag, 2 = send rdy flag)
	* @return:	void
	* @type		CONTROLLER
	*/
	void updateSPIChip(ICM20948_BASE &chip, int quePos, unsigned char * HandArray, unsigned char state);

	/*
	* @name:	pollSensor
	* @brief: 	Get Chip QUAT9 Update from FIFO through SPI
	* @param: 	ICM20948_BASE &chip 			== Core IMU struct
	* @param: 	unsigned char chipCounter 		== number ID of the chip
	* @param: 	unsigned short chipsEnabled 	== enabled chip bitmask
	* @return:	bool check						== True if error, false if OK
	* @type		CONTROLLER
	*/
	bool pollSensor(ICM20948_BASE &chip, unsigned char chipCounter, unsigned short chipsErrored);

	/*
	* @name:	getHandPacket
	* @brief:	Get the Quat9 data from each of the hands
	* @param:	I2CBank &i2c 					== One of two (L/R) hand structs that provide a buffer for the core
	* @param:	uint64_t &sensorEnable 			== Bitmap of the chips that are enabled for that hand
	* @param:	uint64_t &sensorReset 			== Bitmap of the chips that need to be reset
	* @param:	uint64_t &sensorReady 			== Bitmap of the chips that are now ready to send data
	* @param:	long * finalBodyArray 			== The return data from the hand (formatted with gaps)
	* @return:	void
	* @type		CORE
	*/
	void getHandPacket(I2CBank &i2c, uint64_t &sensorEnable, uint64_t &sensorReset, uint64_t &sensorReady, uint64_t &sensorErrored, long * finalBodyArray);

	/*
	* @name:	getCoreBodyArray
	* @brief:	Copy data from indevidual sensor array to body array 
	* @param:	struct ICM20948_BASE chips[16]	== Array of Core IMU structs
	* @param:	long * finalBodyArray			== Current data packet prototype
	* @param:	uint64_t EnSensorMask			== Sensor power/data state from host
	* @return:	void
	* @type		CORE
	*/
	void getCoreBodyArray(struct ICM20948_BASE chips[CORE_CHIPS], long * finalBodyArray, uint64_t EnSensorMask);


	/*
	* @name:	readCoreIMU
	* @brief:	Read IMU data from 
	* @param:	struct ICM20948_BASE chips[16]	== Array of Core IMU structs
	* @param:	long * finalBodyArray			== Current data packet prototype
	* @param:	uint64_t EnSensorMask			== Sensor power/data state from host
	* @return:	void
	* @type		CORE
	*/
	bool readCoreIMU(ICM20948_BASE &chip, long * finalBodyArray, uint64_t EnSensorMask, unsigned char sensorNumber);

	/*
	* @name:	dmp_get_fifo
	* @brief:	Perform hardware read of Ivory Packet containing:
	* -----------------------------------------------------------------------------------------------------------
	* | Size: (B)		In Pkt?		Data 				Description												|
	* -----------------------------------------------------------------------------------------------------------
	* | 2 bytes: 		ALWAYS		Header1      		Sensors enabled?										|
	* | 2 bytes: 		OPT 		Header2      		Appears if calibration status changes					|
	* |14 bytes:		ALWAYS		QUAT9 Output 		(3 * 4 bytes) 3 axis quat + 2 bytes of accuracy			|
	* | 2 bytes: 		OPT 		Accel Accuracy 		0-3, 0 is uncal, unused, 3 is cal, used in fusion		|
	* | 2 bytes: 		OPT 		Gyro Accuracy 		0-3, 0 is uncal, unused, 3 is cal, used in fusion		|
	* | 2 bytes: 		OPT 		Mag Accuracy 		0-3, 0 is uncal, unused, 3 is cal, used in fusion		|
	* -----------------------------------------------------------------------------------------------------------
	* @param:	ICM20948_BASE &chip 			== Core IMU struct
	* @param:	long * out_data 				== 3-Axis Quaternion output
	* @param:	bool chipEnabled 				== if the particular chip is currently enabled
	* @return:	bool check						== True if error, false if OK
	* @type		BOTH
	*/
	bool dmp_get_fifo(ICM20948_BASE &chip, long * out_data, bool chipWorking);


/////////////////////////////////////////////////////////////////////////////////////////////////
//											  MATH 											   //
/////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	* @name:	int32_to_big8
	* @brief: 	Convert 32 bit integer to unsigned char array
	* @param: 	int32_t input 					== Input data
	* @param: 	unsigned char *big8				== Pointer to char array
	* @return:	unsigned char *big8Out 			== POinter to output char array
	* @type		BOTH
	* @note:	TESTED GOOD AS OF: JAN 20 2021
	*/
	unsigned char *int32_to_big8(int32_t input, unsigned char *big8);

#endif
