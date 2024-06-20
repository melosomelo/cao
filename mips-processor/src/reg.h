#ifndef REG_H_
#define REG_H_

#include <systemc.h>
enum reg_num

{
  $zero = 0,
  $at = 1,
  $v0 = 2,
  $v1 = 3,
  $a0 = 4,
  $a1 = 5,
  $a2 = 6,
  $a3 = 7,
  $t0 = 8,
  $t1 = 9,
  $t2 = 10,
  $t3 = 11,
  $t4 = 12,
  $t5 = 13,
  $t6 = 14,
  $t7 = 15,
  $s0 = 16,
  $s1 = 17,
  $s2 = 18
  // ...
};

/**
 * A 32-bit register.
 */

SC_MODULE(reg)
{
  sc_in<sc_uint<32>> in;
  sc_in<bool> clk;
  // sc_in<bool> reset;
  // sc_in<bool> enable;
  sc_out<sc_uint<32>> out;

  sc_uint<32> val;

  SC_CTOR(reg)
  {
    SC_METHOD(entry);
    sensitive << clk.pos();
    val = 0;
  }

  void entry();
};

#endif