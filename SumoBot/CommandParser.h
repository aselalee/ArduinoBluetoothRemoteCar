
#ifndef _COMMANDPARSER_H_
#define _COMMANDPARSER_H_

#include <Arduino.h>

#define BUFFLEN 8
#define CMDLEN 4
#define CMDTERM '\n'

enum Direction {
  STOP = 0,
  FORWARD,
  FORWARDLEFT,
  FORWARDRIGHT,
  BACK,
  BACKLEFT,
  BACKRIGHT,
  LEFT,
  RIGHT
};

class CommandParser
{
  public:
    CommandParser();
    virtual ~CommandParser();
    void Begin();
    void PrintCommand() const;
    bool CommandReady();
    Direction GetDirection() const;
  private:
    bool IsCmdValidLength() const;
    char m_CmdBuffer[BUFFLEN];
    char* m_pChar;
};

#endif // _COMMANDPARSER_H_
