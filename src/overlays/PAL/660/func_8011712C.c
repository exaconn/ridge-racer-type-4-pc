#include "rr/menu_screen_exit.h"
void ovl660_func_8011712C(void)
{
    func_80036068();
    func_8003CAB4();
    D_800F4C6A = 1;
    D_800F3A26 = 53;
    D_800F4D8A = 0;
    func_800207D4(((RrFrameOrderingPrefix *)D_800ACC14)->ordering70, 255, 69);
}
