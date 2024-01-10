// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _MailNotifier_H_
#define _MailNotifier_H_
#include "Arduino.h"

#define requestBufferSize 256

//
// Uncomment exactly one of these #define lines:
//
#define Home
// #define Aiden
// #define Evan
//

//
// Define the channel to be used.  0 < Ian_Channel < 12
// Special Case: Ian_Channel = 0 means automatically search
//               for the appropriate channel.

/*
 * Needed for https POST access:
 *
 * #include <Arduino.h>             OK but with quotes instead of angle brackets
 * #include <ESP8266WiFi.h>         OK but twice already
 * #include <ESP8266HTTPClient.h>        Added below.
 * #include <WiFiClientSecureBearSSL.h>  Added below.
 *
 */

//
// triggerRequest is given a value in
// SSIDprivate.private or in
// SSIDprivate.h
// This define provides the parameters used by triggerRequest
//

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

#include <vector>
//end of add your includes here

//add your function definitions for the project MailNotifier20221005 here

// struct definitions added below.
struct header {
	char * headerDescription ;
	char * name ;
	char * value ;
};

struct request {
	char * overallDescription ;
	char * phoneNumDescription ;
	char * phoneNum ;
	char * deviceDescription ;
	char * device ;
	std::vector <struct header> headers ;
};



//add your function definitions for the project MailNotifier here

void setupBody() ;
void loopBody() ;

void ConnectStationToNetwork(
		const char* encryptedNetworkName,
		const char* encryptedNetworkPassword
		) ;
boolean delayingIsDone(unsigned long &since, unsigned long time) ;
//void httpGet(
//		const char * server, const char * request="/", int port=80,
//		int waitMillis = 3000
//		) ;
//void httpsPostForHomeAssistant(
//		const char * server, const char * request="/", int port=443,
//		int waitMillis = 3000
//		) ;
void scanNetworkSynchronous() ;
void simpleDecrypt(const char *text) ;
void simpleEncrypt(const char *text) ;
void simpleErase(const char *text) ;
void stayHere() ;



//Do not add code below this line
#endif /* _MailNotifier_H_ */
