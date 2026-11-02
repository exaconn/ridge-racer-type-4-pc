
extern int D_800ACC14;
void *func_8004C998(char *a0)
{
  unsigned short *new_var;
  char *new_var2;
  int new_var3;
  char *p = *((char **) 0x1F800000);
  unsigned short a1;
  int v;
  char *d;
  int dd;
  *((unsigned char *) (((char *) p) + 3)) = 9;
  *((unsigned char *) (((char *) p) + 7)) = 0x2C;
  v = *((unsigned int *) (((char *) a0) + 8));
  if (v == 0)
  {
    v = 0x2C;
  }
  else
  {
    v = 0x2E;
  }
  *((unsigned char *) (((char *) p) + 7)) = v;
  if ((*((unsigned int *) (((char *) a0) + 4))) != 0)
  {
    v = (*((volatile unsigned char *) (((char *) p) + 7))) | 1;
  }
  else
  {
    v = (*((volatile unsigned char *) (((char *) p) + 7))) & 0xFE;
  }
  *((unsigned char *) (((char *) p) + 7)) = v;
  *((unsigned char *) (((char *) p) + 4)) = *((unsigned char *) (((char *) a0) + 0x10));
  *((unsigned char *) (((char *) p) + 5)) = *((unsigned char *) (((char *) a0) + 0x11));
  *((unsigned char *) (((char *) p) + 6)) = *((unsigned char *) (((char *) a0) + 0x12));
  *((unsigned short *) (((char *) p) + 8)) = *((unsigned short *) (((char *) a0) + 0x14));
  new_var2 = (char *) a0;
  new_var3 = 1;
  *((unsigned short *) (((char *) p) + 0xA)) = *((unsigned short *) (((char *) a0) + 0x16));
  *((unsigned short *) (((char *) p) + 0x10)) = ((*((unsigned short *) (new_var2 + 0x14))) + new_var3) + (*((unsigned short *) (((char *) a0) + 0x18)));
  *((unsigned short *) (((char *) p) + 0x12)) = *((unsigned short *) (((char *) a0) + 0x16));
  *((unsigned short *) (((char *) p) + 0x18)) = *((unsigned short *) (((char *) a0) + 0x14));
  a1 = 1;
  *((unsigned short *) (((char *) p) + 0x1A)) = ((*((unsigned short *) (((char *) a0) + 0x16))) + a1) + (*((unsigned short *) (((char *) a0) + 0x1A)));
  *((unsigned short *) (((char *) p) + 0x20)) = (((short) (*((unsigned short *) (((char *) a0) + 0x14)))) + 1) + (*((unsigned short *) (((char *) a0) + 0x18)));
  new_var = &(*((unsigned short *) (((char *) a0) + 0x16)));
  *((unsigned short *) (((char *) p) + 0x22)) = ((*new_var) + a1) + (*((unsigned short *) (((char *) a0) + 0x1A)));
  *((unsigned char *) (((char *) p) + 0xC)) = *((unsigned char *) (((char *) a0) + 0x1C));
  *((unsigned char *) (((char *) p) + 0xD)) = *((unsigned char *) (((char *) a0) + 0x1E));
  *((unsigned char *) (((char *) p) + 0x14)) = (*((unsigned char *) (((char *) a0) + 0x1C))) + (*((unsigned char *) (((char *) a0) + 0x20)));
  *((unsigned char *) (((char *) p) + 0x15)) = *((unsigned char *) (((char *) a0) + 0x1E));
  *((unsigned char *) (((char *) p) + 0x1C)) = *((unsigned char *) (((char *) a0) + 0x1C));
  *((unsigned char *) (((char *) p) + 0x1D)) = (*((unsigned char *) (((char *) a0) + 0x1E))) + (*((unsigned char *) (((char *) a0) + 0x22)));
  *((unsigned char *) (((char *) p) + 0x24)) = (*((unsigned char *) (((char *) a0) + 0x1C))) + (*((unsigned char *) (((char *) a0) + 0x20)));
  *((unsigned char *) (((char *) p) + 0x25)) = (*((unsigned char *) (((char *) a0) + 0x1E))) + (*((unsigned char *) (((char *) a0) + 0x22)));
  a1 = *((unsigned short *) (((char *) a0) + 0x26));
  *((unsigned short *) (((char *) p) + 0x16)) = (((((*((unsigned short *) (((char *) a0) + 0xC))) & 3) << 7) | (((a1 & 0x100) << 16) >> 20)) | (((*((unsigned short *) (((char *) a0) + 0x24))) & 0x3FF) >> 6)) | ((a1 & 0x200) * 4);
  *((unsigned short *) (((char *) p) + 0xE)) = ((*((unsigned short *) (((char *) a0) + 0x2A))) << 6) | (((*((unsigned short *) (((char *) a0) + 0x28))) >> 4) & 0x3F);
  dd = D_800ACC14;
  *((unsigned int *) (((char *) p) + 0)) = ((*((unsigned int *) (((char *) p) + 0))) & 0xFF000000) | ((*((unsigned int *) (((char *) ((char *) (((*((unsigned int *) (((char *) a0) + 0))) * 4) + dd))) + 0x70))) & 0xFFFFFF);
  d = (char *) (((*((unsigned int *) (((char *) a0) + 0))) * 4) + dd);
  *((unsigned int *) (((char *) d) + 0x70)) = ((*((unsigned int *) (((char *) d) + 0x70))) & 0xFF000000) | (((unsigned int) p) & 0xFFFFFF);
  p += 0x28;
  *((char **) 0x1F800000) = p;
  return p;
}
