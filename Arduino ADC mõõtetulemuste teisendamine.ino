#include <LiquidCrystal.h>

#define potentiometer A6
#define buttonRow 8
#define buttonType 9

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);                //(RS, E, DB4, DB5, DB6, DB7)

int potValue = 0;                                   //input value from potentiometer
int buttonRowState = 0;                             //button that changes row
int buttonTypeState = 0;                            //button that changes unsigned/signed/percentage values


void setup() {
  lcd.begin(16, 2);                                 //define size of display
  Serial.begin(9600);
}


void binaryValue(int potValue) {
  
  for (int i = 11; i >= 0; i--)                     //12 bits: 0000 0000 0000
  {
    bool num = bitRead(potValue, i);                //read only one bit at a time 
    lcd.print(num);
    if (i == 8 || i == 4)                           
      lcd.print(' ');
  }
  
}

void unsignedValue(int potValue) {

  lcd.print(potValue);                              //no need to change value
  delay(150);
  lcd.clear();
  
}

void signedValue(int potValue) {
                                                    //"-512" now represents "0" and "511" is "1023"                                 
  int signedValue = map(potValue, 0, 1023, -512, 511); 
  lcd.print(signedValue);
  delay(150);
  lcd.clear();
  
}

void percentageValue(int potValue) {
                                                           
  int percentage = map(potValue, 0, 1023, 0, 100);            
  lcd.print(percentage);
  lcd.print("%");
  delay(150);
  lcd.clear();
  
}

bool secondRow = false;                             //to check if second row is chosen                         
int switchType = 0; 

void loop() {

  //read input values
  potValue = analogRead(potentiometer);
  buttonRowState = digitalRead(buttonRow);
  buttonTypeState = digitalRead(buttonType);


  if (buttonRowState == HIGH) {
    if (!secondRow)                                
      secondRow = true;
    else                                            
      secondRow = false;
  }
  if (buttonTypeState == HIGH && secondRow) {       //switchRow is set to 1, so values in the second row can only be changed if this row is chosen
    if (switchType != 2)                            
      switchType++;                                
    else                                           
      switchType = 0;
    
  }


  if (!secondRow) {                                 //first row is chosen
    lcd.setCursor(0, 0);
    lcd.print("> ");
    binaryValue(potValue);
    lcd.setCursor(2, 1);

  } else {                                          //second row is chosen
    lcd.setCursor(2, 0);
    binaryValue(potValue);
    lcd.setCursor(0, 1);
    lcd.print("> ");
  }

  switch (switchType) {
    case 0:
      unsignedValue(potValue);
      lcd.setCursor(8, 1);
      lcd.print("unsigned");
      break;
    case 1:
      signedValue(potValue);
      lcd.setCursor(8, 1);
      lcd.print("signed");
      break;
    case 2:
      percentageValue(potValue);
      break;
  }
}
