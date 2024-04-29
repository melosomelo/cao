#ifndef ID_EX_BUFFER_H_
#define ID_EX_BUFFER_H_

#include <systemc.h>
#include "reg.h"

SC_MODULE(ID_EX_buffer)
{
  sc_in<bool> clk;
  sc_in<sc_uint<32>> pc4_in, reg1_in, reg2_in, extended_offset_in, jumpaddr32_in;
  sc_out<sc_uint<32>> pc4_out, reg1_out, reg2_out, extended_offset_out, jumpaddr32_out;

  reg *pc4, *reg1, *reg2, *extended_offset;

  SC_CTOR(ID_EX_buffer)
  {
    pc4 = new reg("id_ex_pc4");
    pc4->in(pc4_in);
    pc4->out(pc4_out);
    pc4->clk(clk);

    reg1 = new reg("id_ex_reg1");
    reg1->in(reg1_in);
    reg1->out(reg1_out);
    reg1->clk(clk);

    reg2 = new reg("id_ex_reg2");
    reg2->in(reg2_in);
    reg2->out(reg2_out);
    reg2->clk(clk);

    extended_offset = new reg("id_ex_extended_offset");
    extended_offset->in(extended_offset_in);
    extended_offset->out(extended_offset_out);
    extended_offset->clk(clk);
  }
};

#endif