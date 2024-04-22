#ifndef ALU_H
#define ALU_H

#include <systemc.h>

enum ALUOperation
{
  AND = 0,
  OR,
  XOR,
  NOT,
  ADD,
  SUB,
  LESS
};

/**
 * 32-bit ALU module that can perform and, or, xor, not, add, subtract and compare (less and equal).
 * Highly inspired by the ALU constructed in Annex B of
 * Computer Organization and Design MIPS Edition: The Hardware/Software Interface
 * by David Patterson and John Hennessy.
 *
 * Input ports:
 *   - a: first operand
 *   - b: second operand
 *   - op: operation control signal.
 * Output ports:
 *   - result: the result of the calculation of the chosen operation.
 *   - zero: signals if result equals to 0. used for conditional branching.
 */

SC_MODULE(ALU)
{
  sc_in<sc_uint<32>> a;
  sc_in<sc_uint<32>> b;
  sc_in<sc_uint<2>> op;
  sc_out<sc_uint<32>> result;
  sc_out<bool> zero;

  void compute();

  SC_CTOR(ALU)
  {
    SC_METHOD(compute);
    sensitive << a << b << op;
  }
};

#endif