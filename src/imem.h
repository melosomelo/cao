#ifndef IMEM_H_
#define IMEM_H_

#include <systemc.h>
#include "mem.h"

/**
 * Instruction memory module.
 * A simple SystemC module that builds on top of the base mem class.
 * Just to interface with SystemC signals and stuff.
 */
class imem : public sc_module, public mem
{

public:
  sc_in<sc_uint<32>> addr;
  sc_out<sc_uint<32>> inst;

  SC_CTOR(imem)
  {
    SC_METHOD(entry);
    sensitive << addr;
    // init_memory("instmem.hex");
  }

  void entry();
};

#endif