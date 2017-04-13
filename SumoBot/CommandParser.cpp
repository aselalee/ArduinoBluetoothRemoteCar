
#include "CommandParser.h"

CommandParser::CommandParser()
{
}

CommandParser::~CommandParser()
{
}

void CommandParser::Begin()
{
  m_pChar = m_CmdBuffer;
  memset(m_CmdBuffer, 0, BUFFLEN);
}

void CommandParser::PrintCommand() const
{
  Serial.print("<");
  Serial.print(m_CmdBuffer);
}

bool CommandParser::CommandReady()
{
  while (Serial.available() > 0)
  {
    if (m_pChar < (m_CmdBuffer + BUFFLEN - 1)) //One character for null termination.
    {
      *m_pChar = Serial.read();
      if (*m_pChar == CMDTERM)
      {
        //Have received a full command.
        //Return now. Rest of the the serial data if any will
        //be read next time as the next command.
        *(m_pChar + 1) = '\0'; //Null terminate the string
        m_pChar = m_CmdBuffer; //Reset pointer
        return IsCmdValidLength(); //If command length is invalid, it's automatically discarded.
      }
      m_pChar++;
    }
    else
    {
      //Erronous Command.
      //We expect all commands to be less than BuffSize - 1.
      //Reset pointer.
      m_pChar = m_CmdBuffer;
    }
  }
  return false;
}

Direction CommandParser::GetDirection() const
{
  Direction dir = STOP;
  switch (m_CmdBuffer[0])
  {
    case 'U':
      if (m_CmdBuffer[1] == 'U') dir = FORWARD;
      else if (m_CmdBuffer[1] == 'L') dir = FORWARDLEFT;
      else dir = FORWARDRIGHT;
      break;
    case 'D':
      if (m_CmdBuffer[1] == 'D') dir = BACK;
      else if (m_CmdBuffer[1] == 'L') dir = BACKLEFT;
      else dir = BACKRIGHT;
      break;
    case 'L':
      //no needto check the second letter. There's no other command starting with L.
      dir = LEFT;
      break;
    case 'R':
    //no needto check the second letter. There's no other command starting with R.
      dir = RIGHT;
      break;
    default:
      dir = STOP;
  }
  return dir;
}

bool CommandParser::IsCmdValidLength() const
{
  uint8_t count = 0;
  for(count;(count < BUFFLEN) && (m_CmdBuffer[count] != '\0');count++);
  return count == CMDLEN;
}

