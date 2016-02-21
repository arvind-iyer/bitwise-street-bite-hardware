#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
String text;
int l = 2;
String msgs[2];
int i;
unsigned long previousMillis;
void setup()
{
	// initialize the LCD
	lcd.begin();
	// Turn on the blacklight and print a message.
	lcd.backlight();
    lcd.noAutoscroll();
     msgs[0] = "Item ID:XXXXxXX OID:XXX @XX:XXam";
     msgs[1] = "Item ID: A23x1  OID: 32 @12:45pm";
     i = 0;
     previousMillis = millis();
}
void loop()
{
  
 unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 900) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
     display(msgs[i++]); 
    if(i == l)
      i = 0;
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
