
#ifndef _LINEDETECTION_H_
#define _LINEDETECTION_H_

#include <Arduino.h>

enum LineStatus
{
    WHITE = 0,
    BLACK
};

class LineDetection
{
public:
    LineDetection(uint8_t frontSensor, uint8_t backSensor);
    ~LineDetection();
    void Begin() const;
    LineStatus BackSensor();
    LineStatus FrontSensor();
    void PrintLineStatus();
private:
    uint8_t m_frontSensorPin;
    uint8_t m_backSensorPin;
    bool m_prevFrontSensor;
    bool m_prevBackSensor;
};

#endif //_LINEDETECTION_H_
