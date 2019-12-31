/*
 * - testing the `prologue' and `epilogue' patterns
 * - this test constrains itself to passing values in SImode, only
 */

int
add (int a, int b)
{
  return a + b;
}


int
main ()
{
  int a, b;
  a = 100;
  b = 100000;

  return add (a, b);
}
