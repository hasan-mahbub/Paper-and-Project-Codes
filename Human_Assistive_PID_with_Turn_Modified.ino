#define PWMA 11
#define Ain1 A3
#define Ain2 A2
#define PWMB 3
#define Bin1 A1
#define Bin2 A0

int sensor[8] = {4,5,6,7,8,9,10,2};
int sensor_values[5] = {0,0,0,0,0};
float sum = 0, average = 0, error = 0, position = 0;
float propotional = 0, integral = 0, derivative = 0;
float last_error = 0;

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(Ain1, OUTPUT);
  pinMode(Ain2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(Bin1, OUTPUT);
  pinMode(Bin2, OUTPUT);
  
  for (int i=0;i<8;i++){
    pinMode(sensor[i], INPUT);
  }
 
}

void loop() {
  
  pid();
  delay(20);
  
}



void pid()
{
  average = 0; sum = 0;
  for (int i=1;i<=5;i++){
    sensor_values[i] = digitalRead(sensor[i]);
    average += (sensor_values[i]*i);
    sum += sensor_values[i];
  }
  
  while(1){
    
  // Normally, we will be following a line.  The code below is
  // similar to linefollower with pid , but the maximum
  // speed is turned down to 150, 143 for reliability.
  
  position  = average/sum;
  error = position - 3.00;
  propotional = error;
  integral = error + last_error;
  derivative = error - last_error;
  last_error = error;
  
  int gain = ceil(propotional*70.00 + integral*0.00 + derivative*20);
  
  if (error > 0)
     motor(150,(143-gain));
  else if (error < 0)
     motor((150+gain), 143);
     
  delay(20);
  
  if ((digitalRead(5) == HIGH) || (digitalRead(9) == HIGH))
     {
       // Found an intersection.
       break;
     }
     
    //END of infinite while loop
  }
  
  //Check whethere the robot enters straight into intersection
  //This helps us in case we entered the intersection at an angle.
  if(digitalRead(2) == LOW)
  { 
    if((digitalRead(5) == HIGH)){
      //if robot tilts in left direction
      motor(0, 80);
      delay(50);
    }
    else if((digitalRead(9) == HIGH)){
      //if robot tilts in right direction
      motor(90, 0);
      delay(50);
    }
  }
  
  // Drive straight a bit.
  // Note that we are slowing down - this prevents the robot
  // from tipping forward too much.
  motor(100, 93);
  delay(70);
  
  // Check for left and right exits.
  if((digitalRead(9) == HIGH))
    turn(2); //turn Left
  else if((digitalRead(5) == HIGH))
    turn(1); //turn Right if there is no left exit
    
  //END of function, back in loop section with PID
}


void motor(int right_motor, int left_motor)
{
  if (right_motor >255)
    right_motor = 255;
  else if(left_motor > 255)
    left_motor = 255;
    
 if (right_motor < -255)
    right_motor = -255;
 else if(left_motor < -255)
    left_motor = -255;
 
 
 if (right_motor > 0){
     analogWrite(PWMB, right_motor);
     digitalWrite(Bin1, HIGH);
     digitalWrite(Bin2, LOW);
 }
 else
 {
     analogWrite(PWMB, abs(right_motor));
     digitalWrite(Bin1, LOW);
     digitalWrite(Bin2, HIGH);
 }
 
 if (left_motor > 0){
     analogWrite(PWMA, left_motor);
     digitalWrite(Ain1, HIGH);
     digitalWrite(Ain2, LOW);
 }
 else
 {
     analogWrite(PWMA, abs(left_motor));
     digitalWrite(Ain1, LOW);
     digitalWrite(Ain2, HIGH);
 }
}

void turn(int r){
  if(r==1){
    turnRight();
  }

  else if(r==2){
    turnLeft();
  }

  else if(r==3){
    turnStraightLeft();
  }

  else if(r==4){
    turn120Left();
  }

  else if(r==5){
    turn45Right();
  }
  
  else if(r==6){
    turn45Left();
  }
}



void turnRight(){
  stp();
  motor(-100,100);
  delay(150);
  
  //while(digitalRead(7) & digitalRead(5) != 1 & digitalRead(2)==0){
   // motor(-100,100); }
}


void turnLeft(){
  stp();
  motor(100,-100);
  delay(150);
  
  //while(digitalRead(7) & digitalRead(9) !=1){
   // motor(100,-100); }
}

void turnStraightLeft(){
  stp();
  while(digitalRead(7) & digitalRead(9) &  digitalRead(5) !=1){
    motor(100,-100); }
  }

void turn120Left(){
  stp();
  while(digitalRead(7) & digitalRead(2)!=1){
    motor(100,-100); }
  }

void  turn45Right(){
  stp();
  while(digitalRead(6) & digitalRead(4) !=0){
   motor(-100,100); }
  }

void turn45Left(){
  stp();
  while(digitalRead(8) & digitalRead(10) !=0){
    motor(100,-100); }
    }
  



void stp(){
  motor(0,0);
  delay(100);
}
