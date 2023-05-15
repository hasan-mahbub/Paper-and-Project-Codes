
#include <LiquidCrystal.h>                        // LCD library
#include <Keypad.h>                               //Keypad library
#include <Servo.h>                                //Servo library


Servo servo1;                                     //Define SERVO 1
Servo servo2;                                     // Define SERVO 2
char data;                                   // Define a char variable 

char* password ="2017";                           // Set Password
int pos = 0;                                     //keypad position

const byte rows = 4;                             //number of the keypad's rows and columns
const byte cols = 4;

char keyMap [rows] [cols] = {                    //define the Symbols on the buttons of the keypad

  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
  
};

byte rowPins [rows] = {2, 3, 4, 5};               //Pins of the keypad connected to Arduino
byte colPins [cols] = {6, 7, 8, 9};

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols);

LiquidCrystal lcd (A0, A1, A2, A3, A4, A5);       // pins of the LCD. (RS, E, D4, D5, D6, D7)

void setup(){

  Serial.begin(9600);                                //Open serial Begin port for communication
  
  servo1.attach(10);                                // Servo 1 attach at Pin 10
  servo2.attach(11);                                // Servo 2 attach at Pin 11
  
  lcd.begin(16, 2);
  
  pinMode(36, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(46, OUTPUT);                        //set the LED as an output
    
  setLocked (true);                           //state of the LOCK

  
}


void loop(){

  //Loop Start for Keypad
  
  
  char whichKey = myKeypad.getKey();                //define which key is pressed with getKey

  lcd.setCursor(0, 0);
  lcd.print(" Authorize Entry");                   //First LCD Print Message
  lcd.setCursor(0, 1);
  lcd.print(" Enter Password");

  if(whichKey == '*' || whichKey == '#' || whichKey == 'A' ||       //define invalid keys
  whichKey == 'B' || whichKey == 'C' || whichKey == 'D'){

    pos=0;                                         //Keypad Back to the Initial state
    setLocked (true);                              //state of the password
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Invalid Key!");                   //If Password Wrong
    delay(1000);
    lcd.clear();
  }
  if(whichKey == password [pos]){

    pos ++;
    
  }
  if(pos == 4){
    
    setLocked (false);                             
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Smart DoorLock");
    lcd.setCursor(0, 1);
    lcd.print(" Project Done!!");
    delay(7000);
    lcd.clear();
    
  }
  delay(100);

   //Loop end for Keypad

   //Loop Start for Bluetooth Connection
   
  if(Serial.available() > 0)  
  {
    data = Serial.read();      
    Serial.print(data);                                //Prints received data to the serial monitor    
    Serial.print("\n");        
    if(data == '1')
    {  
                
       for(int a = 70; a <= 150; a++)                  // Rotates the SERVO 1 to the LOCKED position
          {
            servo1.write(a);
            
          }
          digitalWrite(36, HIGH);                      // turn the RED LED ON
          digitalWrite(38, LOW);
          digitalWrite(42, LOW);
          digitalWrite(46, LOW);
    }
          
    else if(data == '0'){
          
      
      for(int a = 150; a >= 70; a--)                  // Rotates the SERVO 1 to the UNLOCKED position
          {
            servo1.write(a);
                
    }

           digitalWrite(36, LOW);   // turn the Green LED ON
           digitalWrite(38, LOW);
           digitalWrite(42, HIGH);
           digitalWrite(46, LOW);
  }

    else if(data == '3'){
      
      
      for(int b = 70; b >= 0; b--)                // Rotates the SERVO 2 to the OPENED position
      {

        servo2.write(b);
        delay(5);
      }
    }

    else if(data == '4'){

      
      
      for(int b = 0; b <= 70; b++)              // Rotates the SERVO 2 to the CLOSED position
      {

        servo2.write(b);
        delay(5);
        
      }
    }
 
}                                               //Loop end for Bluetooth Connection


}



void setLocked(int locked){
  
  if(locked){                                 //If Password Wrong - Default State

    

     digitalWrite(36, HIGH);                  // turn the RED LED ON
     digitalWrite(38, LOW);
     digitalWrite(42, LOW);
     digitalWrite(46, LOW);

    for(int b = 70; b <= 70; b++)              // Rotates the SERVO 2 to the CLOSED position
      {

        servo2.write(b);
        delay(5);
        
      }
      delay(5);
    
    //for(int a = 70; a <= 150; a++)              // Rotates the SERVO 1 to the locked position
          {
            //servo1.write(a);
            
          }
    }

    
    else{
                                                //If Password Right - Unlocked State
    
    
    digitalWrite(36, LOW);   // turn the Green LED ON
    digitalWrite(38, LOW);
    digitalWrite(42, HIGH);
    digitalWrite(46, LOW);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** Verified ***");
      for(int a = 150; a >= 70; a--)           // Rotates the SERVO 1 to the Unlocked position
          {
            servo1.write(a);
                
    }
    delay(1000);

    for(int b = 70; b >= 0; b--)                // Rotates the SERVO 2 to the OPENED position
      {

        servo2.write(b);
        delay(5);
      }
    delay(7000);

    for(int b = 0; b <= 70; b++)              // Rotates the SERVO 2 to the CLOSED position
      {

        servo2.write(b);
        delay(5);
        
      }

     delay(1000);
     
    for(int a = 70; a <= 150; a++)              // Rotates the SERVO 1 to the LOCKED position
          {
            servo1.write(a);
            
          }

     digitalWrite(36, HIGH);   // turn the RED LED ON
     digitalWrite(38, LOW);
     digitalWrite(42, LOW);
     digitalWrite(46, LOW);
    }
    pos=0;                                //Keypad Back to the Initial state
  }
