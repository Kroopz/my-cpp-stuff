#include <Arduino.h>
#include <ESP8266WiFi.h>

WiFiServer server(80); //initialize server on port 80

#define PIN_OUT_A D5 //Left
#define PIN_OUT_B D6 //Right
#define PIN_OUT_C D7 //Control/Forward


void input_forward()
{
  digitalWrite(PIN_OUT_A,LOW);
  digitalWrite(PIN_OUT_B,LOW);
  digitalWrite(PIN_OUT_C,HIGH);
}

void input_left()
{
  digitalWrite(PIN_OUT_A,HIGH);
  digitalWrite(PIN_OUT_B,LOW);
  digitalWrite(PIN_OUT_C,HIGH);
}

void input_right()
{
  digitalWrite(PIN_OUT_A,LOW);
  digitalWrite(PIN_OUT_B,HIGH);
  digitalWrite(PIN_OUT_C,HIGH);
}

void input_back()
{
  digitalWrite(PIN_OUT_A,HIGH);
  digitalWrite(PIN_OUT_B,HIGH);
  digitalWrite(PIN_OUT_C,HIGH);
}

void input_stop()
{
  digitalWrite(PIN_OUT_A,LOW);
  digitalWrite(PIN_OUT_B,LOW);
  digitalWrite(PIN_OUT_C,LOW);
}

void handle_input(String &request)
{
  
  if (request.indexOf("/FORWARD") != -1)
  {
    input_forward();
  }
  if (request.indexOf("/LEFT") != -1)
  {
    input_left();
    delay(100);
    input_stop();
  }
  if (request.indexOf("/RIGHT") != -1)
  {
    input_right();
    delay(100);
    input_stop();
  }
  if (request.indexOf("/BACK") != -1)
  {
    input_back();
  }
  if (request.indexOf("/STOP") != -1)
  {
    input_stop();
  }
}

void setup() {
  // put your setup code here, to run once:
  WiFi.mode(WIFI_AP); //AccessPoint
  WiFi.softAP("TEST_IOT","mytestpassword"); //SSID, pass
  server.begin(); //start http server
  //debug ass shit
  Serial.begin(115200); //start communication between board and monitor
  IPAddress HTTPS_ServerIP = WiFi.softAPIP(); //get ip of the server
  Serial.println();
  Serial.print("Server IP is: "); //print IP
  Serial.println(HTTPS_ServerIP);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); //initial is OFF

  //initialize output pins
  pinMode(PIN_OUT_A,OUTPUT);
  pinMode(PIN_OUT_B,OUTPUT);
  pinMode(PIN_OUT_C,OUTPUT);

  digitalWrite(PIN_OUT_A,LOW);
  digitalWrite(PIN_OUT_B,LOW);
  digitalWrite(PIN_OUT_C,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Check if browser (client) connected to ESP
  //if browser connected then do following
  //if not then jump out and check again
  WiFiClient client = server.available();
  if (!client) return;
  Serial.println("Somebody has connected.");

  //debug fuck
  String request = client.readStringUntil('\r');
  Serial.println(request);
  
  //Handle request
  handle_input(request);

  //a fukin mile long line of an html page code cause i cant be bothered to space it out properly dont @ me i do what i want
  String s = "<!DOCTYPE html><html style=\"text-align:center\"><head><style>table{margin: auto;border: 0px solid black;text-align: center;}td { border: 0px solid black; text-align: center; padding: 50px;}.b1{padding: 20px;text-align: center; height: 150px; width: 150px;}</style></head><body><br><br><br><table> <tr>    <td colspan=\"3\"><input class=\"b1\" type=\"button\" value=\"Forward\" onclick=\"location.href='/FORWARD'\"></td>  </tr>  <tr>    <td><input class=\"b1\" type=\"button\" value=\"Left\" onclick=\"location.href='/LEFT'\"></td>    <td><input class=\"b1\" type=\"button\" value=\"Stop\" onclick=\"location.href='/STOP'\"></td>    <td><input class=\"b1\" type=\"button\" value=\"Right\" onclick=\"location.href='/RIGHT'\"></td>  </tr>  <tr>  	<td colspan=\"3\"><input class=\"b1\" type=\"button\" value=\"Back\" onclick=\"location.href='/BACK'\"></td>  </tr></table></body></html>";
  
  //Serve html doc to browser
  client.flush(); //clear previous info in stream
  client.print(s); //Send doc in response to client
  Serial.println("Client disconnected");
  
}
