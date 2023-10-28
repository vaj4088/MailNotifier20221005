
//
// The following include is standard practice.
//
#include "MailNotifier20221005.h"

//
// Ian_Channel < 0 OR Ian_Channel > 11 constitutes an error!
//
#define Ian_Channel 1

//
// Define the local socket to be used for debugging.
//
#define Ian_LocalDebugSocket 8080

//
// Define the local name or IP address to be used for debugging.
// Coyote2021Linux = 192.168.1.68
//

//
// Using a name worked early on but is inconsistent.
//
//#define Ian_LocalDebugAddress "Coyote2021Linux"
//

//
// Using an IP address seems to work consistently.
//
//
#if defined Home
// At home
#define Ian_LocalDebugAddress "192.168.1.94"
//
//
#elif defined Aiden
// At Aiden's
#define Ian_LocalDebugAddress "10.0.0.246"
//
//
#else
// At Evan's
#define Ian_LocalDebugAddress "192.168.1.246"
//
//
#endif
//
//
// Uncomment exactly one of these #define lines:
//
// #define Ian_debug2
 #define Ian_noDebug2

//
// Uncomment exactly one of these #define lines:
//
// #define Ian_debug3
 #define Ian_noDebug3

//
// Uncomment exactly one of these #define lines:
//
// #define Ian_debug4
 #define Ian_noDebug4

//
// Uncomment exactly one of these THREE #define lines:
//
// #define Ian_debug5_otaReprogramming
// #define Ian_debug5_notification
 #define Ian_noDebug5

//
// Uncomment exactly one of these #define lines:
//
// #define Ian_debug6
 #define Ian_noDebug6

#if defined Ian_debug4
WiFiClient debug;
#endif

/*
 * Wanted to access
 *
 * https://maker.ifttt.com/trigger/{event}/with/key/bBzMt3GMKR46GbTLP6v919
 *
 * to trigger ifttt.com to send a text to Ronni.
 *
 * {event} is Mail_Notifier
 *
 * so use
 *
 * https://maker.ifttt.com/trigger/Mail_Notifier/with/key/bBzMt3GMKR46GbTLP6v919
 *
 * Also, use function snprintf to convert a number
 * (as in the password) to ASCII.
 *
 *  Private Network DHCP Info for gateway
 *
 *  Range
 *        192.168.1.64 – 192.168.1.253
 *
 *  Current Internet Connection
 *  Type 	        Value
 *  IP Address 	    45.17.221.124
 *  Subnet Mask 	255.255.252.0
 *  Default Gateway 45.17.220.1
 *  Primary DNS 	68.94.156.9
 *  Secondary DNS 	68.94.157.9
 *  Host Name 	    dsldevice
 *  MAC Address 	f8:2c:18:e4:b8:40
 *  MTU 	        1500
 *
 *  Go to http://ivanzuzak.info/urlecho/
 *
 *  for information on how to use an
 *  HTTP service for echoing the HTTP response
 *  defined in the URL of the request.
 *
 */
//
//
//
ADC_MODE(ADC_VCC) ;  // Self VCC Read Mode
//
// NOTE that this line (above) must be OUTSIDE of any function.
//

// Variables will change:

boolean success ;
int     status  ;
char requestBuffer[requestBufferSize] ;

const double bVCalib = 0.00112016306998 ;

const char* ssid     = "*" ; // Replace * by the name (SSID) for your network.
const char* password = "*" ; // Replace * by the password    for your network.
const char* triggerRequest = "*" ; // Replace * by the request.

const unsigned long CONNECTION_WAIT_MILLIS = 5 * 1000UL ;
const byte pinNumber[] = {D0, D1, D2, D3, D4, D5, D6, D7} ;
//
// The internal pull up/down resistors have values of 30kΩ to 100kΩ,
// according to https://bbs.espressif.com/viewtopic.php?t=1079 .
//
// Avoid GIO0, GPIO2, and GPIO15 because these control boot mode.
// These correspond on an ESP8266 D1 Mini Pro to
// D3, D4, and D8.
// D2 is GPIO4 and may be the SDA line of I2C.
//
const byte otaProgrammingIndicator = D2 ;
enum executionType {
	normalExecution = HIGH,
	otaReprogrammingExecution = LOW
} executionMode ;
//
// The following declarations are used for OTA reprogramming.
//
ESP8266WebServer httpServer(80) ;
ESP8266HTTPUpdateServer httpUpdater ;

