#include <Servo.h>

const uint8_t ENL=6; //ENA
const uint8_t ML1=7; //IN1
const uint8_t ML2=8; //IN2

const uint8_t MR1=13; //IN3
const uint8_t MR2=12; //IN4
const uint8_t ENR=11; //ENB;
const uint8_t TRIG = A0;
const uint8_t ECHO = A1;

enum Direction {
  STOP,
  FORWARD,
  FORWARDLEFT,
  FORWARDRIGHT,
  BACK,
  BACKLEFT,
  BACKRIGHT,
  LEFT,
  RIGHT
};

#define BuffSize 8
#define CmdLength 4
class CommandParser
{
  public:
    CommandParser() {};
    ~CommandParser() {};
    void Reset()
    {
      pChar = CmdBuffer;
      memset(CmdBuffer, 0, BuffSize);
    }
    void PrintCommand()
    {
      Serial.print("<");
      Serial.print(CmdBuffer);
    }
    bool IsCmdValidLength()
    {
      uint8_t count = 0;
      for(count;(count < BuffSize) && (CmdBuffer[count] != '\0');count++);
      return count == CmdLength;
    }
    bool CommandReady()
    {
      while (Serial.available() > 0)
      {
        if (pChar < (CmdBuffer + BuffSize - 1)) //One character for null termination.
        {
          *pChar = Serial.read();
          if (*pChar == '\n')
          {
            //Have received a full command.
            //Return now. Rest of the the serial data if any will
            //be read next time as the next command.
            *(pChar + 1) = '\0'; //Null terminate the string
            pChar = CmdBuffer; //Reset pointer
            return IsCmdValidLength(); //If command length is invalid, it's automatically discarded.
          }
          pChar++;
        }
        else
        {
          //Erronous Command.
          //We expect all commands to be less than BuffSize - 1.
          //Reset pointer.
          pChar = CmdBuffer;
        }
      }
      return false;
    }
    Direction GetDirection()
    {
      Direction dir = STOP;
      switch (CmdBuffer[0])
      {
        case 'U':
          if (CmdBuffer[1] == 'U') dir = FORWARD;
          else if (CmdBuffer[1] == 'L') dir = FORWARDLEFT;
          else dir = FORWARDRIGHT;
          break;
        case 'D':
          if (CmdBuffer[1] == 'D') dir = BACK;
          else if (CmdBuffer[1] == 'L') dir = BACKLEFT;
          else dir = BACKRIGHT;
          break;
        case 'L':
          //no needto check the second letter. There's no other command starting with L.
          dir = LEFT;
          break;
        case 'R':
        //no needto check the second letter. There's no other command starting with R.
          dir = RIGHT;
          break;
        default:
          dir = STOP;
      }
      return dir;
    }
  private:
    char CmdBuffer[BuffSize];
    char* pChar;
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
  analogWrite(ENL, 100);
  analogWrite(ENR, 80);
  Serial.print("BackLeft\n");
}

void BackRight() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(ENL, 200);
  analogWrite(ENR, 80);
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
  analogWrite(ENL, 80);
  analogWrite(ENR, 200);
  Serial.print("ForwardLeft\n");
}

void ForwardRight() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(ENL, 200);
  analogWrite(ENR, 80);
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
  analogWrite(ENL, 50);
  analogWrite(ENR, 200);
  Serial.print("Right\n");
}

void Left() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(ENL, 200);
  analogWrite(ENR, 50);
  Serial.print("Left\n");
}

void EmergencyStop()
{
  Back();
  delay(250);
  Stop();
  Serial.print("Emergency Stop\n");
}

class Ultrosonic
{
  public:
    Ultrosonic(uint8_t trig, uint8_t echo) :
      trigPin(trig)
    , echoPin(echo)
    , prevReading(0)
    , prevTimeStamp(0)
    {
    };
    ~Ultrosonic() {};
    uint32_t GetObjectDistance_1() { return 101; }
    uint32_t GetObjectDistance_mm()
    {
      if ((millis() - prevTimeStamp) <= 60)
      {
        return prevReading;
      }
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      //Speed of sound 340m/s
      //S = UT; S-Distance, U-Velocity, T-Time
      //Timeout is based on the time required to detect an object 1m away.
      //Sound wave has to travel 1m to the object and then 1m back to the sensor.
      //Timeout = (2/340) * 1000000 micro seconds = 5882.35 ~ 6000;
      uint32_t duration = pulseIn(echoPin, HIGH, 6000);
      //Duration is time taken by the sound wave to travel forward
      //hit the object and then travel back to the receiver.
      //Distance = (340 * (duration/1000000) *1000)/2 mm
      (duration == 0) ? prevReading = 1000 : prevReading = duration * 0.170;
      prevTimeStamp = millis();
      return prevReading;
    }
  private:
    uint8_t trigPin;
    uint8_t echoPin;
    uint32_t prevReading;
    uint32_t prevTimeStamp;
};

Direction cmdDirection = STOP;
CommandParser parser;
int16_t servoPos;
int8_t servoOffset;
Servo servo;
Ultrosonic sonicSensor(TRIG,ECHO);

void setup() {
  Serial.begin(9600);
  parser.Reset();
  servo.attach(4);
  servoPos = 0;
  servoOffset = 2;
  pinMode(ENL, OUTPUT);
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(ENR, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  servo.write(90); 
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
  if (sonicSensor.GetObjectDistance_mm() < 100 &&
      (cmdDirection == FORWARD ||
       cmdDirection == FORWARDLEFT ||
       cmdDirection == FORWARDRIGHT) )
  {
    EmergencyStop();
    cmdDirection = STOP;
  }
  //if (servoPos > 180 || servoPos < 0) servoOffset = servoOffset * -1;
  //servoPos += servoOffset;
  //servo.write(servoPos);
  //delay(15); //Settle servo
}
