/* GCC_VERSION: 2.7.2 */
extern int CardRxCount, D_80114730;
int _cardRxStatusByte(a0) int a0; { if (CardRxCount == 3) D_80114730 = a0 & 0xFF; return 0; }
