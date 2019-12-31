/*
 * - testing parameter passing with modes other than SI
 */

void
testSI (int i)
{
  return;
}

void
testHI (int i __attribute__((mode (HI))))
{
  return;
}

int 
main ()
{
  int siA __attribute__((mode (SI)));
  int hiA __attribute__((mode (HI)));
  int qiA __attribute__((mode (QI)));

  hiA = 7;
  testSI (hiA);

  qiA = 5;
  testSI (qiA);

  siA = 9;
  testHI (siA);

  return 0;
}

