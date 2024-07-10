#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Define the value read by the ADC (Analog to Digital Converter)
int value = 0; // The value read

float WaterLevel;
/*Put your SSID & Password*/
const char* ssid = "SSY";  // Enter your WiFi name
const char* password = "123456789";  // Enter your WiFi password

ESP8266WebServer server(80); // Create a server on port 80

void setup() {
  Serial.begin(115200); // Set the baud rate for the serial monitor to 115200
  delay(100);

  // Print WiFi connection information to the serial monitor
  Serial.println("Connecting to ");
  Serial.println(ssid);

  // Connect to your local Wi-Fi network
  WiFi.begin(ssid, password);

  // Check if the device is connected to the Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  WaterLevel = (analogRead(A0) * 100) / 1024; // Calculate the water level
  server.send(200, "text/html", SendHTML(WaterLevel)); 
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float WaterLevelstat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head> <meta name=\"viewport\" content=\"width=device-width,  initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr +="<meta charset=\"UTF-8\">\n";
  ptr +="<title>Smart Water Level Indicator</title>\n";
  ptr +="<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}\n";
  ptr +="body{margin-top: 50px;}\n";
  ptr +="h1 {margin: 50px auto 30px;}\n";
  ptr +=" .wd {margin: 50px auto 30px;width: auto;color: #f39c12}\n";
  ptr +=" .wd1 {margin: 50px auto 30px;width: auto;color: #3498db}\n";
  ptr +=".side-by-side{display: inline-block;vertical-align: middle;position: relative;}\n";
  ptr +=".WaterLevel-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  ptr +=".WaterLevel{font-weight: 300;font-size: 60px;color: #3498db;}\n";
  ptr +=".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n";
  ptr +=".data{padding: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  
  ptr +="<div id=\"webpage\">\n";
   
  ptr +="<h1>Water Level Monitoring System</h1>\n";
  ptr +="<div class=\"data\">\n";
  ptr +="</div>\n";
  ptr +="<span class=\"superscript\">%</span></div>\n";
  ptr +="</div>\n";
  ptr +="<div class=\"data\">\n";
  ptr +="</div>\n";
  ptr +="<div class=\"side-by-side WaterLevel-text\">Current Water Level:</div>\n";
  ptr +="<div class=\"side-by-side WaterLevel\">";
  ptr += (int)WaterLevelstat;
  ptr +="<span class=\"superscript\">%</span></div>\n";
  ptr +="</div>\n";

  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
