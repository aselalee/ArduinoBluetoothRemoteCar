#include <Servo.h>

const uint8_t ENL=11;
const uint8_t ML1=8; //IN1
const uint8_t ML2=9; //IN2

const uint8_t ENR=10;
const uint8_t MR1=6; //IN3
const uint8_t MR2=7; //IN4

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
          if (CmdBuffer[1] == 'U') return FORWARD;
          else if (CmdBuffer[1] == 'L') return FORWARDLEFT;
          else return FORWARDRIGHT;
        case 'D':
          if (CmdBuffer[1] == 'D') return BACK;
          else if (CmdBuffer[1] == 'L') return BACKLEFT;
          else return BACKRIGHT;
        case 'L':
          //no needto check the second letter. There's no other command starting with L.
          return LEFT;
        case 'R':
        //no needto check the second letter. There's no other command starting with R.
          return RIGHT;
        default:
          return STOP;
      }
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
}

void BackLeft() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(ENL, 100);
  analogWrite(ENR, 80);
}

void BackRight() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(ENL, 200);
  analogWrite(ENR, 80);
}

void Forward() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(ENL, 200);
  analogWrite(ENR, 200);
}

void ForwardLeft() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(ENL, 80);
  analogWrite(ENR, 200);
}

void ForwardRight() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(ENL, 200);
  analogWrite(ENR, 80);
}

void Stop() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, LOW);
  analogWrite(ENL, 0);
  analogWrite(ENR, 0);
}

void Right() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(ENL, 50);
  analogWrite(ENR, 200);
}

void Left() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(ENL, 200);
  analogWrite(ENR, 50);
}

Direction cmdDirection;
CommandParser parser;
int servoPos;
int servoOffset;
Servo servo;

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
}

void loop() {
  if(parser.CommandReady())
  {
    cmdDirection = parser.GetDirection();
    parser.PrintCommand();
  }
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
  if (servoPos > 180 || servoPos < 0) servoOffset = servoOffset * -1;
  servoPos += servoOffset;
  servo.write(servoPos);
  delay(15);
}
