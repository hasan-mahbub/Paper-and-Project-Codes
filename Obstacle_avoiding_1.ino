/////////////////////////////////////////////////////////////////
#include "MotorClassL.h"
#include <string.h>
unsigned long long start;
unsigned long long s1;
unsigned long long s2;
int value[8];
boolean isMotorEnabled;
MotorClassL mc;
int M1,M2;//110  140
int m1Speed,m2Speed,boostSpeed;
//////////////////////////////////////////////////////////////////
#define echoPinRight 47
#define trigPinRight 49
/////////////////////////////////////////////////////////////////
#define NUM_SENSOR 8
#define WHITE 1
#define BLACK 0
#define SENSOR_DEBUG
#define CALIBON
#define echoPinF 46       // Echo Pin
#define trigPinF 28       // Trigger Pin
#define echoPinL 32
#define trigPinL 30
//unsigned int sensorA[NUM_SENSOR];
//unsigned int sensorAr2[NUM_SENSOR];
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed

int b;
int   irSensors = B00000000;
int sensorPin[]={A8,A9,A10,A11,A12,A13,A14,A15};
int sensorA[NUM_SENSOR];
int chk[NUM_SENSOR];
//int sensorD[NUM_SENSOR];
int sensorMin[NUM_SENSOR];
int sensorMax[NUM_SENSOR];
int lineColor = WHITE;
int sum;
//////////////////////////////////////////////////////////////////
int lastError=0,lastValue=0;
float KP,KD;
//////////////////////////////////////////////////////////////////
long durationF, distanceF;
long durationRight, distanceRight;

void avoidObstacle()
{
  mc.brake();
  delay(200);
  mc.motorForward(0,-110);
  delay(500);
  mc.brake();
  delay(200);
  mc.motorForward(45,70);
  delay(1000);
  mc.motorForward(0,90);
  delay(900);
  while(1)
  {
     chkPat();
     mc.motorForward(55,90);
    if(chk[3]==0&&chk[4]==0)break; 
  }
  while(1)
  {
   chkPat();
   mc.motorForward(40,80);
   if(chk[3]==1&&chk[4]==1) break;
  }
}

void setup()
{
    Serial.begin(115200);
    sensorInit();
//    M1 = 85;
//    M2 = 135;
//    M1 = 70;
//    M2 = 110;
    M1 = 40;
    M2 = 60;
    //isMotorEnabled=true;
    m1Speed = m2Speed = 0;
    start = millis();
    KP=.32;
    KD=.9;
    pinMode(trigPinF, OUTPUT);
    pinMode(echoPinF, INPUT);
    pinMode(trigPinRight, OUTPUT);
    pinMode(echoPinRight, INPUT);
    
}

void SharpRight()
{
    mc.motorForward(-80,80);
    delay(365);
    mc.motorForward(0,0);
    delay(500);
}
void Rotate()
{
    mc.motorForward(-80,80);
    delay(700);
    mc.motorForward(0,0);
    delay(300);
}

int trigger = 0,c=1;
void loop()
{
    chkPat();
    if(irSensors==B11111111)
    {
      //mc.brake();
      detectionRight();
      if(distanceRight<=25)
      {
        M1 = 40;
        M2 = 65;
        cave();
//      M1 = 85;
//      M2 = 135;
        M1 = 70;
        M2 = 110;
      }
      else
      {
        if(c == 1)
        {
          mc.motorForward(M1,-M2);
//          mc.brake();
          delay(210);
          c=2;
        }
        else if(c == 2)
        {
          mc.motorForward(-M1,M2);
//          mc.brake();
          delay(180);
        }
      }     
    }
    else if(irSensors==B11111100)
    {
      mc.motorForward(M1,-M2);
      delay(250);
    }
    else pid();
      
//    detectionRight();
//    Serial.println(distanceRight);
//    delay(1000);
    Serial.println();
}


void pidCalib(){
  if(Serial.available()){
    char x=Serial.read();
    if(x=='2'){
      KP+=.01;
    }
    else if(x=='1'){
      KP-=.01;
    }
    else if(x=='9'){
      KD+=.01;
    }
    else if(x=='0'){
      KD-=.01;
    }
  }
}
void pid()
{
  int pos=sensorRead();
  #ifdef SENSOR_DEBUG
  //Serial.print('\t');
  //Serial.print(pos);
  //Serial.println();
  #endif
  pos-=3500;
  //int error = ((pos)-2500)/10;
  int error=( ((float)pos)/2500*250 );
  //error-=200;
  if(error == -350 && irSensors==B00000000) error = -450;
#ifdef SENSOR_DEBUG
  Serial.print('\t');
  Serial.print(error);
  Serial.print('\t');
  Serial.print(KP);
  Serial.print('\t');
  Serial.print(KD);
  Serial.print('\t');
#endif
  if((lastError>=-50 && lastError<=50) && (error == 450 || error == -450))
  {
    mc.motorForward(m1Speed,m2Speed);
    //lastError = error;
  }
  else
  {
    int motorSpeed = KP * error + KD * (error - lastError);
    lastError = error;
    Serial.print(motorSpeed);
    if(motorSpeed<0){
      m2Speed=M2+(1.65*(float)motorSpeed);
      m1Speed=M1;
    }
    else {
      m1Speed=M1-(motorSpeed);
      m2Speed=M2;
    }
    int mint=-10;
    /*if (m1Speed < 0)
      m1Speed = 0;
    if (m2Speed < 0)
      m2Speed = 0;*/
    
    mc.motorForward(m1Speed,m2Speed);
  }
}

//////////////////////////////////
  
void detection()
{
   digitalWrite(trigPinF, LOW); 
   delayMicroseconds(2); 
   digitalWrite(trigPinF, HIGH);
   delayMicroseconds(10); 
   digitalWrite(trigPinF, LOW);
   durationF = pulseIn(echoPinF, HIGH, 15000);
   distanceF = durationF/58.2;
   if(distanceF == 0) distanceF = 200;
}
void detectionRight()
{
   digitalWrite(trigPinRight, LOW); 
   delayMicroseconds(2); 
   digitalWrite(trigPinRight, HIGH);
   delayMicroseconds(10); 
   digitalWrite(trigPinRight, LOW);
   durationRight = pulseIn(echoPinRight, HIGH, 15000);
   distanceRight = durationRight/58.2;
   if(distanceRight == 0) distanceRight = 200;
}

void cave()
{
   while(irSensors==B11111111)
   {
     chkPat();
     detectionRight();
     if(distanceRight<16) mc.motorForward(M1-10,M2+15);
     else if(distanceRight>16) mc.motorForward(M1+15,M2-30);
   }
   c = 1;
}
