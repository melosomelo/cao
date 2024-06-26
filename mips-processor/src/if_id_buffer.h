#ifndef IF_ID_BUFFER_H_
#define IF_ID_BUFFER_H_

#include <systemc.h>
#include "reg.h"

SC_MODULE(IF_ID_buffer)
{
  sc_in<bool> clk;
  sc_in<sc_uint<32>> instruction_in, pc4_in;
  sc_out<sc_uint<32>> instruction_out, pc4_out;

  reg *instruction_reg, *pc4_reg;

  SC_CTOR(IF_ID_buffer)
  {
    instruction_reg = new reg("instruction_reg");
    instruction_reg->in(instruction_in);
    instruction_reg->out(instruction_out);
    instruction_reg->clk(clk);

    pc4_reg = new reg("pc4_reg");
    pc4_reg->in(pc4_in);
    pc4_reg->out(pc4_out);
    pc4_reg->clk(clk);
  }
};

#endif