// @file WebServer.ino
// @brief Example implementation using the ESP8266 WebServer.
//
// See also README.md for instructions and hints.
//
// Changelog:
// 21.07.2021 creation, first version

#include <Arduino.h>
#include <Arduino_JSON.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <neotimer.h>
#include <vector>
#include <sstream>
#include "secrets.h"  // add WLAN Credentials in here.
   // File System for Web Server Files
#include <LittleFS.h>  // This file system is used.

// mark parameters not used in example
#define UNUSED __attribute__((unused))

// TRACE output simplified, can be deactivated here
#define TRACE(...) Serial.printf(__VA_ARGS__)

// name of the server. You reach it using http://webserver
#define HOSTNAME "webserver"

// local time zone definition (Berlin)
#define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3"

// need a WebServer for http access on port 80.
ESP8266WebServer server(80);

// The text of builtin files are in this header file
#include "builtinfiles.h"

using namespace std;

int ledStatus = 1;

int mostureSensor = 0;

bool ledOn = true;

bool pumpOff = true;

bool sensorOff = true;

int moistVal = 0;

int airValue = 560;

int waterValue = 210;

int intervals = (airValue - waterValue)/3;

const int second = 1000;

const int day = 3600 * second * 24;

int wateringTime = 10;

const int checkTime = day;

//const char* weekDays[] = {"Sunday", "Monday", "Tuesday","Wednesday", "Thursday", "Friday", "Saturday"};

// Global variables:-

//char* selectedWeekDays[7] = {};

vector<String> weekDays = {"Sunday", "Monday", "Tuesday","Wednesday", "Thursday", "Friday", "Saturday"};

vector<String> selectedWeekDays(7);

Neotimer wateringTimer;

Neotimer weekDayCheck;

Neotimer displayDataSensorTimer;
// ===== Simple functions used to answer simple GET requests =====

void writeFile(const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  bool fileExists = LittleFS.exists(path);

  if(fileExists){
  Serial.println("File alread exists");
  readFile(path);
  return;  
  }

  File file = LittleFS.open(path, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  delay(2000);  // Make sure the CREATE and LASTWRITE times are different
  file.close();
}

void deleteFile(const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (LittleFS.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void appendFile(const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  String str = String(message) + "\n";

  File file = LittleFS.open(path, "a");
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }

  if (file.print(str.c_str())) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }

  file.close();
}

void readFile(const char *path) {
  Serial.printf("Reading file: %s\n", path);

  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) { Serial.write(file.read()); }
  file.close();
}

bool checkWeekDay(String str) {

  File file = LittleFS.open("/setup.txt", "r");
  if (!file) {
    Serial.println("Failed to open file Setup for reading");
    return false;
  }
  String data = file.readString();

  string convertion = data.c_str();
  
  if (convertion.find(str.c_str()) != string::npos)
  {
    Serial.println("Found");
    file.close();
    return true;
  }else
  {
    Serial.println("Not Found");
  }
  file.close();
  return false;
}

// This function is called when the WebServer was requested without giving a filename.
// This will redirect to the file index.htm when it is existing otherwise to the built-in $upload.htm page
void handleRedirect() {
  TRACE("Redirect...");
  String url = "/index.htm";

  if (!LittleFS.exists(url)) { url = "/$update.htm"; }

  server.sendHeader("Location", url, true);
  server.send(302);
}  // handleRedirect()

// This function is called when the WebServer was requested to list all existing files in the filesystem.
// a JSON array with file information is returned.
void handleListFiles() {
  Dir dir = LittleFS.openDir("/");
  String result;

  result += "[\n";
  while (dir.next()) {
    if (result.length() > 4) { result += ","; }
    result += "  {";
    result += " \"name\": \"" + dir.fileName() + "\", ";
    result += " \"size\": " + String(dir.fileSize()) + ", ";
    result += " \"time\": " + String(dir.fileTime());
    result += " }\n";
    // jc.addProperty("size", dir.fileSize());
  }  // while
  result += "]";

  Serial.println(result);
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
}  // handleListFiles()


// This function is called when the sysInfo service was requested.
void handleSysInfo() {
  String result;

  FSInfo fs_info;
  LittleFS.info(fs_info);

  result += "{\n";
  result += "  \"flashSize\": " + String(ESP.getFlashChipSize()) + ",\n";
  result += "  \"freeHeap\": " + String(ESP.getFreeHeap()) + ",\n";
  result += "  \"fsTotalBytes\": " + String(fs_info.totalBytes) + ",\n";
  result += "  \"fsUsedBytes\": " + String(fs_info.usedBytes) + ",\n";
  result += "}";

  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
}  // handleSysInfo()


