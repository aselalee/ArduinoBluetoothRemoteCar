
#ifndef _COMMANDPARSER_H_
#define _COMMANDPARSER_H_

#include "Directions.h"
#include <Arduino.h>

#define BUFFLEN 8
#define CMDLEN 4
#define CMDTERM '\n'

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
