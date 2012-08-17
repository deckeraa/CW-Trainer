/************************************************************************
 *  THIS CODE IS RELEASED UNDER THE
 *  GNU GENERAL PUBLIC LICENSE
 *  Version 2, June 1991
 *
 *  See LICENSE for more information
 *
 ************************************************************************/


#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include "cwsound.h"


void play_note(unsigned char* note, int fd_dsp, int dur)
{
  int status;
  status = write(fd_dsp, note, dur); /* play it back */
  status = ioctl(fd_dsp, SOUND_PCM_SYNC, 0); 
}

#define SAMPLE_RATE		11025

void EncodeSineWave ( unsigned char *Into, int Len, int Pitch )
{
  int x;
  double fAngle;
  unsigned char a;
  int Amplitude;

  fAngle = 0;

  //	this routine fills the region of the tone buffer with a sine wave...
  for ( Amplitude = x = 0; x < Len; x++ )
    {
      //	factor 1 & 2 set the amplitude of the sine wave... By
      //	slowly building it up we avoid any nasty key clicks...

      a = (unsigned char)( Amplitude * sin ( fAngle ));
      a += (unsigned char)128;

      //	give the waveform a gentle start and finish....

      if ( x < 64 )
	{
	  if ( Amplitude < 100 )
	    Amplitude += 2;
	}
      else if ( x >= ( Len - 64 ))
	{
	  if ( Amplitude > 0 )
	    Amplitude -= 2;
	}

      //	poke the byte into the buffer...

      *Into++ = a;

      //	advance the phase of the sine wave...

      fAngle += ( 2 * M_PI * Pitch ) / SAMPLE_RATE;

      //	keep it in the range 0..2*PI

      if ( fAngle > ( 2 * M_PI ))
	fAngle -= ( 2 * M_PI );
    }
}


CWSoundMachine::CWSoundMachine(int speed, int charspacelen, int freq) :
  m_speed(speed),
  m_freq(freq),
  m_charspacelen(charspacelen)
{
  int maxlen = ((SAMPLE_RATE * 1200) / 1000) * 3 ;
  m_ditbuffer = new unsigned char[maxlen];
  m_dahbuffer = new unsigned char[maxlen];

  #ifdef OSS
    /* open sound device */
    m_dsp = open("/dev/dsp", O_WRONLY, 0);
    if (m_dsp < 0) 
    {
      perror("open of /dev/dsp failed");
      _exit(1);
    }
    int value = AFMT_U8;
    ioctl (m_dsp, SNDCTL_DSP_SETFMT, &value);

    int n_fragments = 16; /* number of fragments */
    int fragment_size = 8; /* a buffersize of 2^8 = 256 bytes */

    value = (n_fragments << 16) | fragment_size;
    ioctl (m_dsp, SNDCTL_DSP_SETFRAGMENT, &value);

    value = SAMPLE_RATE; /* frequency */
    ioctl (m_dsp, SNDCTL_DSP_STEREO, &value);

    value = 8; /* number of bits */
    ioctl (m_dsp, SNDCTL_DSP_SAMPLESIZE, &value);

    value = 0; /* mono = 0, stereo = 1 */
    ioctl (m_dsp, SNDCTL_DSP_STEREO, &value);
  #endif /* OSS */

  #ifdef ALSA
    int return_value;
    // open device
    return_value = snd_pcm_open( &m_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if( return_value < 0)
    {
       fprintf(stderr, "Could not open sound device: %s\n", snd_strerror( return_value ));
       _exit(1);
    }

    // get hardware parameters
    snd_pcm_hw_params_alloca(&m_params);
    snd_pcm_hw_params_any(m_handle, m_params); // default values
    
    // don't interleave since we are only using one channel
    snd_pcm_hw_params_set_access(m_handle, m_params, SND_PCM_ACCESS_RW_NONINTERLEAVED);

    // set bit rate
    unsigned int bit_rate = SAMPLE_RATE;
    snd_pcm_hw_params_set_rate_near(m_handle, m_params, &bit_rate, &m_dir);

    // mono ( 1 channel )
    snd_pcm_hw_params_set_channels(m_handle, m_params, 1);
   
    // 16 frames per period
    m_frames = 16;
    snd_pcm_hw_params_set_period_size_near(m_handle, m_params, &m_frames, &m_dir);

    // Use unsigned 8-bit format
    // Because the first version of CWTrainer used OSS and unsigned 8-bit. 
    snd_pcm_hw_params_set_format(m_handle, m_params, SND_PCM_FORMAT_U8);

    // Now write the parameters to the driver
    return_value = snd_pcm_hw_params( m_handle, m_params );
    if ( return_value < 0)
    {
	 fprintf(stderr, "Couldn't set sound parameters :( \n\t%s\n", snd_strerror(return_value));
	 _exit(1);
    }
  #endif /* ALSA */
  BuildBuffers();
}


CWSoundMachine::~CWSoundMachine()
{
  delete [] m_ditbuffer;
  delete [] m_dahbuffer;
  #ifdef ALSA
    snd_pcm_drain(m_handle);
    snd_pcm_close(m_handle);
  #endif /* ALSA */
  #ifdef OSS
    close(m_dsp);
  #endif /* OSS */
}

void CWSoundMachine::AdjustSpeed(int speed)
{
  m_speed = speed;
  BuildBuffers();
}

void CWSoundMachine::BuildBuffers()
{
//   if(m_ditbuffer)
//     delete [] m_ditbuffer;
//   if(m_dahbuffer)
//     delete [] m_dahbuffer;

  m_ditlen = (( SAMPLE_RATE * ( 1200 / m_speed )) / 1000 );
  m_ditlen /= 2;
  m_dahlen = m_ditlen*3;
  m_wordspace = m_dahlen;
  m_charspacelen = m_ditlen;

  //printf("ws: %d - cs: %d\n", m_wordspace, m_charspacelen);

  EncodeSineWave(m_ditbuffer, m_ditlen, m_freq);
  EncodeSineWave(m_dahbuffer, m_dahlen, m_freq);

  memset(m_ditbuffer + m_ditlen, 128, m_ditlen);
  memset(m_dahbuffer + m_dahlen, 128, m_ditlen);

}

void CWSoundMachine::PlayCWNote(const char *note)
{
  for(int i=0 ; note[i] ; i++)
    {
      int dur = note[i] == '.' ? m_ditlen : m_dahlen;
      unsigned char *buf = (note[i] == '.' ? m_ditbuffer : m_dahbuffer);
      void *buffers[1];
      buffers[0] = buf;
      #ifdef ALSA
      //int return_value = snd_pcm_writei(m_handle, buf, m_frames);
      int return_value;
     while ((return_value = snd_pcm_writen(m_handle, buffers, dur+m_ditlen)) < 0) {
          snd_pcm_prepare(m_handle);
//          fprintf(stderr, "xrun !\n");
      }
     /*int return_value = snd_pcm_writen(m_handle, buffers, dur+m_ditlen);
        if( return_value == -EPIPE)
	{
	     fprintf(stderr, "Sound error: underrun\n");
//	     snd_pcm_prepare(m_handle);
	}
	else if (return_value < 0)
	{
	     fprintf(stderr, "Sound error: %s\n", snd_strerror(return_value));
	     }*/
      #endif /* ALSA */
      #ifdef OSS
        play_note(buf, m_dsp, dur+m_ditlen);
      #endif /* OSS */
    }
}
