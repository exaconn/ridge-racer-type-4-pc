/* GCC_VERSION: 2.7.2 */
extern int CardRxCount, CardCmdCount, CardBytesRemaining;
int _cardCmd5CRangeRx() { if (CardRxCount == 9) CardBytesRemaining = CardCmdCount + 1; return 0; }
