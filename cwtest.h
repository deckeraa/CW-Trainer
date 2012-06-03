/************************************************************************
 *  THIS CODE IS RELEASED UNDER THE
 *  GNU GENERAL PUBLIC LICENSE
 *  Version 2, June 1991
 *
 *  See LICENSE for more information
 *
 ************************************************************************/

#ifndef __CW__TEST__H__
#define __CW__TEST__H__

#include <map>
#include <string>

class CWTest
{
 private:
  struct CWTestChar
  {    
    std::string m_morse;
    bool m_inuse;
    CWTestChar(const char* morse, bool inuse) :
      m_morse(morse), m_inuse(inuse){}
    CWTestChar(){}
  };

  typedef std::map<char, CWTest::CWTestChar> CWCharMap;
  CWCharMap m_cw;

  CWTest()
  {
    m_cw['A'] = CWTestChar(".-", true);
    m_cw['B'] = CWTestChar("-...", true);
    m_cw['C'] = CWTestChar("-.-.", true);
    m_cw['D'] = CWTestChar("-..", true);
    m_cw['E'] = CWTestChar(".", true);
    m_cw['F'] = CWTestChar("..-.", true);
    m_cw['G'] = CWTestChar("--.", true);
    m_cw['H'] = CWTestChar("....", true);
    m_cw['I'] = CWTestChar("..", true);
    m_cw['J'] = CWTestChar(".---", true);
    m_cw['K'] = CWTestChar("-.-", true);
    m_cw['L'] = CWTestChar(".-..", true);
    m_cw['M'] = CWTestChar("--", true);
    m_cw['N'] = CWTestChar("-.", true);
    m_cw['O'] = CWTestChar("---", true);
    m_cw['P'] = CWTestChar(".--.", true);
    m_cw['Q'] = CWTestChar("--.-", true);
    m_cw['R'] = CWTestChar(".-.", true);
    m_cw['S'] = CWTestChar("...", true);
    m_cw['T'] = CWTestChar("-", true);
    m_cw['U'] = CWTestChar("..-", true);
    m_cw['V'] = CWTestChar("...-", true);
    m_cw['W'] = CWTestChar(".--", true);
    m_cw['X'] = CWTestChar("-..-", true);
    m_cw['Y'] = CWTestChar("-.--", true);
    m_cw['Z'] = CWTestChar("--..", true);
    m_cw['1'] = CWTestChar(".----", true);
    m_cw['2'] = CWTestChar("..---", true);
    m_cw['3'] = CWTestChar("...--", true);
    m_cw['4'] = CWTestChar("....-", true);
    m_cw['5'] = CWTestChar(".....", true);
    m_cw['6'] = CWTestChar("-....", true);
    m_cw['7'] = CWTestChar("--...", true);
    m_cw['8'] = CWTestChar("---..", true);
    m_cw['9'] = CWTestChar("----.", true);
    m_cw['0'] = CWTestChar("-----", true);
    m_cw['.'] = CWTestChar(".-.-.-", true);
    m_cw[','] = CWTestChar("--..--", true);
    m_cw['?'] = CWTestChar("..--..", true);
    m_cw['/'] = CWTestChar("-..-.", true);
    m_cw[' '] = CWTestChar(" ", true);
  }

  static CWTest& GetChars()
  {
    static CWTest cw;
    return cw;
  }

 public:
  static bool CharInUse(char ch)
    {
      CWCharMap::const_iterator iter = GetChars().m_cw.find(ch);

      if(iter == GetChars().m_cw.end())
	{
	  return false;
	}

      return (*iter).second.m_inuse;
    }

  static void SetCharInUse(char ch, bool state)
    {
      CWCharMap::const_iterator iter = GetChars().m_cw.find(ch);

      if(iter == GetChars().m_cw.end())
	{
	  return;
	}

      GetChars().m_cw[ch].m_inuse = state;
    }
  
  static const std::string& GetCW(char ch)
    {
      CWCharMap::const_iterator iter = GetChars().m_cw.find(ch);

      if(iter == GetChars().m_cw.end())
	{
	  return GetChars().m_cw[' '].m_morse;
	}

      return GetChars().m_cw[ch].m_morse;
    }
};


#endif
