/*
 * - testing comparisons and conditional jumps
 *     - signed as well as unsigned
 *     - only SImode 
 */

/* signed */
int
gt (int a, int b)
{
  return a > b;
}

int
lt (int a, int b)
{
  return a < b;
}

int
ge (int a, int b)
{
  return a >= b;
}

int
le (int a, int b)
{
  return a <= b;
}

int
eq (int a, int b)
{
  return a == b;
}

int
ne (int a, int b)
{
  return a != b;
}

/* unsigned */
int
gt_u (unsigned a, unsigned b)
{
  return a > b;
}

int
lt_u (unsigned a, unsigned b)
{
  return a < b;
}

int
ge_u (unsigned a, unsigned b)
{
  return a >= b;
}

int
le_u (unsigned a, unsigned b)
{
  return a <= b;
}

int
eq_u (unsigned a, unsigned b)
{
  return a == b;
}

int
ne_u (unsigned a, unsigned b)
{
  return a != b;
}


/**** main ****/
int
main ()
{
  return 0;
}
