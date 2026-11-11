#include "rr/channels.h"
#include "rr/spu.h"
#include "game/data/globals.h"


s32 _SsVmSetVol(s32 a0, s16 a1, s16 a2, u16 a3, u16 a4)
{
  u8 new_var;
  s16 i;
  ChannelValueRecordB0 *rec;
  s32 toneIndex;
  u32 vol;
  u32 left;
  u32 right;
  u32 l;
  u32 r;
  u8 pan;
  s32 count = 0;
  s32 k;
  s16 ch;
  rec = _ss_score[a0 & 0xFF] + ((((s16) a0) & 0xFF00) >> 8);
  _SsVmVSetUp(a1, a2);
  _svm_cur_vab_id = a0;
  if (a4 == 0)
  {
    a4 = 1;
  }
  if (a3 == 0)
  {
    a3 = 1;
  }
  i = 0;
  if (count < spuVmMaxVoice)
  do {
    new_var = _svm_vh[0x18];
    if (!(_snd_vmask & (1 << i)))
    {
      if (((_svm_voice[i].match10 == ((s16) a0)) && (_svm_voice[i].match14 == a2)) && (_svm_voice[i].match18 == a1))
      {
        vol = (((_svm_voice[i].unk08 * a3) / 127) * (new_var * 0x3FFF)) / 16129;
        if ((rec->h60[rec->b17] != a3) && (rec->h60[rec->b17] == 0))
        {
          rec->h60[rec->b17] = 1;
        }
        l = (((_svm_voice[i].unk08 * a3) / 127) * (k = _svm_vh[0x18] * 0x3FFF)) / 16129;
        toneIndex = _svm_voice[i].unk12 * 16 + _svm_voice[i].unk16;
        vol = ((l * _svm_pg[a2][VAB_PROGRAM_VOLUME]) * _svm_tn[toneIndex][VAB_TONE_VOLUME]) / 16129;
        l = (vol * rec->h58[0]) / 127;
        r = (vol * rec->h58[1]) / 127;
        pan = _svm_tn[toneIndex][VAB_TONE_PAN];
        if (pan < 0x40)
        {
          right = (r * pan) / 63;
          left = l;
        }
        else
        {
          left = (l * (0x7F - pan)) / 63;
          right = r;
        }
        pan = _svm_pg[_svm_voice[i].match14][VAB_PROGRAM_PAN];
        if (pan < 0x40)
        {
          right = (right * pan) / 63;
        }
        else
        {
          left = (left * (0x7F - pan)) / 63;
        }
        pan = a4;
        if (pan < 0x40)
        {
          right = (right * pan) / 63;
        }
        else
        {
          left = (left * (0x7F - pan)) / 63;
        }
        if (_svm_stereo_mono == 1)
        {
          u32 dummy_hi;
          if (left < right)
          {
            left = right;
          }
          else
          {
            right = left;
          }
        }
        left = (left * left) / 16383;
        right = (right * right) / 16383;
        *((s16 *) (&_svm_sreg_buf[i].left)) = left;
        *((s16 *) (&_svm_sreg_buf[i].right)) = right;
        count++;
        _svm_sreg_dirty.flags[i] |= 3;
      }
    }
    i++;
  } while (i < spuVmMaxVoice);

  return count;
}
