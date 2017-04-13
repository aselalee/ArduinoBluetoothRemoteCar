#include "CommandParser.h"
#include "Ultrasonic.h"
#include <Servo.h>

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

void Back() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(ENL, 200);
  analogWrite(ENR, 200);
  Serial.print("Back\n");
}

void BackLeft() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(ENL, 200);
  analogWrite(ENR, 100);
  Serial.print("BackLeft\n");
}

void BackRight() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(ENL, 200);
  analogWrite(ENR, 100);
  Serial.print("BackRight\n");
}

void Forward() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(ENL, 200);
  analogWrite(ENR, 200);
  Serial.print("Forward\n");
}

void ForwardLeft() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(ENL, 100);
  analogWrite(ENR, 200);
  Serial.print("ForwardLeft\n");
}

void ForwardRight() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(ENL, 200);
  analogWrite(ENR, 100);
  Serial.print("ForwardRight\n");
}

void Stop() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, LOW);
  analogWrite(ENL, 0);
  analogWrite(ENR, 0);
  Serial.print("Stop\n");
}

void Right() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(ENL, 200);
  analogWrite(ENR, 100);
  Serial.print("Right\n");
}

void Left() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(ENL, 100);
  analogWrite(ENR, 200);
  Serial.print("Left\n");
}

void EmergencyStop()
{
  Back();
  delay(150);
  Stop();
  Serial.print("Emergency Stop\n");
}


Direction cmdDirection;
State currentState;
int16_t servoPos;
int8_t servoOffset;
uint32_t lineDetectDelay;

CommandParser parser;
Servo servo;
Ultrasonic sonicSensor(TRIG,ECHO);

void setup() {
  Serial.begin(9600);
  parser.Begin();
  sonicSensor.Begin();
  servo.attach(MS);
  cmdDirection = STOP;
  currentState = HALT;
  servoPos = 0;
  servoOffset = 2;
  servo.write(90);
  lineDetectDelay = 0; 
  pinMode(ENL, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(ENR, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(FS, INPUT);
  pinMode(BS, INPUT);
}

void loop() {
  if(parser.CommandReady())
  {
    cmdDirection = parser.GetDirection();
    parser.PrintCommand();
    switch(cmdDirection)
    {
      case FORWARDRIGHT:
        ForwardRight();
        break;
      case FORWARD:
        Forward();
        break;
      case FORWARDLEFT:
        ForwardLeft();
        break;
      case BACKRIGHT:
        BackRight();
        break;
      case BACK:
        Back();
        break;
      case BACKLEFT:
        BackLeft();
        break;
      case LEFT:
        Left();
        break;
      case RIGHT:
        Right();
        break;
      default:
        Stop();
        break;
    }
  }
  if (sonicSensor.GetObjectDistance_mm() < 110)
  {
    if(cmdDirection == FORWARD ||
       cmdDirection == FORWARDLEFT ||
       cmdDirection == FORWARDRIGHT)
    {
      Stop();
      if (currentState == RUNNING)
      {
        EmergencyStop();
      }
      cmdDirection = STOP;
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
  //if (servoPos > 180 || servoPos < 0) servoOffset = servoOffset * -1;
  //servoPos += servoOffset;
  //servo.write(servoPos);
  //delay(15); //Settle servo
}
