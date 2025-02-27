/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include "..\sercrets.h"

// Replace with your network credentials
const char* ssid     = SSID;
const char* password = PASS;

// Set web server port number to 8081
//http://47.24.149.229:8081/
WiFiServer server(8081);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";
String hotState = "off";
String coldState = "off";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  // Print local IP address and start web server
  /*
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  */
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0)
            {
              output5State = "on";
              Serial.flush();
              Serial.write("on");
            }
            else if (header.indexOf("GET /5/off") >= 0)
            {
              output5State = "off";
              Serial.flush();
              Serial.write("off");
            } 
            else if (header.indexOf("GET /sink/hot/on") >= 0)
            {
              hotState = "on";
              Serial.flush();
              Serial.write("HOT1");
            }
            else if (header.indexOf("GET /sink/hot/off") >= 0)
            {
              hotState = "off";
              Serial.flush();
              Serial.write("hot0");
            }
            else if (header.indexOf("GET /sink/cold/on") >= 0)
            {
              coldState = "on";
              Serial.flush();
              Serial.write("COLD3");
            }
            else if (header.indexOf("GET /sink/cold/off") >= 0)
            {
              coldState = "off";
              Serial.flush();
              Serial.write("cold2");
            }
            else if (header.indexOf("GET /toilet/flush") >= 0)
            {
              Serial.flush();
              Serial.write("flush");
              
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>Sink - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            //Hot Water Button 
            client.println("<p>Hot - State " + hotState + "</p>");
            // If the output4State is off, it displays the ON button       
            if (hotState=="off") {
              client.println("<p><a href=\"/sink/hot/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/sink/hot/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //Cold Water Button
            client.println("<p>Cold - State " + coldState + "</p>");
            // If the output4State is off, it displays the ON button       
            if (coldState=="off") {
              client.println("<p><a href=\"/sink/cold/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/sink/cold/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //Toilet Flush
            client.println("<p>Toilet</p>");      
            client.println("<p><a href=\"/toilet/flush\"><button class=\"button\">Flush</button></a></p>");
            

            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
}
