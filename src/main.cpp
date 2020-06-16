#include <Arduino.h>
#include <WiFi.h>
#include <M5StickC.h>

// Replace with your network credentials
const char* wifi_name = "Science Camp";
const char* wifi_pass = "supersecret";

// Set web server port number to 80
WiFiServer server(80);

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

void setup()
{
    delay(5000);
    Serial.begin(9600);

    // Let's connect to wifi network
    Serial.print("Connecting to ");
    Serial.print(wifi_name);
    WiFi.begin(wifi_name, wifi_pass);       //Connecting to wifi network

    while (WiFi.status() != WL_CONNECTED) { //Waiting for the responce of wifi network
        delay(5000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connection Successful");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());         //Getting the IP address at which our webserver will be created
    Serial.println("Put the above IP address into a browser search bar");
    M5.begin();
    M5.IMU.Init();
    server.begin();  //Starting the server
}
float temp = 0;
/*****************************************
M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
M5.IMU.getAccelData(&accX,&accY,&accZ);
M5.IMU.getAhrsData(&pitch,&roll,&yaw);
M5.IMU.getTempData(&temp);
*****************************************/
void loop()
{
  M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  M5.IMU.getAhrsData(&pitch,&roll,&yaw);
  M5.IMU.getTempData(&temp);
 WiFiClient client = server.available();   //Checking for incoming clients

  if (client)
  {
    Serial.println("new client");
    String currentLine = "";                   //Storing the incoming data in the string
    while (client.connected())
    {
      if (client.available())                 //if there is some client data available
      {
        char c = client.read();                //read a byte
        Serial.print(c);
          if (c == '\n')                      //check for newline character,
          {
           if (currentLine.length() == 0)      //if line is blank it means its the end of the client HTTP request
           {
            client.print("gyroX, "+String(gyroX)+", "+"gyroY, "+String(gyroY)+", "+"gyroZ, "+String(gyroZ)+", ");
            client.print("accX, "+String(accX)+", "+"accY, "+String(accY)+", "+"accZ, "+String(accZ)+", ");
            client.print("Pitch, "+String(pitch)+", "+"Roll, "+String(roll)+", "+"Yaw, "+String(yaw));
            client.print('\n');
            break;                            //Going out of the while loop
           }
           else
           {
            currentLine = "";                //if you got a newline, then clear currentLine
           }
         }
         else if (c != '\r')
         {
          currentLine += c;                 //if you got anything else but a carriage return character,
         }
      }
    }
 }
}