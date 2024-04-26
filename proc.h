#ifndef PROC_H_
#define PROC_H_

#include <systemc.h>
#include "reg.h"
#include "dmem.h"
#include "imem.h"
#include "alu.h"
#include "regfile.h"
#include "decode.h"
#include "extend.h"
#include "shiftl2.h"
#include "mux2.h"
#include "control.h"
#include "andgate.h"
#include "jcalc.h"

SC_MODULE(proc)
{
  sc_in<bool> clk;
  sc_in<bool> reset;

  // The processor modules.

  // The program counter register. Holds the value of the current instruction.
  reg *pc_reg;
  // Instruction memory.
  imem *inst_mem;
  // An ALU used exclusively as an adder. Calculates the address of the
  // next sequential instruction (PC + 4).
  alu *pc_add4;
  // Decoder module. Responsible for splitting up the instruction retrieved
  // from the instruction memory and pass it along to other modules.
  decode *dcode;
  // The register file. Holds 32 32-bit registers.
  regfile *rfile;
  // Data memory.
  dmem *datamem;
  // The main ALU of the processor. Performs the arithmetic
  // and logical operations of the instruction set.
  alu *main_alu;
  // ALU used exclusively as an adder to calcute branch addresses.
  alu *branch_alu;
  // 32 bit sign-extender. Extends the 16-bit branch offset from
  // load, stores and conditional branches into a 32-bit signal.
  extend *extend32;
  shiftl2 *sl2;
  // Multiplexor that chooses between the next sequenital instruciton
  // and the instruction specified by the beq instruction.
  mux2<sc_uint<32>> *branch_alu_pc4_mux;
  // Multiplexor that chooses the value of the second input parameter
  // Chooses between the 2nd output of the register file and extended_offset
  // signal (for arithmetic and load/store instructions, respectively).
  mux2<sc_uint<32>> *main_alu_b_mux;
  // Multiplexor that chooses the value that's going to be written to
  // the register file. Chooses between the value read from the
  // data memory and the main alu result (for arithmetic operations).
  mux2<sc_uint<32>> *rfile_data_in_mux;
  // Multiplexor that chooses the value that specifies the number
  // of the register that will be written to. Chooses between the rt and rd
  // instruction fields, for R-format and lw instructions respectively.
  mux2<sc_uint<5>> *write_reg_mux;
  // Control unit. Responsible for setting the control lines that control the individual components
  // based on the chosen instruction.
  control *ctrl;
  // AND gate that outputs the result of the AND operation between the Branch control signal
  // and the zero output of the main ALU. Used to determine if the branch should be taken.
  andgate *Branch_and_main_alu_zero;
  // Signals.
  // These are the physical wires that connect different modules together

  // The instruction to be fetched from the instruction memory
  sc_signal<sc_uint<32>> pc;
  // The value of the calculation of pc + 4 (address of the next sequential instruction)
  sc_signal<sc_uint<32>> pc4;
  // A constant signal with value of 4 that is used as the second parameter of the pc_add4 module
  sc_signal<sc_uint<32>> const4{"const4", 4};
  // A constant signal that makes some ALUs be used exclusively as adders.
  sc_signal<sc_uint<3>> const_add_op{"add_op_sig", alu_op::add_op};
  // The signal that comes out of the instruction memory
  sc_signal<sc_uint<32>> current_inst;
  // Dummy signals to capture zero output of ALUs used exclusively as adders.
  // Not used but needs to be bound otherwise SystemC complains.
  sc_signal<bool> dummy_pc4_zero;
  sc_signal<bool> dummy_branch_alu_zero;
  // Signals that come out of the register file;
  sc_signal<sc_uint<32>> rfile_out1, rfile_out2;
  // Signal that holds the value to be written to the register file
  sc_signal<sc_uint<32>> rfile_data_in;
  // 32-bit sign-extended instruction offset field
  sc_signal<sc_uint<32>> extended_offset;
  // 2-bit shifted left version of the extend_offset signal.
  sc_signal<sc_uint<32>> sl2_extended_offset;
  // Result output of the branch alu.
  sc_signal<sc_uint<32>> branch_alu_result;
  sc_signal<sc_uint<32>> branch_alu_pc4_mux_out;
  // Signal coming out of main_alu_b_mux module
  sc_signal<sc_uint<32>> main_alu_b_mux_out;
  // Signal coming out of the main ALU
  sc_signal<sc_uint<32>> main_alu_result;
  // Signal coming out of data memory
  sc_signal<sc_uint<32>> dmem_out;
  sc_signal<sc_uint<5>> write_reg_mux_out;
  // Signal that determines if the branch should be taken.
  sc_signal<bool> take_branch;
  sc_signal<bool> main_alu_zero;
  sc_signal<sc_uint<32>> jumpaddr32;
  // Instruction field signals. They're explained in the decode.h file.
  sc_signal<sc_uint<26>> jumpaddr26;
  sc_signal<sc_uint<16>> offset;
  sc_signal<sc_uint<6>> opcode, funct;
  sc_signal<sc_uint<5>> rs, rt, rd, shamt;
  // Control signals. They're explained in more detail in the control.h file.
  sc_signal<bool> RegDst, Branch, RegWrite, MemWrite, MemRead, ALUSrc, MemtoReg, Jump;
  sc_signal<sc_uint<3>> ALUOp;

  SC_CTOR(proc)
  {
    init();
  };

  ~proc();

  /** Initializes the components of the architecture */
  void init();

  void load_instruction_memory(std::vector<sc_uint<32>>);
  void load_data_memory(std::vector<sc_uint<32>>);
  void dump_state();
};

#endif