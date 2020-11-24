#include <LiquidCrystal.h>
#include "IRremote.h"

IRrecv irrecv(A2);
#define DIODE_DROP 0.7
#define LED_PIN 13
#define MOTOR_PIN 6
#define LDR_PIN A1
#define POT_PIN A0
#define LIMIT_LIGHTS 550

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
unsigned long int ms, ms1, ms_light_on, ms_light_off, flash_count;
boolean led_mode, ledWasOn = false, mode_pin = false;
int maxFlash, irspeed = 0, motspeed;
decode_results results;

void setup() {
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  Serial.begin(9600);
  digitalWrite(13, HIGH);
  lcd.begin(16, 2);
  irrecv.enableIRIn();
  
}

void loop() {
  float frequency, mspeed, brightness;
  int rotation;
  
  if(mode_pin) {
    rotation = analogRead(POT_PIN) + 1;
    mspeed = rotation / 4;
    motspeed = mspeed; //abs(mspeed - 255);
    analogWrite(MOTOR_PIN, motspeed);
  }
    
  
  if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println( results.value, HEX ); // печатаем данные
    Serial.println("mode: ");
    Serial.println(mode_pin);
    switch ( results.value ) {
    case 0x86288A23:
        irspeed = min(255, irspeed + 10);
        analogWrite(MOTOR_PIN, irspeed);
        break;
    case 0x169CDC1F:
        irspeed = max(0, irspeed - 10);
        analogWrite(MOTOR_PIN, irspeed);
        break;
    case 0x71B93203:
        mode_pin = !mode_pin;
        Serial.println("mode_case: ");
        Serial.println(mode_pin);
        analogWrite(MOTOR_PIN, irspeed);
        break;
    }    
    irrecv.resume(); // принимаем следующую команду
  }

  brightness = 1024 - analogRead(LDR_PIN);
  boolean ledIsOn = brightness > LIMIT_LIGHTS ;
   if (!ledWasOn && ledIsOn) {
     // led became on
     flash_count++;
     
   }
   ledWasOn = ledIsOn; 

  if(millis() - ms >= 1000) {
     displayInfo();
     ms = millis();
  }
}








void displayInfo() {
  int flash_freq = flash_count;
    flash_count=0;
    if(flash_freq > maxFlash) {
      maxFlash = flash_freq;
     }
    lcd.setCursor(0, 0);
    //lcd.print("R:");
    //lcd.print(rotation);
    lcd.print("Sp:");
    if(mode_pin) {
      lcd.print(motspeed);
    } else {
      lcd.print(irspeed);
    }
    lcd.print(" Det:");
    lcd.print(flash_freq);
    lcd.print("  ");
    
    lcd.setCursor(0, 1);
    lcd.print("MAX:");
    lcd.print(maxFlash);
  //  lcd.print(" [");
  //  lcd.print((int)brightnessStat.minimum());
  //  lcd.print(",");
  //  lcd.print((int)brightnessStat.maximum());
  //  lcd.print("]  ");
    /* Serial.print("rotation (0-1024): ");
    Serial.println(rotation);
    Serial.print("time (ms): ");
    Serial.println(loopTime);

    Serial.println("-------------------");*/
}