const char* otaHost = "MNOTA" ; // ESP8266 Mailbox Notifier OTA programming
const char * updateMessage =
"\nHTTP Update Server ready! Open http://%s/update in your browser.\n" ;
//
// End of "The following declarations are used for OTA reprogramming".
//

#define numberOfArrayElements(x) (sizeof(x)/sizeof(x[0]))

//
// Defined in SSIDprivate.h
// or in SSIDprivate.private
//
// const char* makerRequest = "..." ;
//

#if defined Home

IPAddress localIp( 192, 168,   1,  60) ;
IPAddress gateway( 192, 168,   1, 254) ;
IPAddress subnet ( 255, 255, 255,   0) ;
IPAddress dns1   (  68,  94, 156,   9) ;
IPAddress dns2   (  68,  94, 157,   9) ;

#elif defined Aiden

IPAddress localIp(   0,   0,   0,   0) ; // localIp, gateway and subnet are
IPAddress gateway(   0,   0,   0,   0) ; // 0.0.0.0 to indicate to
IPAddress subnet (   0,   0,   0,   0) ; // use DHCP.
IPAddress dns1   (   0,   0,   0,   0) ; // dns1 and dns2 will be set by DHCP.
IPAddress dns2   (   0,   0,   0,   0) ; // Ian_debug4 will cause assigned
                                         // addresses to be prnted.

/*
#elif defined Aiden

IPAddress localIp( 192, 168,   0, 160) ;
IPAddress gateway( 192, 168,   0,   1) ;
IPAddress subnet ( 255, 255, 255,   0) ;
IPAddress dns1   ( 192, 168,   0, 100) ;
IPAddress dns2   ( 192, 168,   0, 100) ;
*/
#else
// At Evan's
IPAddress localIp(   0,   0,   0,   0) ; // localIp, gateway and subnet are
IPAddress gateway(   0,   0,   0,   0) ; // 0.0.0.0 to indicate to
IPAddress subnet (   0,   0,   0,   0) ; // use DHCP.
IPAddress dns1   (   0,   0,   0,   0) ; // dns1 and dns2 will be set by DHCP.
IPAddress dns2   (   0,   0,   0,   0) ; // dns1 and dns2 will be set by DHCP.

#endif

