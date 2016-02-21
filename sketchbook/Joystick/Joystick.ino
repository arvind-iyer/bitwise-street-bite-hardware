#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Define LCD Screen params
LiquidCrystal_I2C lcd(0x27, 16, 2);



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
  delay(1234);
  Serial.begin(9600);
  
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
      for(int k = i; k < l; k++){
        msgs[k-1] = msgs[k];
      }
      --l;
      lcd.clear();
      lcd.print("Order complete");
      delay(500);
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
