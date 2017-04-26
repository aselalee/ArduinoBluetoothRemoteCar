
#ifndef _MICROSERVO_H_
#define _MICROSERVO_H_

#include <Arduino.h>
#include <Servo.h>

class MicroServo
{
public:
    MicroServo(uint8_t const pin,
               uint16_t const rangeMin,
               uint16_t const rangeMax,
               uint16_t const moveOffest);
    ~MicroServo();
    void Begin();
    void Rotate();
    void MoveTo(uint16_t const pos);
private:
    uint8_t m_Pin;
    uint16_t m_RangeMin;
    uint16_t m_RangeMax;
    uint16_t m_MoveOffset;
    uint16_t m_CurServoPos;
    Servo m_Servo;
};
#endif //_MICROSERVO_H_
