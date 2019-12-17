/*
 * - testing `switch'/`case' statements
 *     - note that GCC uses `casesi' or `jumptable' not until the number of
 *       case labels in the switch statement is at least `CASE_VALUES_THRESHOLD'
 */

int 
main ()
{
  int a = 2;
  int b;

  switch (a)
  {
  case 1:
    b = 1;
    break;
  case 2:
    b = 2;
    break;
  case 3:
    b = 3;
    break;
  case 4:
    b = 4;
    break;
  case 5:
    b = 5;
    break;
  case 6:
    b = 6;
    break;
  default:
    b = -1;
    break;
  }

  return 0;
}

