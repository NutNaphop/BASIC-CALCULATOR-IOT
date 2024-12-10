#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

#define sw1 D5 // First Digit
#define sw2 D6 // Sign 
#define sw3 D7 // Second Digit 
#define sw4 D8 // Clear 

// Set LCD 
LiquidCrystal_I2C lcd (0x27 , 16 , 2) ; 

// Function 
String sign_gen(int indexOfSign , int num1 , int num2){
  if (indexOfSign == 0){
    return String(num1+num2) ; 
  }
  else if (indexOfSign == 1){
    return String(num1-num2) ;
  }
  else if (indexOfSign == 2){
    return String(num1*num2) ;
  }
  else if (indexOfSign == 3){
    // DIV0 Error
    if (num2 == 0 && num1 != 0){ 
      lcd.clear() ; 
      lcd.setCursor(0, 0) ; 
      lcd.print("DIV0") ;
      lcd.setCursor(0,1) ; 
      lcd.print("Reset") ; 
      while (true){
        if (digitalRead(sw4) == 1){
          lcd.clear() ;
          break; 
        }
      }
    }
    else if (num1 == 0 && num2 == 0) {
      return "0" ;
    }
    else {
      return String((float)num1/num2) ;
    }
  }
  return "0" ; 
}

// Variable 
String sign[] = {"+","-","x","/"} ; 
int num1 = 0 ;
int num2 = 0 ; 
int indexOfSign = 0 ; 
int his1 = 0 ;
int his2 = 0 ; 
int his_sign = 0 ; 
int clear_state = 0 ; 
String result = String(num1+num2); 

// Set up 
void setup() {
  Serial.begin(19200) ;
  pinMode(D3 , INPUT) ; 
  lcd.init() ; 
  lcd.backlight();
}

// Start 
void loop() {

// Display Section 
lcd.setCursor(0,0); 
lcd.print(num1) ; 
lcd.setCursor(2, 0);
lcd.print(sign[indexOfSign]) ; 
lcd.setCursor(4,0) ; 
lcd.print(num2) ; 
lcd.setCursor(6, 0) ; 
lcd.print("=") ; 
result = sign_gen(indexOfSign,num1,num2) ; 
lcd.setCursor(8,0);
lcd.print(result) ;

// Clear if 0 and 0 for one time 
if (clear_state == 0 && (num1 == 0 || num2 == 0)){
  lcd.clear() ; 
  clear_state = 1 ; // If press any button clear_state will = 0 
}

// First Digit 
if (digitalRead(sw1) == 0 && his1 == 1){
  num1 += 1 ; 
  his1 = 0 ; 
  clear_state = 0 ; 
  if( num1 > 9){
    num1 = 0 ; 
    clear_state = 0 ; // <- Using for 0 0 condition
  }
  lcd.clear() ; 
}

// Second Digit 
if (digitalRead(sw3) == 0 && his2 == 1){
  num2 += 1 ; 
  his2 = 0 ; 
  if( num2 > 9){
    num2 = 0 ; 
    clear_state = 0 ; // <- Using for 0 0 condition
  }
  lcd.clear() ; 
}

// Sign Digit  
if (digitalRead(sw2) == 0 && his_sign == 1){
  if (indexOfSign >=3){
    indexOfSign = 0 ; 
  }
  else {
    indexOfSign += 1;
  }
  lcd.clear() ; 
}

// Clear switch
if (digitalRead(sw4) == 1) num1 = 0 , num2 = 0 , indexOfSign = 0 , lcd.clear() ; 

// History 
his1 = digitalRead(sw1) ; // his bt1
his2 = digitalRead(sw3) ; // his bt 3 
his_sign = digitalRead(sw2) ; // his sg 

}

