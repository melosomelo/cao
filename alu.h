#ifndef ALU_H
#define ALU_H

#include <systemc.h>

enum ALUOperation
{
  AND = 0,
  OR,
  XOR,
  ADD
};

/**
 * 32-bit ALU module that can perform and, or, xor, not, add, subtract and compare.
 * Highly inspired by the ALU constructed in Annex B of
 * Computer Organization and Design MIPS Edition: The Hardware/Software Interface
 * by David Patterson and John Hennessy.
 *
 * Input ports:
 *   - a: first operand
 *   - b: second operand
 *   - cin: carry in input signal. Used in conjunction with b_invert to execute subtraction.
 *   - b_invert: inverts the b operand. Can be used to calculate logical not (via 0 + not(b)) or
 *     in conjuction with cin to calculate a - b (since in two's complement a - b = a + (not(b) + 1))
 *   - op: operation control signal.
 * Output ports:
 *   - result: the result of the calculation of the chosen operation.
 *   - zero: signals if result equals to 0. used for conditional branching.
 *   - less: outputs if a < b.
 */

SC_MODULE(ALU)
{
  sc_in<sc_uint<32>> a;
  sc_in<sc_uint<32>> b;
  sc_in<bool> cin;
  sc_in<bool> b_invert;
  sc_in<sc_uint<2>> op;
  sc_out<sc_uint<32>> result;
  sc_out<bool> zero;
  sc_out<bool> less;

  void compute();

  SC_CTOR(ALU)
  {
    SC_METHOD(compute);
    sensitive << a << b << op << cin << b_invert;
  }
};

#endif