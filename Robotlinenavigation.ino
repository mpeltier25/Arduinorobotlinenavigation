#include <Ping.h>
#include "Arduino.h"
#include <DCMotor.h>
#include <IRRanger.h>
// MOTOR
DCMotor motor_R(M0_EN, M0_D0, M0_D1);
DCMotor motor_L(M1_EN, M1_D0, M1_D1);
char state = 0;
short spd_l;
short spd_r;
char accel = 1;
char init_spd = 20;
const char max_spd = 50;
const char accel_inc = 0;
int buttonState = 0;
int angle = 0;
char turnspdOut = 18;
char turnspdIn = 22;
PingSensor ping(A0);
IRRanger irRangerL(3, IRRanger::range10to80cm);
IRRanger irRangerR(4, IRRanger::range10to80cm);
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo for the robot
 
int pos = 0;    // variable to store the servo position 
int inc = 15;
float srange = 0;

void setup()
{
  //pinMode(SPEAKER, OUTPUT);
  myservo.attach(A5);  // attaches the servo on pin 9 to the servo object 
  
}

int readLights()
{
  int leftLight, rightLight;
  // Check the light sensors, figure out where the light is
  // relative to robot's front
  leftLight = analogRead(A1);
  rightLight = -analogRead(A2);
  
  // Crude linear estimation
  return leftLight + rightLight; 
}

int checkWalls()
{
  int range=10;
  int irRangeCM=0;
  if(ping.measureCM()<=range){
     motor_R.setSpeed(-500);
     motor_L.setSpeed(-500);
  }
//  state = 1;
/*  if(irRangerL.measureCM() >= 3)
  {
    motor_L.setSpeed(0);
    motor_R.setSpeed(100);
  }
  if(irRangerR.measureCM()>= 3)
  {
    motor_R.setSpeed(0);
    motor_L.setSpeed(100);
  }*/
  //int leftIR, rightIR;
  //leftIR = analogRead(A3);
  //rightIR = -analogRead(A4);
  // This is just temporary, linear estimation.
  //return leftIR + rightIR;
}

int emergencybackup()
{
 int range=200;
 //if(irRangerL.measureCM()>=range)
 //{
 //motor_R.setSpeed(-500);
 //motor_L.setSpeed(0);
 //}
 if(irRangerR.measureCM()<=range)
 {
  motor_R.setSpeed(0);
  //motor_L.setSpeed(-500);
 }
}

void loop()
{
  switch (state) {
    case 0:
    /*
    This is where the robot switches state if it detects the black line, keeping in mind walls and other objects that might
    interfere with the line navigation
    */
      if (readLights() > 0) {
        checkWalls();
        //emergencybackup();
        motor_R.setSpeed(40);
        motor_L.setSpeed(-20); 
      } else {
        checkWalls();
        //emergencybackup();
        motor_R.setSpeed(20);
        motor_L.setSpeed(-40);
      }
        srange = pos/(2.5-(pos*.0022));
    myservo.write(srange);
  //delay(15);
  pos = pos + inc;
  if (pos > 340) inc = -15;
  if (pos < 0) inc = 15;

    break;
    case 1:
      //showLights();      
      motor_R.setSpeed(-spd_l);
      motor_L.setSpeed(spd_r);

    break;
    case 2:
    break;
  }
}
