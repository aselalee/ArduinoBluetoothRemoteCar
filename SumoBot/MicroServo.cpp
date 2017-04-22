
#include "MicroServo.h"

MicroServo::MicroServo(uint8_t const pin,
                       uint16_t const rangeMin,
                       uint16_t const rangeMax,
                       uint16_t const moveOffset):
  m_Pin(pin)
, m_CurServoPos(0)
, m_Servo(Servo())
{
    m_RangeMin = rangeMin < 0 ? 0:rangeMin;
    m_RangeMax = rangeMax > 360 ? 360:rangeMax;
    m_MoveOffset = moveOffset < 0 ? 0:moveOffset;
    m_MoveOffset = moveOffset > 360 ? 360:moveOffset;
}
MicroServo::~MicroServo()
{
}

void MicroServo::Begin() const
{
    m_Servo.attach(m_Pin);
}

void MicroServo::Rotate()
{
    if (m_CurServoPos > m_RangeMax || m_CurServoPos < m_RangeMin) m_MoveOffset = m_MoveOffset * -1;
    m_CurServoPos += m_MoveOffset;
    m_Servo.write(m_CurServoPos);
}

void MicroServo::MoveTo(uint16_t const pos)
{
    m_CurServoPos = pos > 360 ? 360:pos;
    m_CurServoPos = pos < 0 ? 0:pos;
    m_Servo.write(m_CurServoPos);
}

