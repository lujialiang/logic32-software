/*
 * - testing the bit shifting patterns `ashlM3', `ashrM3', and `lshrM3'
 */

int
main ()
{
  /* make vars `a' and `b' 32 bits wide */
  int a           __attribute__((mode (SI)));
  unsigned int b  __attribute__((mode (SI)));

  a = 1;              /* set bit 0 */
  a <<= 31;           /* shift bit 0 to msb, assuming int is 32 bits */
  a >>= 30;           /* shift in `1's from left */

  b = 1;
  b <<= 31;           /* shift bit 0 to msb, assuming int is 32 bits */
  b >>= 30;           /* shift in `0's from left */

  return 0;
}
