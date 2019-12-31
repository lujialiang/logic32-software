/*
 * - testing HImode and QImode
 */

int
main ()
{
  unsigned int siA __attribute__((mode (SI)));
  unsigned int siB __attribute__((mode (SI)));

  unsigned int hiA __attribute__((mode (HI)));
  unsigned int hiB __attribute__((mode (HI)));

  unsigned int qiA __attribute__((mode (QI)));
  unsigned int qiB __attribute__((mode (QI)));

  int dummy;

  siA = 0x10000;    /* constant needs > 16bit */
  /* 65535 is wrapped around to -1 by `immed_double_const' */
  hiA = 65535;
  hiB = 7;
  qiA = 3;
  qiB = 5;

  dummy = 1234;

  return 0;
}