void ledStatusInfo() {
  String result;

  result += String(ledStatus);

  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
}  // handleSysInfo()

void ledStateCommand() {
  ledOn = !ledOn;
  String result;
  result += "The led is ";
  result += ledOn ? "on" : "off";

  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
}

void weekDaysQuery() {

  String result;

  File file = LittleFS.open("/setup.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    file.close();
  }else {
    result = file.readString();
    file.close();
  }
  // serialize the array and send the result to Serial
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
}

void readMostureSensor() {

  mostureSensor = analogRead(A0);
  String result;
  result += mostureSensor;

  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
}

void pumpStateCommand() {
  
  pumpOff = !pumpOff;
  if(!pumpOff){
    appendFile("/lastwatering.txt", GetDateTimeApi().c_str()); 
  }

  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", String(pumpOff));
}

void selectDaysCommand() {
  if (server.method() != HTTP_POST) {
    
    server.send(405, "text/plain", "Method Not Allowed");
    
  } else {
    
    //String message = "POST form was:\n" + server.arg("plain");


    String request = "{}";

    request = server.arg("plain");

    JSONVar myObject = JSON.parse(request);

    // JSON.typeof(jsonVar) can be used to get the type of the variable
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }

    Serial.print("JSON.typeof(myObject) = ");
    Serial.println(JSON.typeof(myObject));

    selectedWeekDays.clear();

    for (int i = 0; i < myObject.length(); i++) {
      String value = myObject[i];
      selectedWeekDays.push_back(value);
    }
    
    deleteFile("/setup.txt");

    writeFile("/setup.txt", "");

    for (int j = 0; j < selectedWeekDays.size(); j++) {
        appendFile("/setup.txt", selectedWeekDays[j].c_str()); 
    }
    //for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg("plain") + "\n"; }
    server.send(200, "text/plain", request);    
  }
}

void wateringTimeCommand() {
  if (server.method() != HTTP_POST) {
    
    server.send(405, "text/plain", "Method Not Allowed");
    
  } else {
    
  
    String request = "{}";

    request = server.arg("plain");

    JSONVar myObject = JSON.parse(request);

    // JSON.typeof(jsonVar) can be used to get the type of the variable
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }

    Serial.print("JSON.typeof(myObject) = ");
    Serial.println(JSON.typeof(myObject));

    wateringTime = myObject;

    deleteFile("/wateringtime.txt");

    writeFile("/wateringtime.txt", String(wateringTime).c_str());

    server.send(200, "text/plain", String(wateringTime));    
  }
}

void wateringTimeQuery() {
  
  String result;

  File file = LittleFS.open("/wateringtime.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    file.close();
  }else {
    result = file.readString();
    file.close();
  }
  // serialize the array and send the result to Serial
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
}

void wateringTimeUpdate() {
  
  String result;

  if (server.method() != HTTP_GET) {
    
    server.send(405, "text/plain", "Method Not Allowed");
    
  } else {
    
    String request = "{}";

    wateringTime++;

    deleteFile("/wateringtime.txt");

    writeFile("/wateringtime.txt", String(wateringTime).c_str());

    server.send(200, "text/plain", String(wateringTime));    
  }
}

void sensorStateUpdate() {
  
  String result;

  sensorOff = !sensorOff;

  if (server.method() != HTTP_GET) {
    
    server.send(405, "text/plain", "Method Not Allowed");
    
  } else {
    
    String request = "{}";

    deleteFile("/sensor.txt");

    writeFile("/sensor.txt", String(sensorOff).c_str());

    server.send(200, "text/plain", String(sensorOff));
  }
}

void getSensorState() {
  
  String result;

  File file = LittleFS.open("/sensor.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    file.close();
  }else {
    result = file.readString();
    file.close();
  }
  // serialize the array and send the result to Serial
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
}

void getLastWatering() {
  
  String result;

  File file = LittleFS.open("/lastwatering.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    file.close();
  }else {
    result = file.readString();
    file.close();
  }
  // serialize the array and send the result to Serial
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/javascript; charset=utf-8", result);
}

void setWateringTimeFromFile(){
  String result;

  File file = LittleFS.open("/wateringtime.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    file.close();
  }else {
    result = file.readString();
    file.close();
    wateringTime = result.toInt();
  }
}

