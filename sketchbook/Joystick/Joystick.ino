#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Define LCD Screen params
LiquidCrystal_I2C lcd(0x27, 16, 2);


#include <aJSON.h>
#include "SPI.h"
#include "WiFi.h"

#include "M2XStreamClient.h"

char ssid[] = "BitwiseHacks"; //  your network SSID (name)
char pass[] = "bwhackathon";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char deviceId[] = "5b51e4b772e8ab8d0e04e95b9cbe9fec"; // Device you want to push to
char streamName[] = "completed-orders"; // Stream you want to push to
char m2xKey[] = "6dec8ed279b20d29e9835233362dcd89"; // Your M2X access key

WiFiClient client;
M2XStreamClient m2xClient(&client, m2xKey);



//Set up joystick constant pins
const int joystickSel = 4;     // the number of the joystick select pin
const int joystickX = 2;       // the number of the joystick X-axis analog
const int joystickY =  6;     // the number of the joystick Y-axis analog

const int ledBlue  =  37;      // the number of the LED pin
const int ledGreen =  38;      // the number of the LED pin
const int ledRed =  39;        // the number of the LED pin
// variables will change:
int joystickSelState = 0;      // variable for reading the joystick sel status
int joystickXState, joystickYState ; 

String text;
int l = 16;
String msgs[16];
int i;
unsigned long previousMillis;


void setup() {
  // initialize the LED pins as  output:
  pinMode(ledRed, OUTPUT);     
   
  // initialize the pushbutton pin as an input:
  pinMode(joystickSel, INPUT_PULLUP);     

  //init lcd
  lcd.begin();
  lcd.backlight();
  lcd.print("Hello");
  Serial.begin(9600);
  
   // attempt to connect to Wifi network:
    Serial.print("Attempting to connect to Network named: ");
    // print the network name (SSID);
    Serial.println(ssid); 
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    WiFi.begin(ssid, pass);
    while ( WiFi.status() != WL_CONNECTED) {
      // print dots while we wait to connect
      Serial.print(".");
      delay(300);
    }
  
    Serial.println("\nYou're connected to the network");
    Serial.println("Waiting for an ip address");
  
    while (WiFi.localIP() == INADDR_NONE) {
      // print dots while we wait for an ip addresss
      Serial.print(".");
      delay(300);
    }

    Serial.println("\nIP Address obtained");
  
    // you're connected now, so print out the status  
    printWifiStatus();
    
  
  msgs[0] =  "Item ID:XXXXxXX OID:XXX @XX:XXam";
  msgs[1] =  "Item ID: B23x1  OID: 01 @10:45am";
  msgs[2] =  "Item ID: A13x1  OID: 02 @10:45am";
  msgs[3] =  "Item ID: A24x1  OID: 03 @11:05am";
  msgs[4] =  "Item ID: A13x3  OID: 04 @11:15am";
  msgs[5] =  "Item ID: A23x1  OID: 05 @11:25am";
  msgs[6] =  "Item ID: A03x1  OID: 06 @11:25am";
  msgs[7] =  "Item ID: C43x1  OID: 07 @11:30am";
  msgs[8] =  "Item ID: N23x1  OID: 08 @11:45am";
  msgs[9] =  "Item ID: B26x2  OID: 09 @12:00pm";
  msgs[10] = "Item ID: A15x3  OID: 10 @12:45pm";
  msgs[11] = "Item ID: A03x1  OID: 11 @01:25pm";
  msgs[12] = "Item ID: V28x4  OID: 12 @01:45pm";
  msgs[13] = "Item ID: Z42x1  OID: 13 @02:00pm";
  msgs[14] = "Item ID: A23x1  OID: 14 @02:30pm";
  msgs[15] = "Item ID: A73x1  OID: 15 @02:45pm";
  i = 0;
  previousMillis = millis();
  
}
boolean triggered = false;
void loop(){
  // read the analog value of joystick x axis
  joystickXState = analogRead(joystickX);
  // scale the analog input range [0,4096] into the analog write range [0,255]
  joystickXState = map(joystickXState, 0, 4096, 0, 2);
  // output to the led
  analogWrite(ledGreen, joystickXState);
  
  // read the analog value of joystick y axis
  joystickYState = analogRead(joystickY);
  // scale the analog input range [0,4096] into the analog write range [0,255]
  joystickYState = map(joystickYState, 0, 4096, 0, 2);
  // output to the led
  analogWrite(ledBlue, joystickYState);
  
  // read the state of the joystick select button value:
  joystickSelState = digitalRead(joystickSel);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (joystickSelState == LOW) {     
    // turn LED on:    
    digitalWrite(ledRed, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledRed, LOW); 
  }
  if(millis() %100 == 0 )
  {
    //lcd.print("RX:" + String(joystickXState) + " RY:" + String(joystickYState) + " SW:" + String(joystickSelState));
    if(joystickYState == 0 && !triggered){
      triggered = true;
      display(msgs[i++]);
      if(i >= l)
        i = 0;
    }
    else if(joystickYState == 1){
      triggered = false;
    }
    
    if(joystickSelState == 0 && !triggered){
      triggered = true;
//      String temp = msgs[l-1];
//      msgs[l-1] = msgs[--i];
//      msgs[i] = temp;
//      l--;
      String temp = msgs[i-1];
      lcd.clear();
      lcd.print("Order complete");
      int response = m2xClient.updateStreamValue(deviceId, streamName, temp);
      Serial.print("M2X client response code: ");
      Serial.println(response);        
    
      if (response == -1)
        while (1)
          ;
      for(int k = i; k < l; k++){
        msgs[k-1] = msgs[k];
      }
      --l;
      i = 0;
      display(msgs[i]);
    }
  } 
}


void display(String text){
 
  lcd.setCursor(0,0);
  if(text.length() > 16)
  {
    lcd.print(text.substring(0,16));
    lcd.setCursor(0,1);
    lcd.print(text.substring(16));
  }
  else
  {
    lcd.print(text);
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
}
