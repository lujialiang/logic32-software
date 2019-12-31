/*
 * - testing `while' and `for' loops
 */

int 
main ()
{
  int i;
  int a;

  for (i = 5; i < 10; i += 2) 
  {
    a = i;
  }

  i = 5;
  while (i < 10)
  {
    a = i;
    i += 2;
  }

  return 0;
}