void setSensorFromFile(){

  String result;

  File file = LittleFS.open("/sensor.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    file.close();
  }else {
    result = file.readString();
    file.close();
    sensorOff = result == "0" ? false : true;
  }
}

// ===== Request Handler class used to answer more complex requests =====

// The FileServerHandler is registered to the web server to support DELETE and UPLOAD of files into the filesystem.
class FileServerHandler : public RequestHandler {
public:
  // @brief Construct a new File Server Handler object
  // @param fs The file system to be used.
  // @param path Path to the root folder in the file system that is used for serving static data down and upload.
  // @param cache_header Cache Header to be used in replies.
  FileServerHandler() {
    TRACE("FileServerHandler is registered\n");
  }


  // @brief check incoming request. Can handle POST for uploads and DELETE.
  // @param requestMethod method of the http request line.
  // @param requestUri request ressource from the http request line.
  // @return true when method can be handled.
  bool canHandle(HTTPMethod requestMethod, const String UNUSED &_uri) override {
    return ((requestMethod == HTTP_POST) || (requestMethod == HTTP_DELETE));
  }  // canHandle()


  bool canUpload(const String &uri) override {
    // only allow upload on root fs level.
    return (uri == "/");
  }  // canUpload()


  bool handle(ESP8266WebServer &server, HTTPMethod requestMethod, const String &requestUri) override {
    // ensure that filename starts with '/'
    String fName = requestUri;
    if (!fName.startsWith("/")) { fName = "/" + fName; }

    if (requestMethod == HTTP_POST) {
      // all done in upload. no other forms.

    } else if (requestMethod == HTTP_DELETE) {
      if (LittleFS.exists(fName)) { LittleFS.remove(fName); }
    }  // if

    server.send(200);  // all done.
    return (true);
  }  // handle()


  // uploading process
  void upload(ESP8266WebServer UNUSED &server, const String UNUSED &_requestUri, HTTPUpload &upload) override {
    // ensure that filename starts with '/'
    String fName = upload.filename;
    if (!fName.startsWith("/")) { fName = "/" + fName; }

    if (upload.status == UPLOAD_FILE_START) {
      // Open the file
      if (LittleFS.exists(fName)) { LittleFS.remove(fName); }  // if
      _fsUploadFile = LittleFS.open(fName, "w");

    } else if (upload.status == UPLOAD_FILE_WRITE) {
      // Write received bytes
      if (_fsUploadFile) { _fsUploadFile.write(upload.buf, upload.currentSize); }

    } else if (upload.status == UPLOAD_FILE_END) {
      // Close the file
      if (_fsUploadFile) { _fsUploadFile.close(); }
    }  // if
  }    // upload()

protected:
  File _fsUploadFile;
};

String GetDateTimeApi() {

  //based on https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/

  HTTPClient https;

  String dateTime = "";
  
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();

  https.begin(*client, "https://www.timeapi.io/api/Time/current/zone?timeZone=Europe/Lisbon");
  
  int httpResponseCode = https.GET();
  
  String payload = "{}";
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = https.getString();

    JSONVar myObject = JSON.parse(payload);
    JSONVar keys = myObject.keys();

    for (int i = 0; i < keys.length(); i++) {
      JSONVar value = myObject[keys[i]];
      
      String key = keys[i];
      if(key == "dateTime"){
        dateTime = String(value);
        break;
      }
    }
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  https.end();
  return dateTime;
}

String DateTimeApi() {

  //based on https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/

  HTTPClient https;

  String dayOfWeek = "";
  
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();

  https.begin(*client, "https://www.timeapi.io/api/Time/current/zone?timeZone=Europe/Lisbon");
  
  int httpResponseCode = https.GET();
  
  String payload = "{}";
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = https.getString();

    JSONVar myObject = JSON.parse(payload);
    JSONVar keys = myObject.keys();

    for (int i = 0; i < keys.length(); i++) {
      JSONVar value = myObject[keys[i]];
      
      String key = keys[i];
      if(key == "dayOfWeek"){
        dayOfWeek = String(value);
        break;
      }
    }
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  https.end();
  return dayOfWeek;
}

