#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define LED_PIN 4
Adafruit_NeoPixel strip = Adafruit_NeoPixel(7, LED_PIN, NEO_GRB + NEO_KHZ800);

#include <Encoder.h>

Encoder myEnc(5, 6);

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const byte firePin = 3;

const char* weapons[]={
  "Entropic shock",
  "Higgs Inverter",
  "Neutrino Burst",
  "Gamma Ray",
  "Electron Beam",
  "Plasma"
};



void setup() {
  Serial.begin(9600);
  Serial.println("Hexagun starting...");

  lcd.begin(16,2);               // initialize the lcd 
  lcd.clear();
  lcd.home();                    // go home
  lcd.print("Hexagun");
  lcd.setCursor(0,1);
  lcd.print("charging...");
  delay(2000);
  lcd.clear();
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  
  pinMode(firePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(firePin), fire, FALLING);
  
  
}

long oldPosition  = -999;
long oldWeaponIndex = -1;
int weaponIndex = 0;

void loop() {
  long newPosition = mod(myEnc.read(), 24);
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.print("pos: ");
    Serial.println(newPosition);

    weaponIndex = (newPosition >> 2);
    selectWeapon();
  }
}

void selectWeapon() {
  if (oldWeaponIndex != weaponIndex) {
    oldWeaponIndex = weaponIndex;
    Serial.print("weapon: ");
    Serial.println(weaponIndex);
    String curWeapon = String("[") + String(weapons[weaponIndex]);

    // pad with spaces
    while(curWeapon.length() < 15) {
      curWeapon += " ";
    }

    curWeapon += "]";
    
    Serial.println(curWeapon);

    lcd.clear();
    lcd.home();
    
    lcd.print(curWeapon.c_str());
  }
}



void fire() {
  Serial.println("fire!");
  uint32_t color;
  
  switch(weaponIndex) {
    // "Entropic shock", red
    case 0:
      color = strip.Color(255, 0, 0);
      break;
      
    // "Higgs Inverter", white
    case 1:
      color = strip.Color(255, 255, 255);
      break;
    
    // "Neutrono Burst", yello
    case 2:
      color = strip.Color(255, 255, 0);
      break;
    
    // "Gamma Ray", green
    case 3:
      color = strip.Color(0, 255, 0);
      break;
    
    // "Electron Beam", blue
    case 4:
      color = strip.Color(0, 0, 255);
      break;
    
    //"Plasma" purple
    case 5:
      color = strip.Color(255, 0, 255);
      break;
      
    default:
      color = strip.Color(255, 255, 255);
      break;
  }

  for(int j=0; j< 30 ; j++) {
    if( (j % 2) == 0)
      colorWipe(color, 50); // Red
    else
      colorWipe(strip.Color(0, 0, 0), 50); // Red

    delay(50); 
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
//      delay(wait);
  }
}

int mod (int a, int b)
{
   if(b < 0) //you can check for b == 0 separately and do what you want
     return mod(a, -b);   
   int ret = a % b;
   if(ret < 0)
     ret+=b;
   return ret;
}
