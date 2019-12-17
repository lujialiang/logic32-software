/*
 * - testing unconditional jumps ("jump" standard pattern)
 */

int
main ()
{
  int a = 1;

loop:
  a += 1;
  goto loop;

  return 0;
}
