/*
 * - testing the "call" and "call_value" patterns and the corresponding
 *   "*..._internal" `define_insn's
 */

int
times2 (int b)
{
  return b + b;
}

void
nop ()
{
  return;
}

int
main ()
{
  int a = -3;
  a = times2 (a);
  nop();
  return a;
}
