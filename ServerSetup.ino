
#include "ServerSetup.h"
#include "esp_system.h"

// Handlers
static esp_err_t stream_handler(httpd_req_t *req){
 // if(!Authorized()){}
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];
   
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "Content-Type");

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      

      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
    //Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
  }
  return res;
}
// Handler for the root URL. 'http://192.168.137.180:81/stream'
esp_err_t root_handler(httpd_req_t *req) {
    char imageSrc[100];
    
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");  // Allow requests from any origin
    snprintf(imageSrc, sizeof(imageSrc), "http://%s:81/stream", WiFi.localIP().toString().c_str());

    if(GuestMode)
    {
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "Content-Type");
      String adminAccesstypes[] ={"superadmin","admin","resedant"};
      String accesstypes[] ={"stream","tenant","streamtenant","regular","guest"};
      if(authenticate_handler(req,adminAccesstypes,3)){
          Serial.println("Login for admin passed 1");
         httpd_resp_send(req, combinedPage(ssid,password,portalName,portalPassword,serverUrl,wifiGood,MessageText,domainName).c_str(), HTTPD_RESP_USE_STRLEN);
      }else{
         
          if(authenticate_handler(req,accesstypes,5)){
            Serial.println("Login for guest passed 1");
            httpd_resp_send(req, GuestPage().c_str(), HTTPD_RESP_USE_STRLEN);
            return ESP_OK;  
          }else{
            Serial.println("Login failed 1");
            httpd_resp_send(req, GuestLoginPage().c_str(), HTTPD_RESP_USE_STRLEN);


              // Set headers for captive portal detection
              httpd_resp_set_type(req, "text/html");
              httpd_resp_set_hdr(req, "Connection", "close");
              httpd_resp_set_hdr(req, "Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
              httpd_resp_set_hdr(req, "Pragma", "no-cache");
              httpd_resp_set_status(req, "302 Found");
            return ESP_OK;  
          }
      }
    }else{
            Serial.println("Not guesst mode 1");
      httpd_resp_send(req, combinedPage(ssid,password,portalName,portalPassword,serverUrl,wifiGood,MessageText,domainName).c_str(), HTTPD_RESP_USE_STRLEN);
    }  
    return ESP_OK;
}

