#include "FS.h"
#include "SD_MMC.h"
#include <ArduinoJson.h>

int clk = 36;
int cmd = 35;
int d0  = 37;
int d1  = 38;
int d2  = 33;
int d3  = 39; // GPIO 34 is not broken-out on ESP32-S3-DevKitC-1 v1.1

bool conficExist = false;

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file && !conficExist){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
            String configName = "config.json";
            if(configName.equals(file.name()))
            {
               Serial.println("Config found");
              conficExist= true;
            }
            
           /* if(file.name() == "config.json"){
            }*/
        }
        file = root.openNextFile();
    }
    if(!conficExist){
      createDir(SD_MMC, "/conf");
      writeJsonFile(SD_MMC, "/conf/config.json");
      conficExist = true;
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void readJsonFile(fs::FS &fs, const char *path) {
    Serial.printf("Reading JSON file: %s\n", path);

    if (!mountSDCard()) {
        return;
    }

    File file = fs.open(path);
    if (!file) {
        Serial.println("Failed to open JSON file for reading");
        return;
    }

    // Parse the JSON content
    DynamicJsonDocument doc(8192); // Adjust the size as needed
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
        Serial.print("Failed to parse JSON file: ");
        Serial.println(error.c_str());
        return;
    }

    // Access and print specific values from the JSON
    ssid = doc["ssid"].as<String>();
    password = doc["password"].as<String>();
    portalName = doc["portalName"].as<String>();
    portalPassword = doc["portalPassword"].as<String>();
    serverUrl = doc["serverUrl"].as<String>();
    GuestMode = doc["GuestMode"].as<bool>();
    wifiGood = doc["wifiGood"].as<bool>();

    LocationName = doc["LocationName"].as<String>();
    LocationPin = doc["LocationPin"].as<String>();
    CostomeMessage = doc["CostomeMessage"].as<String>();

    // Access the "devices" array
    JsonArray userArray = doc["users"].as<JsonArray>();
    JsonArray keyArray = doc["keys"].as<JsonArray>();

    // Iterate through users
    int userCount = 0;
    for (JsonObject user : userArray) {
        String username = user["username"].as<String>();
        String password = user["password"].as<String>();
        String category = user["category"].as<String>();
        users[userCount] = {username,password,category};
        // Handle each device as needed
        userCount++;
      }
      numUsers = userCount;

    // Iterate through users
    int keyCount = 0;
    for (JsonObject key : keyArray) {
        String code = key["code"].as<String>();
        int remoteNumber = key["remoteNumber"].as<int>();
        keys[keyCount] = {code,remoteNumber};
        // Handle each device as needed
        keyCount++;
      }
      numKeys = keyCount;

        
      MessageText = CostomeMessage+
      "\n Virtual Key for: "+LocationName+ ".\n"
      "Location Pin: "+LocationPin+".\n"
      "Instructions: \n Connect to Virtual Key Hotspot( "+portalName+" ), wait for redirect to Guest Portal and enter your Key Code, or connect to local Wifi Network( "+ssid+" ) and click on your key Link.\n";

      file.close();
      
      listDir(SD_MMC, "/", 0);
      // After performing data changes, unmount the SD card
      unmountSDCard();
}

bool writeJsonFile(fs::FS &fs, const char *path) { 


    if (!mountSDCard()) {
       return false;
    }

    Serial.printf("Writing JSON file: %s\n", path);
    // Create a JSON document
    DynamicJsonDocument doc(2048); // Adjust the size as needed
    doc["ssid"] = ssid;
    doc["password"] = password;
    doc["portalName"] = portalName; 
    doc["portalPassword"] = portalPassword;
    doc["serverUrl"] = serverUrl;
    doc["GuestMode"] = GuestMode;
    doc["wifiGood"] = wifiGood;

    doc["LocationName"] = LocationName;
    doc["LocationPin"] = LocationPin;
    doc["CostomeMessage"] = CostomeMessage;

    
    // Add array property "devices"
    JsonArray userArray = doc.createNestedArray("users");
    JsonArray keyArray = doc.createNestedArray("keys");

    // Example: Adding devices to the array
    
    for (int i = 0; i < numUsers; i++)
    { 
      JsonObject useri = userArray.createNestedObject();
      useri["username"] = users[i].username;
      useri["password"] = users[i].password;
      useri["category"] = users[i].category;
    }
    for (int i = 0; i < numKeys; i++)
    { 
      JsonObject keyi = keyArray.createNestedObject();
      keyi["code"] = keys[i].code;
      keyi["remoteNumber"] = keys[i].remoteNumber;
    }

    MessageText = CostomeMessage+
    "\n Virtual Key for: "+LocationName+ ".\n"
    "Location Pin: "+LocationPin+".\n"
    "Instructions: \n Connect to Virtual Key Hotspot( "+portalName+" ), wait for redirect to Guest Portal and enter your Key Code, or connect to local Wifi Network( "+ssid+" ) and click on your key Link.\n";

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open JSON file for writing");
        Serial.println("Attempting to fix SD card failure.");
        configSetup();//rerun setup
        return false;
    }

    // Serialize the JSON document to the file
    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write JSON to file");
        file.close();
        return false;
    } else {
        Serial.println("JSON written to file");
    }

    file.close();
    unmountSDCard();

    return true;
}

bool mountSDCard() {
    int attempts = 3;
    while (attempts-- && !sdActive) {
        if(SD_MMC.begin("/conf",true)){
           sdActive = true;
            Serial.println("SD Card mounted successfully");
            return true;
        } else {
            Serial.println("Card Mount Failed, retrying...");
            delay(1500); // Wait before retrying
        }
    }
    sdActive = false;
    return false;
}
void unmountSDCard() {
    SD_MMC.end();
    Serial.println("SD Card unmounted");
}
void configSetup(){
    readJsonFile(SD_MMC, "/conf/config.json");
}
