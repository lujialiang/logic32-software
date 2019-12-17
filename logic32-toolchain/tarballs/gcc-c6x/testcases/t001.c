/*
 * - testing GCC's capabilities to handle access to local variables
 * - requires: 
 *     - `case MEM:' in `c6x_print_operand' for storing to a memory location
 *       given by a register
 *     - the "addsi3" pattern for computing FP + var slot offset
 *     - the "movsi" pattern (alternative "i"->"r") for loading the constant 7,
 *       the var slot offset and the return value 3
 */

int
main()
{
  int a=7;
  return 3;
}
