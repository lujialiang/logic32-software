/*
 * - testing the "movhi" pattern
 * - data in comments was drawn from `cc1 -dP' (see `decode_d_option')
 */

int
main ()
{
  short int a = -3;   /* `movhi/4', which is `movhi: immediate->reg' */
  short int b = a;    /* `movhi/2 for loading `a' from stack,
                         `movhi/3 for storing it into `b' on stack  */
  b += 1;             /* uses addsi3 rather than addhi3, see `widen_operand' */
  b = b + b;          /* dito */
  return 0;
}
