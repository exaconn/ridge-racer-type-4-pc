
extern struct 
{
  char pad[0x26C6];
  short h26C6;
  unsigned short h26C8;
  unsigned short h26CA;
  unsigned short h26CC;
} SoundState;
extern int CdAudioSetVolume();
extern int CdAudioMute();
void CdAudioFadeStep()
{
  int d = SoundState.h26C6;
  int t;
  int v;
  int from;
  unsigned c;
  int to;
  if (d > 0)
  {
    c = SoundState.h26C8;
    if (((short) SoundState.h26C8) < d)
    {
      t = c + 1;
      to = SoundState.h26CA;
      from = SoundState.h26CC;
      v = (to - from) << 12;
      v = (v / d) * ((short) t);
      SoundState.h26C8 = t;
      if (v < 0)
      {
        v += 0xFFF;
      }
      v = (v >> 12) + from;
      CdAudioSetVolume(v & 0xFFFF, &SoundState, from);
      return;
    }
    SoundState.h26C6 = 0;
    CdAudioMute(d, &SoundState);
  }
}
