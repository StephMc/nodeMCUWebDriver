/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor

 This example code is in the public domain.
 */

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 13;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Lab202";
const char* password = "Robots01";


int getY(char *request);
int getX(char *request);
int parseRequest(char *request);
WiFiServer server(80);

// the setup routine runs once when you press reset:
void setup() {
  pinMode(13, OUTPUT);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  delay(100);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  printWifiStatus();
}

// the loop routine runs over and over again forever:
void loop() {
  int x=0;
  int y=0;
  char request[500] = {0};
  int counter = 0;
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
      
        char c = client.read();
        request[counter] = c;
        counter++;
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          if(parseRequest(request)) {
              x = getX(request);
              y = getY(request);
              Serial.println(y);
              Serial.println(x);
              break;
          } else {
            // send a standard http response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println();
            client.println("<html>\n");
            client.println("<head>\n");
            client.println("<meta charset=\"utf-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0\">");
            client.println("<style>");
            client.println("#info {\n"
                         "position  : absolute;\n"
                         "top   : 0px;\n"
                         "width   : 100%;\n"
                         "padding   : 5px;\n"
                         "text-align  : center;\n"
                         "}");
            client.println("#container {\n"
                        "width   : 100%;\n"
                        "height    : 100%;\n"
                        "overflow  : hidden;\n"
                        "padding   : 0;\n"
                        "margin    : 0;\n"
                        "-webkit-user-select : none;\n"
                        "-moz-user-select  : none;\n"
                        "}");
            client.println("</style>");
                    
            client.println("</head>");
            client.println("<body>");
          
            client.println("<div id=\"container\"></div>");
            client.println("<div id=\"info\">");
            client.println("<span id=\"result\"></span>");
            client.println("</div>");
            client.println("<div id=\"test\"></div>");
            client.println("<script src=\"http://192.168.1.136/javascript/post.js\"></script>");
            client.println("<script src=\"http://192.168.1.136/javascript/virtualjoystick.js\"></script>");
            client.println("<script>\n"
                "console.log(\"touchscreen is\", VirtualJoystick.touchScreenAvailable() ? \"available\" : \"not available\");\n"
                "var joystick  = new VirtualJoystick({\n"
                "container : document.getElementById('container'),\n"
                "mouseSupport  : true,\n"
                 "});\n"
                "joystick.addEventListener('touchStart', function(){\n"
                "console.log('down')\n"
                "})\n"
                "joystick.addEventListener('touchEnd', function(){\n"
                "console.log('up')\n"
                "})\n"

                "setInterval(function(){\n"
                "console.log(loadDoc());\n"
                "var outputEl  = document.getElementById('result');\n"
                "outputEl.innerHTML  = '<b>Result:</b> '\n"
                "+ ' dx:'+joystick.deltaX()\n"
                "+ ' dy:'+joystick.deltaY()\n"
                "+ (joystick.right() ? ' right'  : '')\n"
                "+  (joystick.up()  ? ' up'   : '')\n"
                "+ (joystick.left()  ? ' left' : '')\n"
                "+ (joystick.down()  ? ' down'   : '') \n"
                "}, 1/30 * 100000);\n"
                "</script>\n"
                "</body>\n"
                "</html>");
            break;
            }
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
   
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

int parseRequest(char *request) {
  if (request[0] == 'P' && request[1] == 'O' && request[2] == 'S' && request[3] == 'T') {
    return true;
  }else {
    return false;
  }
}

int getX(char *request) {
  int counter = 0;
  long returnValue;
  while (request[counter] != 'x' && counter < 100) {
    counter++;
  }
  Serial.println(counter);
  returnValue = myAtoL(&request[counter+2]);
  return returnValue;
}


int getY(char *request) {
  int counter = 0;
  long returnValue;
  while (request[counter] != 'y' && counter < 100) {
    counter++;
  }
  Serial.println(counter);
  if (request[counter] == 'y') {
    returnValue = myAtoL(&request[counter+2]);
  } else {
    returnValue = -1;
  }
  return returnValue;
}

long myAtoL(char *message) {
  long num = 0;
  int negative = false;
  int currentPosition = 0;
  if (message[currentPosition] == '-'){
    negative = true;
    currentPosition++;
  }
  while (message[currentPosition] >= '0' && message[currentPosition] <= '9') {
    int digitValue = message[currentPosition] - '0';
    num *= 10;
    num += digitValue;
    currentPosition++;
  }
  if (negative) {
    num *= -1;
  }
  return num;
}



