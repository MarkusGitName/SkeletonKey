#include <WiFi.h>
#include <DNSServer.h>
#include <SD.h>

#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include "esp_http_server.h"
#include "html.h"
#include "ServerSetup.h"
#include "SDCntrol.h"

//******* global
String ssid = "MARKUS-10";
String password = "WorkingHard";
String portalName = "SK_testPortal";
String portalPassword = "SKAdminPass";
bool wifiGood =false;
bool sdActive =false;
String serverUrl= "";
String domainName= "SkelletonKey";
String LocationName ="Markus Home";
String LocationPin ="Markus Home";
String CostomeMessage ="";
String MessageText = CostomeMessage+
    "\n Virtual Key for: "+LocationName+ ".\n"
    "Location Pin: "+LocationPin+".\n"
    "Instructions: \n* Connect to Virtual Key Hotspot( "+portalName+" ),\n* wait for redirect to Guest Portal and enter your Key Code,\n* or connect to local Wifi Network( "+ssid+" ) \n* and click on your key Link.\n";
// users
struct User {
  String username;
  String password;
  String category; // Possible values: "admin", "guest", "regular", "stream", "tenant", "resedant"
};
struct Key {
 // String cellNumber;
  String code;
  int remoteNumber;
};
Key keys[50] {
};
User users[20] = {
    {"superadmin", "adminpass", "superadmin"},            // can do all
    {"admin", "adminpass", "admin"},                      // can do all and create non admin users
    {"stream", "streampass", "stream"},                   // stream video feed only(for designated streaming device)
    {"tenant", "tenantpass", "tenant"},                   // can create keys, and guests
    {"streamtenant", "streamtenantpass", "streamtenant"}, // stream video feed and controlls(for designated streaming device)
    {"resedant", "resedantpass", "resedant"},             // no setup, no user creation exept guest
    {"regular", "regularpass", "regular"},                // aregular can operate remotes
    {"guest", "guestpass", "guest"},                      // guests can operate all remotes, expire 20 uses or/or date as expiry
    // Add more users as needed
};
int numKeys = 0;
int numUsers = 8;
//

// ************* Settings
bool GuestMode = false;
// *************

// *******common methods
// deleting key and user
void removeElementAtIndex(int array[], int &arraySize, int indexToRemove) {
  // Check if the index is within valid bounds
  if (indexToRemove < 0 || indexToRemove >= arraySize) {
    // Invalid index, do nothing
    return;
  }

  // Shift elements after the target index to fill the gap
  for (int i = indexToRemove; i < arraySize - 1; i++) {
    array[i] = array[i + 1];
  }

  // Decrease the array size
  arraySize--;
}
// *******
// *************************
//Pins
int led1Pin = 4;  // Replace with the actual pin for light 1

int remotePin1 =1;  // Replace with the actual pin for remote 1
//int remotePin1 =12;  // Replace with the actual pin for remote 1

int remotePin2 = 3;  // Replace with the actual pin for remote 2
//int remotePin2 = 13;  // Replace with the actual pin for remote 2

int remotePin3 = 12;  // Replace with the actual pin for remote 3
int remotePin4 = 13;  // Replace with the actual pin for remote 4

//
#define PART_BOUNDARY "123456789000000000000987654321"

  #define PWDN_GPIO_NUM     32
  #define RESET_GPIO_NUM    -1
  #define XCLK_GPIO_NUM      0
  #define SIOD_GPIO_NUM     26
  #define SIOC_GPIO_NUM     27
  
  #define Y9_GPIO_NUM       35
  #define Y8_GPIO_NUM       34
  #define Y7_GPIO_NUM       39
  #define Y6_GPIO_NUM       36
  #define Y5_GPIO_NUM       21
  #define Y4_GPIO_NUM       19
  #define Y3_GPIO_NUM       18
  #define Y2_GPIO_NUM        5
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     23
  #define PCLK_GPIO_NUM     22

