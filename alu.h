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

SC_MODULE(ALU)
{
  sc_in<sc_uint<32>> a;
  sc_in<sc_uint<32>> b;
  sc_in<bool> cin;
  sc_in<bool> b_invert;
  sc_in<sc_uint<2>> op;
  sc_out<sc_uint<32>> result;
  sc_out<bool> cout;

  void compute();

  SC_CTOR(ALU)
  {
    SC_METHOD(compute);
    sensitive << a << b << op << cin << b_invert;
  }
};

#endif