#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* This sample code demonstrates just about every built-in operation of TinyGPS++ (TinyGPSPlus).
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).

   The Crystal Display should be connected to SDA = A3, SCL A4
*/

int page_counter=1;         // This is to move between pages
int up=8;                   // Up button
int down=10;                // Down button
boolean current_up =  LOW;  // States
boolean last_up = LOW;      //
boolean last_down = LOW;
boolean current_down = LOW;

static const int RXPin = 4, TXPin = 3;      // This is the wiring for the GPS
static const uint32_t GPSBaud = 9600;       // This is the baud rate for the GPS

TinyGPSPlus gps;                            // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin);            // The serial connection to the GPS device
unsigned long last = 0UL;                   // For stats that happen every 5 seconds
LiquidCrystal_I2C lcd(0x27, 16,2);          // This is for the Crystal Display

void setup()
{
  Serial.begin(115200);                     // This is to turn on the terminal Display
  ss.begin(GPSBaud);                        // This is to turn on the GPS
  lcd.init();                               // This is for the Crystal Display
  lcd.backlight();                          // This is to turn on the Backlight
  lcd.print("Hello GPS");                   // This is Display a Welcome
}

// ---Debouncing function for all buttons
boolean debounce(boolean last, int pin){
  boolean current=digitalRead(pin);
  if (last != current){
    delay(5);
    current = digitalRead(pin);
  }
  return current;
  }

void loop()
{
   delay(2);
  // Dispatch incoming characters
  while (ss.available() > 0)
    gps.encode(ss.read());

  if (gps.location.isUpdated())
  {
    Serial.println("+++++++++++++++++++++++++++");

// begin the button pushing
current_up = debounce(last_up, up);         // Debounce for UP button
current_down = debounce(last_down, down);   // Debounce for  Down button

    if (last_up== LOW && current_up == HIGH) {  // When up button pressed
      lcd.clear();                          // When page is changed, lcd clear to print new page
      if(page_counter < 5){                 // Page counter never higher than 5 pages
        page_counter = page_counter + 1;    // Page up
      }  
      else {
        page_counter = 5;
      }
    }
     last_up = current_up;

    if (last_down== LOW && current_down ==HIGH){    // When down button pressed
      lcd.clear();                          // When page is changed, lcd is cleared
      if(page_counter > 1){                  // Page counter never lower than 1
        page_counter = page_counter -1;     // Page down
      }
      else {
        page_counter = 1;
      }
    }
    last_down = current_down;

  switch (page_counter) {
    case 1: {                         // Show date and time
      lcd.setCursor(0,0);
      lcd.print(gps.date.year());
      lcd.print("/");
      lcd.print(gps.date.month());
      lcd.print("/");
      lcd.print(gps.date.day());
      lcd.setCursor(0,1);
      lcd.print(gps.time.hour()-6);
      lcd.print(":");
      lcd.print(gps.time.minute());
      lcd.print(":");
      lcd.print(gps.time.second());
      
      }
      break;
    case 2: {                         // Show Location and Altitude
      lcd.setCursor(0,0);
      lcd.print(gps.location.lat(), 4);
      lcd.setCursor(0,1);
      lcd.print(gps.location.lng(), 4);
      }
      break;
    case 3: {                         // Show Speed and direction and Satelites
      lcd.setCursor(0,0);
      lcd.print("Speed: ");
      lcd.print(gps.speed.mph(),1);
      lcd.setCursor(0,1);
      lcd.print("Dir:   ");
      lcd.print(gps.course.deg(),0);
      }
      break;
    case 4: {                         // Show page 4
      lcd.setCursor(0,0);
      lcd.setCursor(0,1);
      lcd.print("Page 4");
      }
      break;
    case 5: {                         // Show page 5
      lcd.setCursor(0,0);
      lcd.setCursor(0,1);
      lcd.print("Page 5");
      }
      break;
  }   //switch end

    

    
/*    Serial.print("DATE      ");       //Date
    Serial.print(F("     = "));
    Serial.print(gps.date.year());
    Serial.print(F("/"));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.println(gps.date.day());
//    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DT ");
    lcd.print(gps.date.year());
    lcd.print("/");
    lcd.print(gps.date.month());
    lcd.print("/");
    lcd.print(gps.date.day());
 
    
    Serial.print("TIME MDT");         //Time
    Serial.print(F("       = "));
    Serial.print(gps.time.hour()-6);
    Serial.print(F(":"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    Serial.println(gps.time.second());
    lcd.setCursor(0,1);
    lcd.print("TM ");
    lcd.print(gps.time.hour()-6);
    lcd.print("/:");
    lcd.print(gps.time.minute());
    lcd.print(":");
    lcd.print(gps.time.second(),10);

    Serial.print("LOCATION   ");      //Location
    Serial.print ("Lat = ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" Long = "));
    Serial.println(gps.location.lng(), 6);
    lcd.setCursor(0,0);
    lcd.print("Lat = ");
    lcd.print(gps.location.lat(), 4);
    lcd.setCursor(0,1);
    lcd.print("Lng = ");
    lcd.print(gps.location.lng(), 4);

    Serial.print(F("Altitude       = ")); //Altitude
    Serial.println(gps.altitude.feet(),0);   
    
    Serial.print("SPEED ");           //Speed
    Serial.print(F(" MPH     = "));
    Serial.println(gps.speed.mph(),1);
    lcd.setCursor(0,0);
    lcd.print("ALT = ");
    lcd.print(gps.altitude.feet(),0); 
    lcd.print("    ");
    lcd.print(gps.satellites.value());
    lcd.setCursor(0,1);
    lcd.print("S = ");
    lcd.print(gps.speed.mph(),1);
    lcd.print(" ");
    lcd.print(gps.course.deg(),0);


    Serial.print("COURSE ");          //Course Direction
    Serial.print(F(" Deg    = "));
    Serial.println(gps.course.deg(),0);

    Serial.print("SATELLITES     = ");  //Satellites
    Serial.println(gps.satellites.value());
  */  
    
  }

}