void RegistRoutes() {
    // serve a built-in htm page
  server.on("/$upload.htm", []() {
    server.send(200, "text/html", FPSTR(uploadContent));
  });

  // register a redirect handler when only domain name is given.
  server.on("/", HTTP_GET, handleRedirect);

  // register some REST services
  server.on("/$list", HTTP_GET, handleListFiles);

  server.on("/$sysinfo", HTTP_GET, handleSysInfo);

  server.on("/$ledstatus", HTTP_GET, ledStatusInfo);

  server.on("/$ledState", HTTP_POST, ledStateCommand);

  server.on("/$readSensor", HTTP_GET, readMostureSensor);

  server.on("/$pumpCommand", HTTP_POST, pumpStateCommand);

  server.on("/$selectDays", selectDaysCommand);

  server.on("/$wateringTime", HTTP_POST, wateringTimeCommand);

  server.on("/$getWateringTime", wateringTimeQuery);

  server.on("/$getWeekDays", HTTP_GET, weekDaysQuery);

  server.on("/$wateringTimeUpdate", HTTP_GET, wateringTimeUpdate);

  server.on("/$updateSensorState", HTTP_GET, sensorStateUpdate);

  server.on("/$getSensorState", HTTP_GET, getSensorState);

  server.on("/$getLastWatering", HTTP_GET, getLastWatering);

}

// Setup everything to make the webserver work.
void setup(void) {
  
  Serial.begin(115200);
  
  delay(3000);  // wait for serial monitor to start completely.

  // Start counting
  
  Serial.setDebugOutput(false);
  
  // Use Serial port for some trace information from the example

  pinMode(D8, OUTPUT);

  pinMode(A0, INPUT);

  pinMode(D1, OUTPUT);

  TRACE("Starting WebServer example...\n");

  // start WiFI
  WiFi.mode(WIFI_STA);
  if (strlen(ssid) == 0) {
    WiFi.begin();
  } else {
    WiFi.begin(ssid, passPhrase);
  }

  // allow to address the device by the given name e.g. http://webserver
  WiFi.setHostname(HOSTNAME);

  TRACE("Connect to WiFi...\n");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    TRACE(".");
  }
  TRACE("connected.\n");

  TRACE("Mounting the filesystem...\n");
  if (!LittleFS.begin()) {
    TRACE("could not mount the filesystem...\n");
    delay(2000);
    ESP.restart();
  }

  // Ask for the current time using NTP request builtin into ESP firmware.
  TRACE("Setup ntp...\n");

  configTime(TIMEZONE, "pool.ntp.org");

  TRACE("Register service handlers...\n");

  DateTimeApi();

  RegistRoutes();
  // UPLOAD and DELETE of files in the file system using a request handler.
  server.addHandler(new FileServerHandler());

  // enable CORS header in webserver results
  server.enableCORS(true);

  // enable ETAG header in webserver results from serveStatic handler
  server.enableETag(true);

  // serve all static files
  server.serveStatic("/", LittleFS, "/");

  // handle cases when file is not found
  server.onNotFound([]() {
    // standard not found in browser.
    server.send(404, "text/html", FPSTR(notFoundContent));
  });

  server.begin();

  TRACE("hostname=%s\n", WiFi.getHostname());

  writeFile("/setup.txt", "");
  writeFile("/wateringtime.txt", "0");
  writeFile("/sensor.txt", "1");
  writeFile("/lastwatering.txt", "");

  setWateringTimeFromFile();

  setSensorFromFile();

  weekDayCheck.set(checkTime);

  displayDataSensorTimer.set(10000);

}  // setup
// run the server...
void loop(void) {
  
  server.handleClient();

  digitalWrite(D8 , pumpOff ? 0 : 1);

  if(displayDataSensorTimer.repeat()){
    
    moistVal = analogRead(A0);
    
    Serial.println(moistVal);
    
    int percent = 2.718282 * 2.718282 * (.008985 * moistVal + 0.207762); //calculate percent for probes about 1 - 1.5 inches apart
    
    Serial.print(percent);
    
    Serial.println("% Moisture ");

    
    if(moistVal < 220 && sensorOff == false)
    {       
      digitalWrite(D8, HIGH);
      delay(wateringTime * 1000);
      digitalWrite(D8, LOW);
    } 
    else
    {
      digitalWrite(D8, LOW);
    }
  }


  if(weekDayCheck.repeat()) {
    String ret = DateTimeApi();
    Serial.println(ret);
    for(int i = 0; i < 7; i++) {
      if(checkWeekDay(ret)){
        digitalWrite(D8, HIGH);
        delay(wateringTime * 1000);
        digitalWrite(D8, LOW);
        break;
      }
    } 
  }
}  // loop()

// end.