void setupBody() {
	for (byte i = 0; i < numberOfArrayElements(pinNumber); i++) {
		pinMode(pinNumber[i], INPUT_PULLUP);
	}
	//
	// Make unit a station, and connect to network.
	//

	//
	// First, get the private encrypted strings.
	//
    // #include "SSIDprivate.private"
	//
	// __has_include is defined for gcc but probably not for other compilers.
	// This is the way gcc documentation says to use it safely.
	//
#if defined __has_include
#  if __has_include ("SSIDprivate.private")
#    include "SSIDprivate.private"
#  elif __has_include ("SSIDprivate.h")
#    include "SSIDprivate.h"
#  else
#    error "Missing SSIDprivate.h and SSIDprivate.private include file."
#  endif
#else
#  include "SSIDprivate.private"
#endif
	//
	// End of "Get the private encrypted strings.".
	//
	ConnectStationToNetwork(ssid, password);
	//
	// End of "Make unit a station, and connect to network.".
	//

	//
	// Erase the private encrypted strings.
	//
	simpleErase(ssid);
	simpleErase(password);
	//
	// End of "Erase the private encrypted strings."
	//

#if defined Ian_debug5_otaReprogramming
	executionMode = otaReprogrammingExecution ;

#elif defined Ian_debug5_notification
	executionMode = normalExecution ;

#else
	if (digitalRead(otaProgrammingIndicator) == normalExecution) {
		executionMode = normalExecution;
	} else {
		executionMode = otaReprogrammingExecution;
	}
#endif

	if (executionMode == normalExecution) {
		/*
		 * Wanted to access
		 *
		 * https://maker.ifttt.com/trigger/{event}/with/key/<IFTTT_Service_key>
		 *
		 * to trigger ifttt.com to send a text to Ronni.
		 *
		 * {event} is Mail_Notifier
		 *
		 * so use
		 *
		 https://maker.ifttt.com/trigger/Mail_Notifier/with/key/<IFTTT_Service_key>
		 *
		 */
		double batteryVoltage = ESP.getVcc() * bVCalib ;

#if defined Ian_debug4
		debug.connect(Ian_LocalDebugAddress, Ian_LocalDebugSocket);

		debug.printf("Start=============================================\n") ;
		debug.printf("Mail Notifier connected to %s at port %d.\n",
				Ian_LocalDebugAddress, Ian_LocalDebugSocket) ;
		debug.printf("Vcc is %u .\n", ESP.getVcc()) ;
		debug.printf("bVCalib is %.14f .\n", bVCalib) ;
		debug.printf("Battery voltage is %f volts.\n", batteryVoltage) ;
		debug.printf("Compiled on %s %s\n", __DATE__, __TIME__) ;
		debug.printf("triggerRequest: %s\nbatteryVoltage: %#.2f\n",
				triggerRequest, batteryVoltage) ;
		debug.printf("Execution mode is normal.\n") ;
		debug.printf("Assigned address is %s .\n",
				WiFi.localIP().toString().c_str() ) ;
		debug.printf("Gateway is %s .\n", WiFi.gatewayIP().toString().c_str()) ;
		debug.printf("DNS 0 is %s .\n", WiFi.dnsIP(0).toString().c_str()) ;
		debug.printf("DNS 1 is %s .\n", WiFi.dnsIP(1).toString().c_str()) ;
		debug.printf("DNS 2 is %s .\n", WiFi.dnsIP(2).toString().c_str()) ;
		debug.printf("Channel is %hd .\n", WiFi.channel() ) ;
		debug.printf("Subnet Mask is %s .\n",
				WiFi.subnetMask().toString().c_str()) ;
		debug.printf("End===============================================\n\n") ;
//		debug.printf("EOF_FOR_LOGGER\n") ;
//		debug.flush() ;
#endif
		//
		//
		//
		// Define a secure client.
		//
		std::unique_ptr<BearSSL::WiFiClientSecure>
		  client(new BearSSL::WiFiClientSecure) ;
		// Ignore SSL certificate validation
		client->setInsecure();
		// Create an HTTPClient instance for our POST request.
		HTTPClient https;

		// Set up the message to be sent.
		//
		snprintf(
				requestBuffer,
				requestBufferSize,
				triggerRequest,
				batteryVoltage
				) ;
		//Initializing HTTPS communication using the secure client
#if defined Ian_debug4
		debug.print("[HTTPS] begin...\n");
		if (https.begin(*client, requestBuffer)) {  // HTTPS
			debug.print("NOW going to ");
			debug.println(requestBuffer);
#  if !defined Ian_debug6
			debug.print("[HTTPS] POST...\n");
			// start connection and send HTTP header
			int httpCode = https.POST("");
			//
			//
			// HTTP header has been sent and
			// Server response header has been handled internally.
			//
			debug.printf("[HTTPS] POST... code: %d\n", httpCode);
			// file found at server
			if (
					httpCode == HTTP_CODE_OK ||
					httpCode == HTTP_CODE_MOVED_PERMANENTLY)
			{
				debug.println(https.getString());
			} else {
				debug.printf("[HTTPS] POST... failed, error: %s\n",
						https.errorToString(httpCode).c_str());
				debug.println(https.getString());
			}
#  endif
			debug.printf("EOF_FOR_LOGGER\n") ;
			debug.flush() ;
#else
			https.begin(*client, requestBuffer) ;  // HTTPS
#  if !defined Ian_debug6
			https.POST("");
#  endif
#endif

#if defined Ian_debug3
		scanNetworkSynchronous() ;
#endif

		ESP.deepSleepInstant(0, WAKE_RF_DEFAULT);
		//
		// Above is normal execution mode (notification), and
		// below is Over The Air (OTA) reprogramming.
		//
	} else {
		// REPROGRAM OTA (Over The Air) using a web browser !
		//		Serial.printf("\n\nOTA Reprogramming via a web browser !\n\n\n") ;
		//                   1         2         3         4
		//          1234567890123456789012345678901234567890

#if defined Ian_debug4
		debug.connect(Ian_LocalDebugAddress, Ian_LocalDebugSocket);

		debug.printf("Start=============================================\n") ;
		debug.printf("Mail Notifier connected to %s at port %d.\n",
				Ian_LocalDebugAddress, Ian_LocalDebugSocket) ;
		double batteryVoltage = ESP.getVcc() * bVCalib ;
		debug.printf("Vcc is %u .\n", ESP.getVcc()) ;
		debug.printf("bVCalib is %.14f .\n", bVCalib) ;
		debug.printf("Battery voltage is %f volts.\n", batteryVoltage) ;
		debug.printf("Compiled on %s %s\n", __DATE__, __TIME__) ;
		debug.printf("\n") ;
		debug.printf("W A R N I N G : In reprogramming mode.\n") ;
		debug.printf("W A R N I N G : In reprogramming mode.\n") ;
		debug.printf("W A R N I N G : In reprogramming mode.\n") ;
		debug.printf("\n") ;
		debug.printf(updateMessage, WiFi.localIP().toString().c_str());
		debug.printf("Assigned address is %s .\n",
				WiFi.localIP().toString().c_str() ) ;
		debug.printf("Gateway is %s .\n", WiFi.gatewayIP().toString().c_str()) ;
		debug.printf("DNS 0 is %s .\n", WiFi.dnsIP(0).toString().c_str()) ;
		debug.printf("DNS 1 is %s .\n", WiFi.dnsIP(1).toString().c_str()) ;
		debug.printf("DNS 2 is %s .\n", WiFi.dnsIP(2).toString().c_str()) ;
		debug.printf("Channel is %hd .\n", WiFi.channel() ) ;
		debug.printf("Subnet Mask is %s .\n",
				WiFi.subnetMask().toString().c_str()) ;
		debug.printf("End===============================================\n\n") ;
		debug.printf("EOF_FOR_LOGGER\n") ;
		debug.flush() ;
#endif

		MDNS.begin(otaHost);
		httpUpdater.setup(&httpServer);
		httpServer.onNotFound([]() {
			httpServer.send(200, "text/plain", "Go to /update.");
		}
		);
		httpServer.begin();
		MDNS.addService("http", "tcp", 80);
		Serial.printf(updateMessage, WiFi.localIP().toString().c_str());
	}
}

void loopBody() {
	//
	// Nothing to do for normal execution.
	// (Deep Sleep should keep us from getting here.)
	//
	// However, need to do some work for OTA reprogramming.
	//
	if (executionMode == otaReprogrammingExecution) {
		httpServer.handleClient();
		MDNS.update();
	}
}

//	  ESP.deepSleepInstant(microseconds, mode) will put the chip into deep sleep
//	  but sleeps instantly without waiting for WiFi to shutdown.
//	  mode is one of WAKE_RF_DEFAULT, WAKE_RFCAL, WAKE_NO_RFCAL,
//	  WAKE_RF_DISABLED.
//

void setup()
{
	setupBody();
}

void loop()
{
	loopBody();
}

boolean delayingIsDone(unsigned long &since, unsigned long time) {
  // return false if we're still "delaying", true if time ms has passed.
  // this should look a lot like "blink without delay"
  unsigned long currentmillis = millis();
  if (currentmillis - since >= time) {
    since = currentmillis;
    return true;
  }
  return false;
}

#if defined Ian_debug3
void scanNetworkSynchronous() {
	const char* formatString ;
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
	WiFi.scanDelete();
	Serial.println("Network scan starting.");
	int numberOfNetworks = WiFi.scanNetworks();
	if (numberOfNetworks == 1) {
		formatString = "%d network found.\n" ;
	} else {
		formatString = "%d networks found.\n" ;
	}
	Serial.printf(formatString, numberOfNetworks) ;
	for (int i = 0; i < numberOfNetworks; i++)
	{
		Serial.printf(
				"%d: %s, Ch:%d (%ddBm) %s\n", i+1, WiFi.SSID(i).c_str(),
				WiFi.channel(i), WiFi.RSSI(i),
				WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : ""
		);
	}
	WiFi.scanDelete();
	Serial.println();
}
#endif

void simpleEncrypt(const char *text) {
	char * textPointer = (char *)text ;
	Serial.print("Input:  ") ;
	Serial.print(textPointer) ;
	Serial.print(" ") ;
	for (char * i = (char *)text ; *i != 0 ; i++) {
		Serial.printf(" %#2.2hhX", *i) ;
	}
	Serial.println() ;
	byte index = 0 ;
	while(*textPointer) {
		*textPointer = 0x7F & (*textPointer + (3 & index++)) ;
		textPointer++ ;
	}

	Serial.print("Output: ") ;
	Serial.print(text) ;
	Serial.print(" {") ;
	char separator = ' ' ;
	for (char *i = (char *)text ; *i != 0 ; i++) {
		Serial.print(separator) ;
		Serial.printf(" %#2.2hhX", *i) ;
		separator = ',' ;
	}
	Serial.println("}") ;
	Serial.println() ;
}


void simpleDecrypt(const char *text) {
	char * textPointer = (char *)text ;
	byte index = 0 ;
	while(*textPointer) {
		*textPointer = 0x7F & (*textPointer - (3 & index++)) ;
		textPointer++ ;
	}
}

void simpleErase(const char *text) {
	char * textPointer = (char *)text ;
	while(*textPointer) {
		*textPointer = (byte)0 ;
		textPointer++ ;
	}
}

void stayHere() {
	while (true)
		yield();
}

void ConnectStationToNetwork(
		const char* encryptedNetworkName,
		const char* encryptedNetworkPassword
		) {
	char * writeableNetworkName     = (char *) encryptedNetworkName     ;
	char * writeableNetworkPassword = (char *) encryptedNetworkPassword ;

	//
	// Set up for station mode.
	//
	WiFi.mode(WIFI_STA) ;
	//
	// End of "Set up for station mode."
	//

	//
	// Configure for network.
	//
	success = WiFi.config(localIp, gateway, subnet, dns1, dns2);
	if (!success) {
		Serial.println("Could not configure.");
		stayHere();
	}

	//
	// Decrypt the private strings.
	//
	simpleDecrypt(writeableNetworkName    );
	simpleDecrypt(writeableNetworkPassword);
	//
	// End of "Read and decrypt the private strings."
	//

	//
	// End of "Configure for network."
	//


	//
	// Connect to network.
	//

	// attempt to connect to Wifi network:

	while (WiFi.status() != WL_CONNECTED) {
		unsigned long connectionStart = millis() ;

#ifdef Ian_Channel
	#if (Ian_Channel > 0) && (Ian_Channel < 12)
			WiFi.begin(
					writeableNetworkName,
					writeableNetworkPassword,
					Ian_Channel
			);
		#elif (Ian_Channel == 0)
			WiFi.begin(
					writeableNetworkName,
					writeableNetworkPassword
			);
		#else
			#error "Ian_Channel has bad value,"
			#errror "should be an integer in the range [0, 11]"
	#endif /* if (Ian_Channel > 0) && (Ian_Channel < 12) */
	#else
		#error "Ian_Channel should be defined and is not."
#endif /* ifdef Ian_Channel */

		// timed wait for connection
		while (
				(WiFi.status() != WL_CONNECTED) &&
				( (millis()-connectionStart) < CONNECTION_WAIT_MILLIS)
		){
			yield() ;
		}
		unsigned long connectionEnd = millis() ;
		unsigned long connectionTime = connectionEnd - connectionStart ;
		if (connectionTime < CONNECTION_WAIT_MILLIS) {
#if defined Ian_debug2
			Serial.printf(
				"DEBUG >>>>>>>>  Connection took %lu milliseconds.\n",
				connectionTime
			) ;
// https://www.arduino.cc/en/Reference/WiFiRSSI
			#endif
		} else {
#if defined Ian_debug2
			Serial.printf(
				"DEBUG >>>>>>>>  Failed to connect in %lu milliseconds.\n",
				connectionTime
			) ;
#endif
			WiFi.disconnect() ;  //  Reset and try again.
		}
	}
	/*
	typedef enum {
    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_DISCONNECTED     = 6
} wl_status_t;
	 */
	//
	// Erase the private strings.
	//
	simpleErase(writeableNetworkName    );
	simpleErase(writeableNetworkPassword);
	//
	// End of "Erase the private strings."
	//

	status = WiFi.status();
	switch (status) {
	case WL_CONNECTED:
#if defined Ian_debug2
		Serial.println("Successful network connection.");
#endif
		break;
	case WL_NO_SSID_AVAIL:
		Serial.print("Failed to connect to network because ") ;
		Serial.println("configured SSID (network name) could not be reached.") ;
		break;
	case WL_CONNECT_FAILED:
		Serial.print("Failed to connect to network ") ;
		Serial.println("because password is incorrect.") ;
		break;
	case WL_IDLE_STATUS:
		Serial.println("WiFi status is changing.");
		break;
	case WL_DISCONNECTED:
		Serial.println("Failed to connect to network because") ;
		Serial.print("a specific MAC ID (BSSID) was requested ") ;
		Serial.println("that is not available, or") ;
		Serial.println("wait time was not long enough, or") ;
		Serial.println("unit is not configured for station mode.");
		break;
	default:
		Serial.println("Failed to connect to network due to unknown reason.");
	}
	if (status != WL_CONNECTED) {
		stayHere();
	}
}
