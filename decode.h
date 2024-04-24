#ifndef DECODE_H_
#define DECODE_H_

#include <systemc.h>

/**
 * A module responsible for splitting the output of the memory instruction module
 * into its counterparts. I did not find a way to split the signal individually,
 * so I did this.
 */
SC_MODULE(decode)
{
  // The instruction comming out of the instruction memory.
  sc_in<sc_uint<32>> instruction;
  // The operation code. Serves as input for the control unit.
  sc_out<sc_uint<6>> opcode;
  // Number of the first register operand. Goes into the register file.
  sc_out<sc_uint<5>> rs;
  // Number of the second register operand. Goes into the register file.
  sc_out<sc_uint<5>> rt;
  // Number of the destination register. Goes into the register file as well.
  sc_out<sc_uint<5>> rd;
  // Shift amount.
  sc_out<sc_uint<5>> shamt;
  // Species the specific operation within the opcode category.
  sc_out<sc_uint<6>> funct;
  // 16-bit offset for load/store instructions.
  sc_out<sc_uint<16>> offset;

  SC_CTOR(decode)
  {
    SC_METHOD(entry);
    sensitive << instruction;
  }

  void entry();
};

#endif