// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _MailNotifier_H_
#define _MailNotifier_H_
#include "Arduino.h"
//
//add your includes for the project MailNotifier here
//

//
// The following include is needed for WiFi network connection.
//
#include "ESP8266WiFi.h"

//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <AsyncElegantOTA.h>


//end of add your includes here


//add your function definitions for the project MailNotifier here

void ConnectStationToNetwork(
		const char* encryptedNetworkName,
		const char* encryptedNetworkPassword
		) ;
boolean delayingIsDone(unsigned long &since, unsigned long time) ;
void httpGet(
		const char * server, const char * request="/", int port=80,
		int waitMillis = 3000
		) ;
void scanNetworkSynchronous() ;
void simpleDecrypt(const char *text) ;
void simpleEncrypt(const char *text) ;
void simpleErase(const char *text) ;
void stayHere() ;



//Do not add code below this line
#endif /* _MailNotifier_H_ */
