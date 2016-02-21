#include <LiquidCrystal.h>

//DEFINE LIQUID CRYSTAL PINS

#define LQCR_RS 2
#define LQCR_EN 3
#define LQCR_D0 4
#define LQCR_D1 5
#define LQCR_D2 6
#define LQCR_D3 7
#define LQCR_D4 8
#define LQCR_D5 9
#define LQCR_D6 10
#define LQCR_D7 30

//INITIALIZE LIQUID CRYSTAL DISPLAY

LiquidCrystal lcd(LQCR_RS, LQCR_EN, LQCR_D0, LQCR_D1, LQCR_D2, LQCR_D3, LQCR_D4, LQCR_D5, LQCR_D6, LQCR_D7);

void setup(){
  //SETUP LCD columns and rows: 
  lcd.begin(16, 2);

  lcd.clear();
  delay(20);
  lcd.print("***** READY ****");
  delay(100);

  lcd.setCursor(0,1);
  lcd.print("Set...");
  delay(100);

  lcd.setCursor(9,1);
  lcd.print("GO !!!");
}