static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

void connectToWifi(){
  // Wi-Fi connection
  Serial.println("ssid: "+ssid);
  Serial.println("pass: "+password);
  WiFi.begin(ssid.c_str(), password.c_str());
  
  // Set hostname
  WiFi.setHostname("SkelletonKey");

  int count =0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;
    if(count > 50){
      Serial.print("Wifi connection Failed");
      break;
    }
  }
  wifiGood =false;
  if(WiFi.status() == WL_CONNECTED){

    // Start streaming web server
    wifiGood =true;
    Serial.println("");
    Serial.println("WiFi connected");
    serverUrl = "http://" + WiFi.localIP().toString();
   // serverUrl =  WiFi.localIP().toString();
    Serial.print("Camera Stream Ready! Go to: http://");
    Serial.print(WiFi.localIP());
  }
}

//******* Captive Portal varialbles
const byte DNS_PORT = 53;
IPAddress apIP(8,8,4,4); // The default android DNS
DNSServer dnsServer;
WiFiServer wifiserver(80);

bool sdPresent = false;
//*******

// goes in loop if wifi fails
void handleCaptivePortal(){
  dnsServer.processNextRequest();
  WiFiClient client = wifiserver.available();   // listen for incoming clients
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Access-Control-Allow-Origin:GET, *");
    client.println("Access-Control-Allow-Methods:GET, POST, OPTIONS");
    client.println("Access-Control-Allow-Header:Content-Type");
    client.println();
    client.print(LandinPage("http://"+WiFi.softAPIP().toString(),"http://"+serverUrl));
            
   client.stop();
  }
}
void restartSoftAP() {
  Serial.println("restarting Portal. Guest Mode? "+GuestMode);
  // Stop the existing soft access point
  WiFi.softAPdisconnect(true);

  // Stop the existing WiFiServer
  wifiserver.stop();

  // Stop the existing DNSServer
  dnsServer.stop();


    // WiFi mode is set to AP
  WiFi.mode(WIFI_AP);
  // Set the SSID and password for the soft access point
  // Restart the new soft access point with updated credentials
  if(GuestMode){
    WiFi.softAP(portalName.c_str());
  }else{
    WiFi.softAP(portalName.c_str(), portalPassword.c_str());
  }

  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  dnsServer.start(DNS_PORT, "*", apIP);

  wifiserver.begin();

  // If WiFi was previously connected, reconnect
  if (wifiGood) {
    connectToWifi();
  }
}
// if wifi connection fails
void SetupCaptivePortal(){

    // WiFi mode is set to AP
  WiFi.mode(WIFI_AP);
  // Set the SSID and password for the soft access point
  // Restart the new soft access point with updated credentials
  if(GuestMode){
    WiFi.softAP(portalName.c_str());
  }else{
    WiFi.softAP(portalName.c_str(), portalPassword.c_str());
  }

  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  dnsServer.start(DNS_PORT, "*", apIP);

  wifiserver.begin();
}

void CameraSetup(){

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

void loop() {
      handleCaptivePortal();
}

void setup() { 
  
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
 
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  configSetup();

  CameraSetup();
  SetupCaptivePortal();
  startCameraServer();
  
  // If WiFi was previously connected, reconnect
  if (wifiGood) {
    connectToWifi();
  }
  
  // Initialize pins for lights
  pinMode(led1Pin, OUTPUT);
  digitalWrite(led1Pin, LOW);
  // Initialize pins for remotes
  pinMode(remotePin1, OUTPUT);
  digitalWrite(remotePin1, LOW);
  pinMode(remotePin2, OUTPUT);
  digitalWrite(remotePin2, LOW);
  pinMode(remotePin3, OUTPUT);
  digitalWrite(remotePin4, LOW);
  pinMode(remotePin4, OUTPUT);
  digitalWrite(remotePin4, LOW);

}
