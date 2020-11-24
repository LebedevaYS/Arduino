#include <LiquidCrystal.h>
#include "IRremote.h"

IRrecv irrecv(A0);
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
decode_results results;
long unsigned int num[10] = {0x9F8ECFD5, 0x4DA7A0BF, 0x3E3D6F9, 0xFB54EA5B, 0x26B9C4DD, 0x87E5C91F, 0xEF881E99, 0x264C7D03, 0x2D607981, 0x87EAA93D};
int (*operation)(int x, int y);
int num1, num2;
int add(int, int);
int subtract(int, int);
int multiply(int, int);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  irrecv.enableIRIn();
  lcd.setCursor(0, 0);
  
 
}

void loop() { 
  
    

    
  if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println( results.value, HEX ); // печатаем данные
    irrecv.resume(); // принимаем следующую команду
    
   
    for(int i = 0; i <= 9; i++) {
      if(results.value == num[i]) {
        if(operation == 0) {
          num1 = num1*10 + i;
          lcd.print(i);
        } else {
          num2 = num2*10 + i;
          lcd.print(i);
        }
     }
   }
    
    switch(results.value) {
      case 0xEF3B295B:
        operation = add;
        lcd.print("+");
        break;
      case 0xD7C4FB7F:
        operation = subtract;
        lcd.print("-");
        break;
      case 0xF1FFE9FB:
        operation = multiply;
        lcd.print("*");
        break;
      case 0x69C6FA7D:
        lcd.print("=");
        lcd.print(operation(num1, num2));
        break;
      case 0xDE4001F: 
        lcd.print("                "); 
        num1 = 0;
        num2 = 0;
        operation = 0;
        lcd.setCursor(0, 0);
        break;
    }

}
}


int add(int x, int y) {
    return x + y;
}
int subtract(int x, int y) {
    return x - y;
}
int multiply(int x, int y) {
    return x * y;
}