esp_err_t resetBoard_handler(httpd_req_t *req) 
{
  String accesstypes[] ={{"superadmin"}};
  if(!authenticate_handler(req,accesstypes,1))
  {
      httpd_resp_send(req, "Access denied!", HTTPD_RESP_USE_STRLEN);
      return ESP_OK;
  }
  esp_cpu_reset(true);
  httpd_resp_send(req, "Restarting...... ", HTTPD_RESP_USE_STRLEN);
  return ESP_OK;
}
esp_err_t readSD_handler(httpd_req_t *req) 
{
  String accesstypes[] ={{"superadmin"}};
  if(!authenticate_handler(req,accesstypes,1))
  {
      httpd_resp_send(req, "Access denied!", HTTPD_RESP_USE_STRLEN);
      return ESP_OK;
  }
  readJsonFile(SD_MMC, "/conf/config.json");
  httpd_resp_send(req, "Reading...... ", HTTPD_RESP_USE_STRLEN);
  return ESP_OK;
}
bool authenticate_handler(httpd_req_t *req,String* categories,int cattegoriesSize) 
{
    if(!GuestMode)
    {
      return true;
    }
    if(GuestMode)
    {
      char query[100];
      if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK)
      {
          Serial.println("query started!!!!!!");
          char userName_buffer[64];
          char userPassword_buffer[64];
          if (httpd_query_key_value(query, "loginName", userName_buffer, sizeof(query)) == ESP_OK &&
              httpd_query_key_value(query, "loginPassword", userPassword_buffer, sizeof(query)) == ESP_OK)
          {
              //debug
              Serial.println(userName_buffer);
              Serial.println(userPassword_buffer);
              //check
              for (int i = 0; i < numUsers; ++i)
              {
                  Serial.println("checking details!!!!!");
                  Serial.println();
                  String myUserName(userName_buffer);
                  String myUserPassword(userPassword_buffer);

                  Serial.println(myUserName +" compare with: ");
                  Serial.println(users[i].username);
                  Serial.println(" And ");
                  Serial.println(myUserPassword+" compare with: ");
                  Serial.println(users[i].password);
                  if (myUserName==users[i].username && myUserPassword ==users[i].password) 
                  {
                    // autherizing
                    Serial.println("Authentication Susccess. Authorization started.");
                    for (int y = 0; y < cattegoriesSize; y++) {
                      Serial.println(users[i].category + " compared to ");
                      Serial.println(categories[y]);
                      if (users[i].category == categories[y]) {
                        // User is authorized, category found
                        Serial.println("Authorization Success.");
                        return true;
                      }
                    }
                  }
              }
           }
       }
    }
    return false;
}
// Handler for the "/toggle" URL
esp_err_t toggle_handler(httpd_req_t *req) {
    char query[100];
      String accesstypes[] ={"superadmin","admin","stream","tenant","streamtenant","resedant","regular","guest"};
    if(!authenticate_handler(req,accesstypes,8))
    {
          httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST,  "Access denied!");
        return ESP_OK;
    }
  
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        int light;
        if (httpd_query_key_value(query, "light", query, sizeof(query)) == ESP_OK) {
            light = atoi(query);

            // Handle the toggle for the corresponding light
             Serial.println("Togling light started:");
            switch (light) {
                case 1:
             Serial.println("Togling light : 1"+led1Pin);
                    digitalWrite(led1Pin, !digitalRead(led1Pin));
                    break;
                default:
                    break;
            }

            httpd_resp_send(req, "Light toggled", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    }
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST,  "Invalid request");
    return ESP_OK;
}
// Handler for the "/signal" URL
esp_err_t signal_handler(httpd_req_t *req) {
    char query[100];
      String accesstypes[] ={"superadmin","admin","stream","tenant","streamtenant","resedant","regular","guest"};
      if(!authenticate_handler(req,accesstypes,8))
      {
          httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST,  "Access denied!");
          return ESP_OK;
      }
      else if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        int remote;
        if (httpd_query_key_value(query, "remote", query, sizeof(query)) == ESP_OK) {
            remote = atoi(query);

            // Handle the toggle for the corresponding light
             Serial.println("remote started:");
            switch (remote) {
                case 1:
             Serial.println("using remote: 1"+remotePin1);
                    digitalWrite(remotePin1, HIGH);
                    delay(1000);
                    digitalWrite(remotePin1, LOW);
                    break;
                case 2:
             Serial.println("using remote: 2"+remotePin1);
                    digitalWrite(remotePin2, HIGH);
                    delay(1000);
                    digitalWrite(remotePin2, LOW);
                    break;
                case 3:
             Serial.println("using remote: 3"+remotePin1);
                    digitalWrite(remotePin3, HIGH);
                    delay(1000);
                    digitalWrite(remotePin3, LOW);
                    break;
                case 4:
             Serial.println("using remote: 4"+remotePin1);
                    digitalWrite(remotePin4, HIGH);
                    delay(1000);
                    digitalWrite(remotePin4, LOW);
                    break;
                default:
                    break;
            }

            httpd_resp_send(req, "Remote pressed", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    }

    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST,  "Invalid request");
    return ESP_OK;
}
// Handler for the "/connectx" URL
esp_err_t connect_handler(httpd_req_t *req) {
      String accesstypes[] ={{"superadmin"},{"admin"}};
      if(!authenticate_handler(req,accesstypes,2))
      {
          httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST,  "Access denied!");
          return ESP_OK;
      }
  connectToWifi();
  httpd_resp_send(req, "Attemting to connect to Wifi network ", HTTPD_RESP_USE_STRLEN);
  return ESP_OK;
}
esp_err_t saveWifi_handler(httpd_req_t *req) {
  
      String accesstypes[] ={{"superadmin"},{"admin"}};
      if(!authenticate_handler(req,accesstypes,2))
      {
          httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST,  "Access denied!");
          return ESP_OK;
      }
    char query[100];
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        char ssid_buffer[64];
        char domainName_buffer[64];
        char password_buffer[64];
        if (httpd_query_key_value(query, "ssid", ssid_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "domainName", domainName_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "password", password_buffer, sizeof(query)) == ESP_OK)
          {
            //debug
            Serial.println(ssid_buffer);
            Serial.println(domainName_buffer);
            Serial.println(password_buffer);

            // set
            ssid = ssid_buffer;
            domainName = domainName_buffer;
            password = password_buffer;
            //save
            
            if(writeJsonFile(SD_MMC, "/conf/config.json"))
            {
              httpd_resp_send(req, "New Credentials Saved to SD card", HTTPD_RESP_USE_STRLEN);
              return ESP_OK;
            }
            
            httpd_resp_send(req, "New Credential set. No SD card", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
         }
      }
          httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
      return ESP_OK;
    
 }
