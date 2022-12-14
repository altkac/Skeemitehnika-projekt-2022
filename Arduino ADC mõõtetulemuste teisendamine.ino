#include <LiquidCrystal.h>                          //Library to controll LCD screen

#define potentiometer A6                            //Arduino analog pin 6
#define buttonRow 8                                 //Arduino digital pin 8
#define buttonType 9                                //Arduino digital pin 9

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);                //(RS, E, DB4, DB5, DB6, DB7)

int potValue = 0;                                   //Input value from potentiometer
int buttonRowState = 0;                             //Button that changes row
int buttonTypeState = 0;                            //Button that changes unsigned/signed/percentage values


void setup() {                                        
  lcd.begin(16, 2);                                 //Define size of display (16 columns and 2 rows)
  Serial.begin(9600);
}


void binaryValue(int potValue) {                    //Function to convert value to binary
  
  for (int i = 11; i >= 0; i--)                     //12 bits: 0000 0000 0000
  {
    bool num = bitRead(potValue, i);                //Read only one bit at a time 
    lcd.print(num);                                 //Print one current bit
    if (i == 8 || i == 4)                           //Spaces after 4th and 8th bits,           
      lcd.print(' ');                               //so it looks "0011 1111 1111", not "001111111111"              
  }
  
}

void unsignedValue(int potValue) {                  //Function to print unsigned value

  lcd.print(potValue);                              //No need to change value
  delay(150);                                       //Clears display every 150ms,
  lcd.clear();                                      //so it prints a new value instead of previous
  
}

void signedValue(int potValue) {                    //Function to convert unsigned input value to signed
                                                    //"-512" now represents "0" and "511" is "1023"                                 
  int signedValue = map(potValue, 0, 1023, -512, 511); //so 0 (middle value) in the new (signed) range is 512 in previous (unsigned) range
  lcd.print(signedValue);
  delay(150);
  lcd.clear();
  
}

void percentageValue(int potValue) {                //Function to convert unsigned input value to percentage
                                                    //"0" now represents "0" and "100" is "1023"            
  int percentage = map(potValue, 0, 1023, 0, 100);  //so 50 (middle value) in the new (percentage) range is 512 in previous (unsigned) range          
  lcd.print(percentage);
  lcd.print("%");
  delay(150);
  lcd.clear();
  
}

bool secondRow = false;                             //To check if second row is chosen                         
int switchType = 0;                                 //To check current value type

void loop() {

  //read input values
  potValue = analogRead(potentiometer);             
  buttonRowState = digitalRead(buttonRow);
  buttonTypeState = digitalRead(buttonType);


  if (buttonRowState == HIGH) {                     //Button pressed
    if (!secondRow)                                 //First row is chosen                       
      secondRow = true;                             //Switch to the second
    else                                            //Second row is chosen       
      secondRow = false;                            //Switch to the first
  }
  
  
  if (buttonTypeState == HIGH && secondRow) {       //secondRow should be chosen (1), so values in the second row can only be changed if this row is chosen
    
    //switchType has 3 states.
    //First state 0: unsigned
    //Second state 1: signed
    //Third state 2: percentage
    if (switchType != 2)                            //The last state (2) is not reached yet                 
      switchType++;                                 //Switch to the next state
    else                                            //The last state (2) is reached 
      switchType = 0;                               //Switch to the first state
  }


  if (!secondRow) {                                 //First row is chosen
    lcd.setCursor(0, 0);                            //0, 0 - first column and first row
    lcd.print("> ");                                //Symbol that indicates which row is chosen
    binaryValue(potValue);                          //Binary value is always displayed in the first row 
    lcd.setCursor(2, 1);                            //2, 1 - third column and second row

  } else {                                          //Second row is chosen
    lcd.setCursor(2, 0);                            //2, 0 - third column and first row
    binaryValue(potValue);
    lcd.setCursor(0, 1);                            //0, 1 - first column and second row
    lcd.print("> ");
  }

  switch (switchType) {                             //Check which type of value should be displayed in the second row          
    case 0:                                         
      unsignedValue(potValue);                      //Call unsignedValue function and give input value from potentiometer as argument
      
      lcd.setCursor(8, 1);                          
      lcd.print("unsigned");                         
      break;
    case 1:
      signedValue(potValue);                        //Call signedValue function and give input value from potentiometer as argument
      
      lcd.setCursor(8, 1);
      lcd.print("signed");
      break;
    case 2:
      percentageValue(potValue);                    //Call percentageValue function and give input value from potentiometer as argument
      break;
  }
}
