extern struct { char pad[0xC]; unsigned char a; unsigned char b; short c; } MenuFrameState;
void SetMenuFrameOptions(a, b, c) short a; int b; int c; { MenuFrameState.c = a; MenuFrameState.a = b; MenuFrameState.b = c; }
