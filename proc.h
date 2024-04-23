#ifndef PROC_H_
#define PROC_H_

#include <systemc.h>
#include "reg.h"
#include "imem.h"
#include "alu.h"

SC_MODULE(proc)
{
  sc_in<bool> clk;
  sc_in<bool> reset;

  // The program counter register. Holds the value of the current instruction.
  reg *pc_reg;
  // Instruction memory.
  imem *inst_mem;
  // An ALU used exclusively as an adder. Calculates the address of the
  // next sequential instruction (PC + 4).
  alu *add4;

  // Signals.
  // These are the physical wires that connect different modules together
  // The instruction to be fetched from the instruction memory
  sc_signal<sc_uint<32>> pc;
  // The value of the calculation of pc + 4 (address of the next sequential instruction)
  sc_signal<sc_uint<32>> pc4;
  // A constant signal with value of 4 that is used as the second parameter of the add4 module
  sc_signal<sc_uint<32>> const4{"const4", 4};
  // A constant signal that sets the add4 alu to be always used as an adder
  sc_signal<sc_uint<3>> add4_op{"add4_op", alu_op::add_op};
  // The signal that comes out of the instruction memory
  sc_signal<sc_uint<32>> current_inst;
  // A dummy signal for the zero output of the add4 alu. Not used but needs to be bound
  // otherwise SystemC complains.
  sc_signal<bool> dummy_zero_add4;

  SC_CTOR(proc)
  {
    SC_METHOD(print_state);
    sensitive << clk.pos();
    init();
  };

  /** Initializes the components of the architecture */
  void init();

  // Debug method.
  void print_state();
};

#endif