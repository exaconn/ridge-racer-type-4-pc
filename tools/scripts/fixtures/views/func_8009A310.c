/* GCC_VERSION: 2.7.2 */

extern char PadChannelRecordArray[];

char *_padPortToChannel(a0) int a0; {
    char *p = PadChannelRecordArray;
    if (a0 & 0xF0) p += 0xF0;
    return p;
}
