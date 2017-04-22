
#include "Motors.h"

Motors::Motors(uint8_t const enL,
               uint8_t const in1L,
               uint8_t const in2L,
               uint8_t const enR,
               uint8_t const in1R,
               uint8_t const in2R):
      m_ENL(enL)
    , m_ML1(in1L)
    , m_ML2(in2L)
    , m_MR1(in1R)
    , m_MR2(in2R)
    , m_ENR(enR)
{
}

Motors::~Motors()
{
}

void Motors::MoveMotors(Direction dir) const
{
  switch(dir)
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
    case EMERGENCYSTOP:
      EmergencyStop();
      break;
    default:
      Stop();
      break;
  }
}
void Motors::Begin() const
{
  pinMode(m_ENL, OUTPUT);
  pinMode(m_ML1, OUTPUT);
  pinMode(m_ML2, OUTPUT);
  pinMode(m_ENR, OUTPUT);
  pinMode(m_MR1, OUTPUT);
  pinMode(m_MR2, OUTPUT);
}

void Motors::Back() const
{
  digitalWrite(m_ML1, HIGH);
  digitalWrite(m_ML2, LOW);
  digitalWrite(m_MR1, HIGH);
  digitalWrite(m_MR2, LOW);
  analogWrite(m_ENL, 200);
  analogWrite(m_ENR, 200);
  Serial.print("Back\n");
}

void Motors::BackLeft() const
{
  digitalWrite(m_ML1, HIGH);
  digitalWrite(m_ML2, LOW);
  digitalWrite(m_MR1, HIGH);
  digitalWrite(m_MR2, LOW);
  analogWrite(m_ENL, 200);
  analogWrite(m_ENR, 100);
  Serial.print("BackLeft\n");
}

void Motors::BackRight() const
{
  digitalWrite(m_ML1, HIGH);
  digitalWrite(m_ML2, LOW);
  digitalWrite(m_MR1, HIGH);
  digitalWrite(m_MR2, LOW);
  analogWrite(m_ENL, 200);
  analogWrite(m_ENR, 100);
  Serial.print("BackRight\n");
}

void Motors::Forward() const
{
  digitalWrite(m_ML1, LOW);
  digitalWrite(m_ML2, HIGH);
  digitalWrite(m_MR1, LOW);
  digitalWrite(m_MR2, HIGH);
  analogWrite(m_ENL, 200);
  analogWrite(m_ENR, 200);
  Serial.print("Forward\n");
}

void Motors::ForwardLeft() const
{
  digitalWrite(m_ML1, LOW);
  digitalWrite(m_ML2, HIGH);
  digitalWrite(m_MR1, LOW);
  digitalWrite(m_MR2, HIGH);
  analogWrite(m_ENL, 100);
  analogWrite(m_ENR, 200);
  Serial.print("ForwardLeft\n");
}

void Motors::ForwardRight() const
{
  digitalWrite(m_ML1, LOW);
  digitalWrite(m_ML2, HIGH);
  digitalWrite(m_MR1, LOW);
  digitalWrite(m_MR2, HIGH);
  analogWrite(m_ENL, 200);
  analogWrite(m_ENR, 100);
  Serial.print("ForwardRight\n");
}

void Motors::Stop() const
{
  digitalWrite(m_ML1, LOW);
  digitalWrite(m_ML2, LOW);
  digitalWrite(m_MR1, LOW);
  digitalWrite(m_MR2, LOW);
  analogWrite(m_ENL, 0);
  analogWrite(m_ENR, 0);
  Serial.print("Stop\n");
}

void Motors::Right() const
{
  digitalWrite(m_ML1, LOW);
  digitalWrite(m_ML2, HIGH);
  digitalWrite(m_MR1, HIGH);
  digitalWrite(m_MR2, LOW);
  analogWrite(m_ENL, 200);
  analogWrite(m_ENR, 100);
  Serial.print("Right\n");
}

void Motors::Left() const
{
  digitalWrite(m_ML1, HIGH);
  digitalWrite(m_ML2, LOW);
  digitalWrite(m_MR1, LOW);
  digitalWrite(m_MR2, HIGH);
  analogWrite(m_ENL, 100);
  analogWrite(m_ENR, 200);
  Serial.print("Left\n");
}

void Motors::EmergencyStop() const
{
  Back();
  delay(150);
  Stop();
  Serial.print("Emergency Stop\n");
}

