
#include "Ultrasonic.h"


Ultrasonic::Ultrasonic(uint8_t const trig, uint8_t const echo) :
  m_TrigPin(trig)
, m_EchoPin(echo)
, m_PrevReading(0)
, m_PrevTimeStamp(0)
{
};

Ultrasonic::~Ultrasonic()
{
};

void Ultrasonic::Begin()
{
  pinMode(m_EchoPin, INPUT);
  pinMode(m_TrigPin, OUTPUT);
};

uint32_t Ultrasonic::GetObjectDistance_mm()
{
  if ((millis() - m_PrevTimeStamp) <= 1) //Skipping readas within 1 milli second
  {
    return m_PrevReading;
  }
  digitalWrite(m_TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(m_TrigPin, LOW);
  //Speed of sound 340m/s
  //S = UT; S-Distance, U-Velocity, T-Time
  //Timeout is based on the time required to detect an object 1m away.
  //Sound wave has to travel 1m to the object and then 1m back to the sensor.
  //Timeout = (2/340) * 1000000 micro seconds = 5882.35 ~ 6000;
  uint32_t duration = pulseIn(m_EchoPin, HIGH, 6000);
  //Duration is time taken by the sound wave to travel forward
  //hit the object and then travel back to the receiver.
  //Distance = (340 * (duration/1000000) *1000)/2 mm
  (duration == 0) ? m_PrevReading = 1000 : m_PrevReading = duration * 0.170;
  m_PrevTimeStamp = millis();
  return m_PrevReading;
}
