
#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

#include <Arduino.h>

class Ultrasonic
{
  public:
    Ultrasonic(uint8_t const trig, uint8_t const echo);
    virtual ~Ultrasonic();
    void Begin();
    uint32_t GetObjectDistance_mm();
  private:
    uint8_t m_TrigPin;
    uint8_t m_EchoPin;
    uint32_t m_PrevReading;
    uint32_t m_PrevTimeStamp;
};

#endif //_ULTRASONIC_H_
