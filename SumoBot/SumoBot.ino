#include "CommandParser.h"
#include "Ultrasonic.h"
#include "Motors.h"
#include "MicroServo.h"
#include "Directions.h"

const uint8_t ENL = 6;  //ENA
const uint8_t ML1 = 7;  //IN1
const uint8_t ML2 = 8;  //IN2
const uint8_t MR1 = 13; //IN3
const uint8_t MR2 = 12; //IN4
const uint8_t ENR = 11; //ENB;

const uint8_t TRIG = A0; //Ultrasonic trigger
const uint8_t ECHO = A1; //Ultrasonic echo

const uint8_t FS = 2; //Line sensor front
const uint8_t BS = 3; //Line sensor back

const uint8_t MS = 4; //Micro servo

enum State {
  RUNNING,
  HALT
};

Direction cmdDirection;
State currentState;
int16_t servoPos;
int8_t servoOffset;
uint32_t lineDetectDelay;

CommandParser parser;
MicroServo servo(MS, 0, 180, 2);
Ultrasonic sonicSensor(TRIG, ECHO);
Motors motors(ENL, ML1, ML2, ENR, MR1, MR2);

void setup() {
  Serial.begin(9600);
  parser.Begin();
  sonicSensor.Begin();
  motors.Begin();
  servo.Begin();
  servo.MoveTo(90);
  
  cmdDirection = STOP;
  currentState = HALT;
  lineDetectDelay = 0; 
  pinMode(FS, INPUT);
  pinMode(BS, INPUT);
}

void loop() {
  if(parser.CommandReady())
  {
    cmdDirection = parser.GetDirection();
    parser.PrintCommand();
    motors.MoveMotors(cmdDirection);

  }
  if (sonicSensor.GetObjectDistance_mm() < 110)
  {
    if(cmdDirection == FORWARD ||
       cmdDirection == FORWARDLEFT ||
       cmdDirection == FORWARDRIGHT)
    {
      cmdDirection = STOP;
      motors.MoveMotors(cmdDirection);
      if (currentState == RUNNING)
      {
        motors.MoveMotors(EMERGENCYSTOP);
      }
    }
    currentState = HALT;
  }
  else
  {
    currentState = RUNNING;
  }
  if ((millis() - lineDetectDelay) > 500) //Skipping readas within 500 milli second
  {
    //digitalRead(BS) == HIGH ? Serial.print("BS Black\n"): Serial.print("BS WHITE\n");
    //digitalRead(FS) == HIGH ? Serial.print("FS Black\n"): Serial.print("FS WHITE\n");
    lineDetectDelay = millis();
  }
  //servo.Rotate();
  //delay(15); //Settle servo
}
