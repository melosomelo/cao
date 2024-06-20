#ifndef DMEM_H_
#define DMEM_H_

#include <systemc.h>
#include "mem.h"

class dmem : public sc_module, public mem
{
public:
  sc_in<sc_uint<32>> addr;
  sc_in<sc_uint<32>> data_in;
  sc_in<bool> MemWrite;
  sc_in<bool> MemRead;
  sc_in<bool> clk;
  sc_out<sc_uint<32>> data_out;

  SC_CTOR(dmem)
  {
    SC_METHOD(read);
    sensitive << addr << MemRead;
    SC_METHOD(write);
    sensitive << clk.pos();
    // init_memory("datamem.dat");
  }

  void read();
  void write();
};

#endif