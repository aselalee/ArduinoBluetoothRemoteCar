
#ifndef _MOTORS_H_
#define _MOTORS_H_

#include "Directions.h"
#include <Arduino.h>

class Motors
{
public:
    Motors(uint8_t const enL,
           uint8_t const in1L,
           uint8_t const in2L,
           uint8_t const enR,
           uint8_t const in1R,
           uint8_t const in2R);
    ~Motors();
    void Begin() const;
    void MoveMotors(Direction dir) const;
private:
    void Back() const;
    void BackLeft() const;
    void BackRight() const;
    void Forward() const;
    void ForwardLeft() const;
    void ForwardRight() const;
    void Stop() const;
    void Right() const;
    void Left() const;
    void EmergencyStop() const;
    uint8_t m_ENL; //ENA
    uint8_t m_ML1; //IN1
    uint8_t m_ML2; //IN2
    uint8_t m_MR1; //IN3
    uint8_t m_MR2; //IN4
    uint8_t m_ENR; //ENB;


};

#endif // _MOTORS_H_
