#ifndef REG_H_
#define REG_H_

#include <systemc.h>

/**
 * A 32-bit register.
 */

SC_MODULE(reg)
{
  sc_in<sc_uint<32>> in;
  sc_in<bool> reset;
  sc_in<bool> clk;
  // sc_in<bool> enable;
  sc_out<sc_uint<32>> out;

  sc_uint<32> val;

  SC_CTOR(reg)
  {
    SC_METHOD(entry);
    sensitive << reset << clk.neg();
    val = 0;
  }

  void entry();
};

#endif