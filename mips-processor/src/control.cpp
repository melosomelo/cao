#include "control.h"

void control::entry()
{
  // NOP. Do nothing. Reset all control lines.
  if (opcode.read() == 0 && funct.read() == 0)
  {
    RegDst.write(0);
    Branch.write(0);
    Jump.write(0);
    RegWrite.write(0);
    MemWrite.write(0);
    MemRead.write(0);
    ALUSrc.write(0);
    MemtoReg.write(0);
    return;
  }
  switch (opcode.read())
  {
  case 0:              // R-format instruction
    RegDst.write(1);   // `write_reg` from regfile will receive the rd instruction field.
    ALUSrc.write(0);   // second output from the register file goes into the main ALU.
    MemtoReg.write(0); // output from the main ALU will go into the register file.
    RegWrite.write(1); // enables writing to the register file
    // Will not touch data memory
    MemRead.write(0);
    MemWrite.write(0);
    Branch.write(0); // clearly not a branch
    switch (funct.read())
    {
    case 32:
      ALUOp.write(alu_op::add_op);
      break;
    case 34:
      ALUOp.write(alu_op::sub_op);
      break;
    case 36:
      ALUOp.write(alu_op::and_op);
      break;
    case 37:
      ALUOp.write(alu_op::or_op);
      break;
    case 38:
      ALUOp.write(alu_op::xor_op);
      break;
    case 39:
      ALUOp.write(alu_op::nor_op);
      break;
    case 42:
      ALUOp.write(alu_op::less_op);
      break;
    }
    break;
  case 2: // jump
    ALUSrc.write(0);
    RegWrite.write(0);
    MemRead.write(0);
    MemWrite.write(0);
    ALUOp.write(alu_op::sub_op);
    Jump.write(1);
    break;
  case 4: // beq (branch on equal)
    ALUSrc.write(0);
    RegWrite.write(0);
    MemRead.write(0);
    MemWrite.write(0);
    Branch.write(1); // clearly a branch
    ALUOp.write(alu_op::sub_op);
    break;
  case 35:             // lw (load word)
    RegDst.write(0);   // `write_reg` from regfile will receive the rt instruction field.
    ALUSrc.write(1);   // 32-bit extend 16-bit offset serves as second output from the main ALU
    MemtoReg.write(1); // Output from the data memory will go into the register file
    RegWrite.write(1); // enables writing to the register file
    MemRead.write(1);  // since it's a load, we read from data memory
    MemWrite.write(0); // since it's a load, we do not write to the data memory
    Branch.write(0);   // clearly not a branch
    ALUOp.write(alu_op::add_op);
    break;
  case 43: // sw (store word)
    ALUSrc.write(1);
    RegWrite.write(0); // disables writing to the register file
    MemRead.write(0);  // since it's a store, we do not read from memory
    MemWrite.write(1); // since it's a store, we write to memory
    Branch.write(0);   // clearly not a branch
    ALUOp.write(alu_op::add_op);
    break;
  }
}