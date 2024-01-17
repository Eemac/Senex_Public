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

#ifndef _SENEX_WIRELESS_H
#define _SENEX_WIRELESS_H

//Basic Environment Functionality
#include "Arduino.h"

//Access to the ESP32 WiFi stack
#include <esp_WiFi.h>

//UDP Sensor broadcast
#include <WiFiUdp.h>

//Multicast Beacon for the Suit's search mode
#include <AsyncUDP.h>

//Multiple wifi networks
#include <WiFiMulti.h>

//OTA updates for CORE chip
#include <ArduinoOTA.h>

//Suit-Wide control settings
#include "Senex_Settings.h"


//Task Delays
#define DEBUG_SEND_DELAY 50
#define FAST_UDP_DELAY 16
#define UDP_STREAM_DELAY 16


typedef struct S_IO
{
    /* ctrl_1:
    * ----------------------------------------------------------------------------------------------------------
    * |  Bit  |   Meaning                                                                       |  Functional  |
    * ----------------------------------------------------------------------------------------------------------
    * |   7   |   Full system reset bit 0 - set to 1 for reset.                                 |      X       |
    * |   6   |   Full system reset bit 1 - set to 1 for reset.                                 |      X       |
    * |   5   |   Set bit to reset right hand slave controller.                                 |      X       |
    * |   4   |   Set bit to reset left hand slave controller.                                  |      X       |
    * |   3   |   IMU Suit EEPROM bias reset - set to 1 for reset.                              |      X       |
    * |   2   |   IMU right hand EEPROM bias reset - set to 1 for reset.                        |      X       |
    * |   1   |   IMU left hand EEPROM bias reset - set to 1 for reset.                         |      X       |
    * |   0   |   Set bit to place the system into a lower power state.                         |      X       |
    * ----------------------------------------------------------------------------------------------------------
    */

    unsigned char ctrl_1;

    /* ctrl_2:
    * ----------------------------------------------------------------------------------------------------------
    * |  Bit  |   Meaning                                                                       |  Functional  |
    * ----------------------------------------------------------------------------------------------------------
    * |   7   |   Set bit to enable Sleep Mode.                                                 |              |
    * |   6   |   Set bit to enable LED functionality.                                          |              |
    * |   5   |   Set bit to enable h***** engine.                                              |              |
    * |   4   |   Set bit to enable ESP32 quaternion postprocessing.                            |              |
    * |   3   |   External device (1) enable.                                                   |              |
    * |   2   |   External device (2) enable.                                                   |              |
    * |   1   |   External device (3) enable.                                                   |              |
    * |   0   |   External device (4) enable.                                                   |              |
    * ----------------------------------------------------------------------------------------------------------
    */
    unsigned char ctrl_2;

    /* ctrl_3:
    * ----------------------------------------------------------------------------------------------------------
    * |  Bit  |   Meaning                                                                       |  Functional  |
    * ----------------------------------------------------------------------------------------------------------
    * |   7   |   Set bit to enable Waypoint ping connectivity.                                 |              |
    * |   6   |   Set bit to enable Waypoint data recieving.                                    |              |
    * |   5   |   Set bit to enable WiFi.                                                       |              |
    * |   4   |   Set bit to force WiFi connection to single (preffered) network.               |              |
    * |   3   |   WiFi debug enable bit.                                                        |              |
    * |   2   |   IMU debug enable bit.                                                         |              |
    * |   1   |   I/O debug enable bit.                                                         |              |
    * |   0   |   H****** debug enable bit.                                                     |              |
    * ----------------------------------------------------------------------------------------------------------
    */
    unsigned char ctrl_3;

    //Eight byte unique chip ID
    uint32_t chipID;

    /* Chip States:
    * -----------------------------------------------------------------------------------------------
    * |   imu_en  |  imu_rdy  |  imu_cal  |  Meaning                                                |
    * -----------------------------------------------------------------------------------------------
    * |     0     |     0     |     0     |  IMU reset/powerOn state - not init, no data stream     |
    * |     0     |     0     |     1     |  Chip is initing or calibrating                         |
    * |     1     |     0     |     0     |  Chip is enabled, but not inited - will calibrate       |
    * |     1     |     0     |     1     |  Chip is initing + will send data when it has finished  |
    * |     1     |     1     |     0     |  Chip is ready and sending data                         |
    * -----------------------------------------------------------------------------------------------
    */
 	//IMU Enable bitmask (bits 0-35 used)
    //I2C BitMask:          0x000000000000FFFF
    //Right Hand BitMask:   0x0000000003FF0000
    //Left Hand BitMask:    0x0000000FFC000000
    //All Sensors:          0x0000000FFFFFFFFF
    //No Sensors:           0x0000000000000000
    uint64_t imu_en;

    //If IMUs are ready + calibrated (bits 0-35 used)
    uint64_t imu_rdy;

    //IMU Reset output bitmask (bits 0-35 used)
    uint64_t imu_cal;

    //IMU Reset output bitmask (bits 0-35 used)
    uint64_t imu_cot;

    uint64_t imu_cal_shadow;
    long calTimer;

    //IMU Error bitmask (bits 0-35 used)
    uint64_t imu_err;

    //CPU Frequency (normal = 240Mhz)
    unsigned char freqCpu;

    //Simple bank to control LEDS
    unsigned char led_bank[8][3]; 

    //For more advanced LED scenes
    unsigned char led_ctrl;

    //H***** bank
    unsigned char bank[130];

    //Ctrl registers for 4 external devices
    unsigned char eCtrl[4][4];

    unsigned char ip[4];
    //If client verified or not
    bool clientState;
    bool UIDVerify;

    //Boolean value (true = send UDP packet)
    bool doStream;

    //long - time when the user last contacted the suit...
    long lastHeard;

    //long - when we last sent a data packet (sued for keeplalive)
    long keepAliveTimer;

    //Are we currently connected to a network?
    bool WiFiConnected;

    //Keeps track of how long the suit has been operational and provides timestamping for recordings
    unsigned long suitTimer;

    //Stores prototype data packet as data is added
    long finalBodyArray[105] = {0};

    //String for debug printouts
    String logData;

    //UDP does not recollect packets in any order, this number specifies which ones come first
    bool firstPkt;

    //Send packet data or disable stream to catch whole message
    bool burnPacket;

    //Allow firing of UDP log
    unsigned long logTimeoutTimer;

    //For fast data writes
    unsigned long forcePacketSendTimer;

    long packetOrderNumber;

    //Init of fastUDP socket listener
    bool initFastUDP;
} S_IO;

#include "Senex_Photonics.h"
#include "Senex_Vibe.h"

#ifdef WIFI
    extern I2CBank left_i2c;
    extern I2CBank right_i2c;
#endif

void runWifi(void * pvParameters);
void streamPacket(void * pvParameters);
void streamDebugInfo(void * pvParameters);
void fastUDPChannel(void * pvParameters);

void logPrint(S_IO &wirelessIO, String text);
void logPrint(String text);
void logPrintln(S_IO &wirelessIO, String text);
void logPrintln(String text);

class S_Wireless
{
    public:
        S_Wireless(void);

        void wifiSetup(S_IO &wirelessIO);
        void updateWiFi(S_IO &wirelessIO);
        void debugSendHelper(S_IO &wirelessIO);
        void fastUDPChannelHelper(S_IO &wirelessIO);

    private:
};

#endif 
