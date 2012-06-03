/************************************************************************
 *  THIS CODE IS RELEASED UNDER THE
 *  GNU GENERAL PUBLIC LICENSE
 *  Version 2, June 1991
 *
 *  See LICENSE for more information
 *
 ************************************************************************/

#ifndef __CW__SOUND__H__
#define __CW__SOUND__H__

void makesound();

class CWSoundMachine
{
 private:
  int m_dsp; //handle to dsp device
  int m_speed;
  int m_freq;
  unsigned char *m_ditbuffer;
  unsigned char *m_dahbuffer;
  int m_ditlen;
  int m_dahlen;
  int m_charspacelen;
  int m_wordspace;

  void BuildBuffers();

  //prevent copies
  CWSoundMachine(const CWSoundMachine& cw){};
 public:
  CWSoundMachine(int speed, int charspacelen, int freq);
  ~CWSoundMachine();

  void AdjustSpeed(int speed);

  void PlayCWNote(const char *note);
  int CharSpaceLen(){return m_charspacelen;}
  int WordSpace(){return m_wordspace;}
};

#endif
