#ifndef PROC_H_
#define PROC_H_

#include <systemc.h>
#include "reg.h"
#include "imem.h"

SC_MODULE(proc)
{
  sc_in<bool> clk;
  sc_in<bool> reset;

  // The program counter. Holds the value of the current instruction.
  reg *pc_reg;
  // Instruction memory.
  imem *imem;

  SC_CTOR(proc)
  {
    init();
  };

  /** Initializes the components of the architecture */
  void init();
};

#endif