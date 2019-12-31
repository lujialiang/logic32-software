/*
register int a0 asm ("a0");
register int a1 asm ("a1");
register int a2 asm ("a2");
register int a3 asm ("a3");
register int a4 asm ("a4");
register int a5 asm ("a5");
register int a6 asm ("a6");
register int a7 asm ("a7");
register int a8 asm ("a8");
register int a9 asm ("a9");
register int a10 asm ("a10");
register int a11 asm ("a11");
register int a12 asm ("a12");
register int a13 asm ("a13");
register int a14 asm ("a14");
register int a15 asm ("a15");
register int a16 asm ("a16");
register int a17 asm ("a17");
register int a18 asm ("a18");
register int a19 asm ("a19");
register int a20 asm ("a20");
register int a21 asm ("a21");
register int a22 asm ("a22");
register int a23 asm ("a23");
register int a24 asm ("a24");
register int a25 asm ("a25");
register int a26 asm ("a26");
register int a27 asm ("a27");
register int a28 asm ("a28");
register int a29 asm ("a29");
register int a30 asm ("a30");
*/

int main() {
  int a, b, c, d, e, f, g, h;
  a = 1;
  b = 100000;
  c = 3;
  d = 4;
  e = 5;
  f = 6;
  g = 7;

  h = ((a+b) << 2 - c ) * d;

  return 0;
}
