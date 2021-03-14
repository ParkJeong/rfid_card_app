#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();
  
}

void loop()
{
  int my_ran;
  my_ran=random(10);
	lcd.setCursor(0,0);
  lcd.print("Waste System!");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print(my_ran);
  delay(1000);
  lcd.clear();
	// Do nothing here...
}
