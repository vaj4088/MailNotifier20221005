// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _MailNotifier_H_
#define _MailNotifier_H_
#include "Arduino.h"
/*
 * Needed for https POST access:
 *
 * #include <Arduino.h>             OK but with quotes instead of angle brackets
 * #include <ESP8266WiFi.h>         OK but twice already
 * #include <ESP8266HTTPClient.h>        Added below.
 * #include <WiFiClientSecureBearSSL.h>  Added below.
 *
 */

 #include <ESP8266HTTPClient.h>
 #include <WiFiClientSecureBearSSL.h>

//
//add your includes for the project MailNotifier here
//

//
// The following includes are used for OTA reprogramming.
//
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WiFi.h>
//
// End of "The following includes are used for OTA reprogramming".
//

//
// The following include is needed for WiFi network connection.
//
#include "ESP8266WiFi.h"

//
// Enabling this define allows the usage of ArduinoBearSSL without crypto chip.
//
//#define ARDUINO_DISABLE_ECCX08

//
// The following include is needed for secure HTTP (HTTPS).
//
// #include <WiFiClientSecure.h>
//#include <ArduinoBearSSL.h>

//
// Enabling this define allows the usage of ArduinoBearSSL without crypto chip.
//
#define ARDUINO_DISABLE_ECCX08

//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <AsyncElegantOTA.h>


//end of add your includes here.


//add your function definitions for the project MailNotifier here

void setupBody() ;
void loopBody() ;

void ConnectStationToNetwork(
		const char* encryptedNetworkName,
		const char* encryptedNetworkPassword
		) ;
boolean delayingIsDone(unsigned long &since, unsigned long time) ;
void httpGet(
		const char * server, const char * request="/", int port=80,
		int waitMillis = 3000
		) ;
void httpsPostForHomeAssistant(
		const char * server, const char * request="/", int port=443,
		int waitMillis = 3000
		) ;
void scanNetworkSynchronous() ;
void simpleDecrypt(const char *text) ;
void simpleEncrypt(const char *text) ;
void simpleErase(const char *text) ;
void stayHere() ;



//Do not add code below this line
#endif /* _MailNotifier_H_ */
