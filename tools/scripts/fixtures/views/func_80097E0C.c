/* CC1_FLAGS: -mno-split-addresses */

extern int _padIntPad(), _padIsVsync();

extern int D_8011441C[];

void _padSetVsyncParam() {
    int *p = D_8011441C;
    p[0] = (int)_padIntPad;
    p[1] = (int)_padIsVsync;
    p[-1] = 0;
    p[2] = 0;
}