esp_err_t savePortal_handler(httpd_req_t *req) {
  
      String accesstypes[] ={{"superadmin"},{"admin"}};
      if(!authenticate_handler(req,accesstypes,2))
      {
          httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Access denied!");
          return ESP_OK;
      }
    char query[100];
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        char ssid_buffer[64];
        char password_buffer[64];
        char guestMode_buffer[64];
        if (httpd_query_key_value(query, "pname", ssid_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "ppassword", password_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "guestMode", guestMode_buffer, sizeof(query)) == ESP_OK)
         {
            //debug
            Serial.println(ssid_buffer);
            Serial.println(password_buffer);
            Serial.println(guestMode_buffer);
            // set
            portalName = ssid_buffer;
            portalPassword = password_buffer;
            GuestMode = guestMode_buffer;
            //save
            if(writeJsonFile(SD_MMC, "/conf/config.json"))
            {
              httpd_resp_send(req, "New Credentials Saved to SD card", HTTPD_RESP_USE_STRLEN);
              return ESP_OK;
            }
            httpd_resp_send(req, "New Credential set. No SD card", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    }
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
    return ESP_OK;
}
esp_err_t saveProfile_handler(httpd_req_t *req) {
      // to complete
    String accesstypes[] ={{"superadmin"},{"admin"},{"stream"},{"tenant"},{"streamtenant"},{"resedant"},{"regular"}};
    if(!authenticate_handler(req,accesstypes,7))
    {
        httpd_resp_send(req, "Access denied!", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }
      
    char query[100];
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        char newUserName_buffer[64];
        char oldUserName_buffer[64];
        char oldPassword_buffer[64];
        char newPassword_buffer[64];
        if (
            httpd_query_key_value(query, "newUserName", newUserName_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "loginName", oldUserName_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "oldPassword", oldPassword_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "newPassword", newPassword_buffer, sizeof(query)) == ESP_OK )
         {
            //debug
            Serial.println(newUserName_buffer);
            Serial.println(oldUserName_buffer);
            Serial.println(oldPassword_buffer);
            Serial.println(newPassword_buffer);
      
            String mynewUserName(newUserName_buffer);
            String myoldUserName(oldUserName_buffer);
            String myoldPassword(oldPassword_buffer);
            String mynewPassword(newPassword_buffer);
            // set
            for (int i = 0; i < numUsers; ++i) {
                if (myoldUserName==users[i].username && myoldPassword ==users[i].password ) {
                  users[i].username = mynewUserName;
                  users[i].password = mynewPassword;
                  return true;
                }
              }
            //save
            if(writeJsonFile(SD_MMC, "/conf/config.json"))
            {
              httpd_resp_send(req, "Profile Saved to SD card", HTTPD_RESP_USE_STRLEN);
              return ESP_OK;
            }
            httpd_resp_send(req, "Profile edited. No SD card", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    }
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
    return ESP_OK;
}
esp_err_t createKey_handler(httpd_req_t *req) {

    String accesstypes[] ={{"superadmin"},{"admin"},{"tenant"},{"resedant"}};
    if(!authenticate_handler(req,accesstypes,4))
    {
        httpd_resp_send(req, "Access denied!", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }
    char query[100];
    Serial.println("does actually serve .................");
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        char key_buffer[64];
        char remoteNumber_buffer[64];
        if (
            httpd_query_key_value(query, "key", key_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "remoteNumber", remoteNumber_buffer, sizeof(query)) == ESP_OK)
         {
            //debug
            Serial.println(key_buffer);
            Serial.println(remoteNumber_buffer);
      
            String myCode(key_buffer);
            int myRemoteNumber = atoi(remoteNumber_buffer);

            keys[numKeys] = {myCode,myRemoteNumber};
            
            numKeys++;

            //save
            if(writeJsonFile(SD_MMC, "/conf/config.json"))
            {
              httpd_resp_send(req, "Key Created and Saved to SD card", HTTPD_RESP_USE_STRLEN);
              return ESP_OK;
            }
            httpd_resp_send(req, "Key Created. No SD card", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    }
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
    return ESP_OK;
}

esp_err_t updateCustomMessage_handler(httpd_req_t *req) {

    String accesstypes[] ={{"superadmin"},{"admin"},{"tenant"},{"resedant"}};
    if(!authenticate_handler(req,accesstypes,4))
    {
        httpd_resp_send(req, "Access denied!", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }
    char query[100];
    Serial.println("does actually serve .................");
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        char msg_buffer[128];
        char propName_buffer[128];
        char locationPin_buffer[128];
        if (
            httpd_query_key_value(query, "msg", msg_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "propName", propName_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "locationPin", locationPin_buffer, sizeof(query)) == ESP_OK)
         {
            //debug
            Serial.println(msg_buffer);
            Serial.println(propName_buffer);
            Serial.println(locationPin_buffer);
      
            String myMsg(msg_buffer);
            String myPropName(propName_buffer);
            String myLocationPin(locationPin_buffer);

            CostomeMessage = myMsg;
            LocationName = myPropName;
            LocationPin = myLocationPin;
            // set
            //save
            if(writeJsonFile(SD_MMC, "/conf/config.json"))
            {
              httpd_resp_send(req, "Key Created and Saved to SD card", HTTPD_RESP_USE_STRLEN);
              return ESP_OK;
            }
            httpd_resp_send(req, "Key Created. No SD card", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    }
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
    return ESP_OK;
}

esp_err_t removeKey_handler(httpd_req_t *req) {
    String accesstypes[] = {"superadmin", "admin", "tenant", "resedant"};
    if (!authenticate_handler(req, accesstypes, 4)) {
        httpd_resp_send(req, "Access denied!", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }

    char query[100];
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        char code_buffer[64];
        if (httpd_query_key_value(query, "key", code_buffer, sizeof(query)) == ESP_OK) {
            String keyToRemove(code_buffer);

            // Search for the key to remove and shift remaining keys to fill the gap
            for (int i = 0; i < numKeys; ++i) {
                if (String(i) == keyToRemove) {
                    for (int j = i; j < numKeys - 1; ++j) {
                        keys[j] = keys[j + 1];
                    }
                    numKeys--;

                    // Save modified key array
                    if (writeJsonFile(SD_MMC, "/conf/config.json")) {
                        httpd_resp_send(req, "Key removed and saved to SD card", HTTPD_RESP_USE_STRLEN);
                        return ESP_OK;
                    }
                    httpd_resp_send(req, "Key removed but failed to save to SD card", HTTPD_RESP_USE_STRLEN);
                    return ESP_OK;
                }
            }

            // Key not found
            httpd_resp_send(req, "Key not found in the array", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    }
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
    return ESP_OK;
}
esp_err_t removeUser_handler(httpd_req_t *req) {
    String accesstypes[] = {"superadmin", "admin"};
    if (!authenticate_handler(req, accesstypes, 4)) {
        httpd_resp_send(req, "Access denied!", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }

    char query[100];
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        char code_buffer[64];
        if (httpd_query_key_value(query, "user", code_buffer, sizeof(query)) == ESP_OK) {
            String userToRemove(code_buffer);

            // Search for the key to remove and shift remaining keys to fill the gap
            for (int i = 0; i < numUsers; ++i) {
                if (String(i) == userToRemove) {
                    for (int j = i; j < numUsers - 1; ++j) {
                        users[j] = users[j + 1];
                    }
                    numUsers--;

                    // Save modified key array
                    if (writeJsonFile(SD_MMC, "/conf/config.json")) {
                        httpd_resp_send(req, "User removed and saved to SD card", HTTPD_RESP_USE_STRLEN);
                        return ESP_OK;
                    }
                    httpd_resp_send(req, "User removed but failed to save to SD card", HTTPD_RESP_USE_STRLEN);
                    return ESP_OK;
                }
            }

            // Key not found
            httpd_resp_send(req, "User not found in the array", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    }
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
    return ESP_OK;
}

esp_err_t createUser_handler(httpd_req_t *req) {
  
      String accesstypes[] ={{"superadmin"},{"admin"}};
      if(!authenticate_handler(req,accesstypes,2))
      {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Access denied!");
          return ESP_OK;
      }
    char query[100];
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        char newUser_buffer[64];
        char password_buffer[64];
        char category_buffer[64];
        if (
            httpd_query_key_value(query, "newUser", newUser_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "newUserPassword", password_buffer, sizeof(query)) == ESP_OK &&
            httpd_query_key_value(query, "category", category_buffer, sizeof(query)) == ESP_OK
            )
         {
            //debug
            Serial.println(newUser_buffer);
            Serial.println(password_buffer);
            Serial.println(category_buffer);
      
            String myUser(newUser_buffer);
            String mypass(password_buffer);
            String myCategory(category_buffer);
            // set
            users[numUsers] = {myUser, mypass, myCategory};
            numUsers++;
            //save
            if(writeJsonFile(SD_MMC, "/conf/config.json"))
            {
              httpd_resp_send(req, "User Created and Saved to SD card", HTTPD_RESP_USE_STRLEN);
              return ESP_OK;
            }
            httpd_resp_send(req, "User Created. No SD card", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    }
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
    return ESP_OK;
}
esp_err_t useKey_handler(httpd_req_t * req){
    char query[100];
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        char code_buffer[64];
        if ( httpd_query_key_value(query, "code", code_buffer, sizeof(query)) == ESP_OK)
            {
              
              String thisCode(code_buffer);
              for (int i = 0; i < numKeys; ++i) {
                if (thisCode.equals(keys[i].code)) {
                  digitalWrite(remotePin1, HIGH);
                  delay(2000);
                  digitalWrite(remotePin1, LOW);
                  httpd_resp_send(req, "Remote pressed", HTTPD_RESP_USE_STRLEN);
                  return ESP_OK;
                }
              }
        }
    }
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid Key Code");
    return ESP_OK;

}
esp_err_t login_handler(httpd_req_t *req) {
    char query[100];
    Serial.println("login started!!!!!!");
      String adminAccesstypes[] ={"superadmin","admin","resedant"};
      String accesstypes[] ={"stream","tenant","streamtenant","regular","guest"};
      
    if(GuestMode)
    {
      if(authenticate_handler(req,adminAccesstypes,3)){
          Serial.println("Login for admin passed 2");
         httpd_resp_send(req, combinedPage(ssid,password,portalName,portalPassword,serverUrl,wifiGood,MessageText,domainName).c_str(), HTTPD_RESP_USE_STRLEN);
      }else{
          if(authenticate_handler(req,accesstypes,5)){
            Serial.println("Login for guest passed 2");
            httpd_resp_send(req, GuestPage().c_str(), HTTPD_RESP_USE_STRLEN);
            return ESP_OK;  
          }
          else{
            Serial.println("Login failed 2");
            httpd_resp_send(req, GuestLoginPage().c_str(), HTTPD_RESP_USE_STRLEN);
            return ESP_OK;  
          }
      }
    }else{
            Serial.println("Not guesst mode 2");
      httpd_resp_send(req, combinedPage(ssid,password,portalName,portalPassword,serverUrl,wifiGood,MessageText,domainName).c_str(), HTTPD_RESP_USE_STRLEN);
    }  
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Access denied.");
    return ESP_OK;
}
// Handler for the "/restartPoertal" URL
esp_err_t restartPortal_handler(httpd_req_t *req) {
  String accesstypes[] ={{"superadmin"}};
  if(!authenticate_handler(req,accesstypes,1))
  {
      httpd_resp_send(req, "Access denied!", HTTPD_RESP_USE_STRLEN);
      return ESP_OK;
  }
  restartSoftAP();
  httpd_resp_send(req, "Restarting...... ", HTTPD_RESP_USE_STRLEN);
  return ESP_OK;
}
// paths
httpd_uri_t login_uri = {
    .uri = "/login",
    .method = HTTP_GET,
    .handler = login_handler,
    .user_ctx = NULL};
httpd_uri_t root_uri = {
    .uri = "/",
    //.uri = "/camServe",
    .method = HTTP_GET,
    .handler = root_handler,
    .user_ctx = NULL};
  httpd_uri_t redirect_uri = {
    .uri = "/redirect",
    //.uri = "/camServe",
    .method = HTTP_GET,
    .handler = root_handler,
    .user_ctx = NULL};
httpd_uri_t toggle_uri = {
    .uri = "/toggle",
    .method = HTTP_GET,
    .handler = toggle_handler,
    .user_ctx = NULL};
httpd_uri_t signalButton_uri = {
    .uri = "/signal",
    .method = HTTP_GET,
    .handler = signal_handler,
    .user_ctx = NULL};
httpd_uri_t index_uri = {
  .uri       = "/stream",
  .method    = HTTP_GET,
  .handler   = stream_handler,
  .user_ctx  = NULL};
httpd_uri_t connect_uri = {
  .uri       = "/connectx",
  .method    = HTTP_GET,
  .handler   = connect_handler,
  .user_ctx  = NULL};
httpd_uri_t restartPortal_uri = {
  .uri       = "/restartPortal",
  .method    = HTTP_GET,
  .handler   = restartPortal_handler,
  .user_ctx  = NULL};
httpd_uri_t saveWifi_uri = {
  .uri       = "/savewifi",
  .method    = HTTP_GET,
  .handler   = saveWifi_handler,
  .user_ctx  = NULL};
httpd_uri_t savePortal_uri = {
  .uri       = "/saveportal",
  .method    = HTTP_GET,
  .handler   = savePortal_handler,
  .user_ctx  = NULL};
httpd_uri_t createUser_uri = {
  .uri       = "/saveUser",
  .method    = HTTP_GET,
  .handler   = createUser_handler,
  .user_ctx  = NULL};
httpd_uri_t saveProfile_uri = {
  .uri       = "/saveProfile",
  .method    = HTTP_GET,
  .handler   = saveProfile_handler,
  .user_ctx  = NULL};
httpd_uri_t createKey_uri = {
  .uri       = "/createVirtualKey",
  .method    = HTTP_GET,
  .handler   = createKey_handler,
  .user_ctx  = NULL};
httpd_uri_t updateCustomMessage_uri = {
  .uri       = "/updateCustomMessage",
  .method    = HTTP_GET,
  .handler   = updateCustomMessage_handler,
  .user_ctx  = NULL};
httpd_uri_t removeKey_uri = {
  .uri       = "/removeVirtualKey",
  .method    = HTTP_GET,
  .handler   = removeKey_handler,
  .user_ctx  = NULL};
httpd_uri_t removeUser_uri = {
  .uri       = "/removeUser",
  .method    = HTTP_GET,
  .handler   = removeUser_handler,
  .user_ctx  = NULL};
httpd_uri_t useKey_url = {
  .uri       = "/useKey",
  .method    = HTTP_GET,
  .handler   = useKey_handler,
  .user_ctx  = NULL};
httpd_uri_t resetBoard_url = {
    .uri = "/reset",
    .method = HTTP_GET,
    .handler = resetBoard_handler,
    .user_ctx = NULL};
httpd_uri_t readSD_url = {
    .uri = "/readSD",
    .method = HTTP_GET,
    .handler = readSD_handler,
    .user_ctx = NULL};
httpd_uri_t random_url = {
    .uri = "/*",
    .method = HTTP_GET,
    .handler = root_handler,
    .user_ctx = NULL};
//*******


void startCameraServer(){
  
  // Initialize and start the http server
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  httpd_handle_t server;
  httpd_handle_t CamServer;
  config.max_uri_handlers = 21;
  if (httpd_start(&server, &config) == ESP_OK) {
      // Register URI handlers
      httpd_register_uri_handler(server, &saveProfile_uri);
      httpd_register_uri_handler(server, &createKey_uri);
      httpd_register_uri_handler(server, &useKey_url);
      httpd_register_uri_handler(server, &createUser_uri);
      httpd_register_uri_handler(server, &toggle_uri);
      httpd_register_uri_handler(server, &signalButton_uri);
      httpd_register_uri_handler(server, &connect_uri);
      httpd_register_uri_handler(server, &savePortal_uri);
      httpd_register_uri_handler(server, &restartPortal_uri);
      httpd_register_uri_handler(server, &login_uri);
      httpd_register_uri_handler(server, &root_uri);
      httpd_register_uri_handler(server, &saveWifi_uri);
      httpd_register_uri_handler(server, &removeKey_uri);
      httpd_register_uri_handler(server, &removeUser_uri);
      httpd_register_uri_handler(server, &updateCustomMessage_uri);
      httpd_register_uri_handler(server, &resetBoard_url);
      httpd_register_uri_handler(server, &readSD_url);
      httpd_register_uri_handler(server, &random_url);
  } else {
      Serial.println("Error starting server");
  }
    config.server_port += 1;
    config.ctrl_port += 1;
    config.max_uri_handlers = 2;

  Serial.printf("Starting web server on port: '%d'\n", config.server_port);
  if (httpd_start(&CamServer, &config) == ESP_OK) {
    httpd_register_uri_handler(CamServer, &index_uri);
  }
}
