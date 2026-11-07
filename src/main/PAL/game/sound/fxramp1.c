#include "game/sound/fxramp1.h"
extern struct {
    char pad[0x288A];
    short h288A;
    unsigned short h288C;
    short h288E;
    short h2890;
} SoundState;
extern int SeqSetVolume();
extern int func_80053CB0();

void SeqFadeStep()
{
  int d = SoundState.h288A;
  int t;
  int v;
  int from;
  unsigned c;
  int to;
  if (d > 0)
  {
    c = SoundState.h288C;
    if (((short) SoundState.h288C) < d)
    {
      t = c + 1;
      to = SoundState.h288E;
      from = SoundState.h2890;
      v = (to - from) << 12;
      v = (v / d) * ((short) t);
      SoundState.h288C = t;
      if (v < 0)
      {
        v += 0xFFF;
      }
      v = (v >> 12) + from;
      SeqSetVolume((short)v, &SoundState, from);
      return;
    }
    func_80053CB0(d, &SoundState);
  }
}
