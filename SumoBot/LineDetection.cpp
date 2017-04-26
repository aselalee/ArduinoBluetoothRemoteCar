
#include "LineDetection.h"

LineDetection::LineDetection(uint8_t frontSensor, uint8_t backSensor):
  m_frontSensorPin(frontSensor)
, m_backSensorPin(backSensor)
, m_prevFrontSensor(BLACK)
, m_prevBackSensor(BLACK)
{
}

LineDetection::~LineDetection()
{
}

void LineDetection::Begin() const
{
    pinMode(m_frontSensorPin, INPUT);
    pinMode(m_backSensorPin, INPUT);
}


LineStatus LineDetection::FrontSensor()
{
    if (digitalRead(m_frontSensorPin) == HIGH)
    {
        return BLACK;
    }
    else
    {
        return WHITE;
    }
}

LineStatus LineDetection::BackSensor()
{
    if (digitalRead(m_backSensorPin) == HIGH)
    {
        return BLACK;
    }
    else
    {
        return WHITE;
    }
}

void LineDetection::PrintLineStatus()
{
    LineStatus front = FrontSensor();
    LineStatus back = BackSensor();
    if (front != m_prevFrontSensor)
    {
        front == BLACK ? Serial.print("BS Black\n"): Serial.print("BS WHITE\n");
        m_prevFrontSensor = front;
    }
    if (back != m_prevBackSensor)
    {
        back == BLACK ? Serial.print("FS Black\n"): Serial.print("FS WHITE\n");
        m_prevBackSensor = back;
    }
}
