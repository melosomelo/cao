#ifndef CONTROL_H_
#define CONTROL_H_

#include <systemc.h>
#include "alu.h"

/**
 * Control module that sets the value of control lines based on
 * some fields from the instruction
 */
SC_MODULE(control)
{
  // Instruction fields
  sc_in<sc_uint<6>> opcode;
  sc_in<sc_uint<6>> funct;

  // Control outputs

  // Determines which register number will be fed into
  // the write_reg input port of the register file.
  // Chooses between the rt instruction field and rd intrusction field
  sc_out<bool> RegDst;
  // Signals that instruction is a branch. Used in conjutction with the
  // zero output from the main ALU to determine if the branch should be taken or not.
  sc_out<bool> Branch;
  // Signals that instruction is a jump.
  sc_out<bool> Jump;
  // Enables writing of the register file.
  sc_out<bool> RegWrite;
  // Enables writing of the data memory.
  sc_out<bool> MemWrite;
  // Enables reading of the data memory.
  sc_out<bool> MemRead;
  // Selector signal for the multiplexor that chooses
  // between the second output from the register file
  // and sign-extended 16-bit offset for the
  // second parameter of the main ALU.
  sc_out<bool> ALUSrc;
  // Selector signal for the multiplexor that chooses
  // between the value read from the data memory and
  // main ALU result to be written into the register file.
  sc_out<bool> MemtoReg;
  // Sets the operation of the main ALU
  sc_out<sc_uint<3>> ALUOp;

  SC_CTOR(control)
  {
    SC_METHOD(entry);
    sensitive << opcode << funct;
  }

  void entry();
};

#endif