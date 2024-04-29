#ifndef ID_EX_BUFFER_H_
#define ID_EX_BUFFER_H_

#include <systemc.h>
#include "reg.h"
#include "regT.h"

SC_MODULE(ID_EX_buffer)
{
  sc_in<bool> clk;
  sc_in<bool> RegDst_in, Branch_in, Jump_in, RegWrite_in, MemWrite_in, MemRead_in, ALUSrc_in, MemToReg_in;
  sc_in<sc_uint<3>> ALUOp_in;
  sc_in<sc_uint<32>> pc4_in, reg1_in, reg2_in, extended_offset_in, jumpaddr32_in;
  sc_out<sc_uint<32>> pc4_out, reg1_out, reg2_out, extended_offset_out, jumpaddr32_out;
  sc_out<bool> RegDst_out, Branch_out, Jump_out, RegWrite_out, MemWrite_out, MemRead_out, ALUSrc_out, MemToReg_out;
  sc_out<sc_uint<3>> ALUOp_out;

  reg *pc4, *reg1, *reg2, *extended_offset;
  regT<bool> *RegDst, *Branch, *Jump, *RegWrite, *MemWrite, *MemRead, *ALUSrc, *MemToReg;
  regT<sc_uint<3>> *ALUOp;

  SC_CTOR(ID_EX_buffer)
  {
    ALUOp = new regT<sc_uint<3>>("id_ex_ALUOp");
    ALUOp->clk(clk);
    ALUOp->din(ALUOp_in);
    ALUOp->dout(ALUOp_out);

    RegDst = new regT<bool>("id_ex_RegDst");
    RegDst->clk(clk);
    RegDst->din(RegDst_in);
    RegDst->dout(RegDst_out);

    Branch = new regT<bool>("id_ex_Branch");
    Branch->clk(clk);
    Branch->din(Branch_in);
    Branch->dout(Branch_out);

    Jump = new regT<bool>("id_ex_Jump");
    Jump->clk(clk);
    Jump->din(Jump_in);
    Jump->dout(Jump_out);

    RegWrite = new regT<bool>("id_ex_RegWrite");
    RegWrite->clk(clk);
    RegWrite->din(RegWrite_in);
    RegWrite->dout(RegWrite_out);

    MemWrite = new regT<bool>("id_ex_MemWrite");
    MemWrite->clk(clk);
    MemWrite->din(MemWrite_in);
    MemWrite->dout(MemWrite_out);

    MemRead = new regT<bool>("id_ex_MemRead");
    MemRead->clk(clk);
    MemRead->din(MemRead_in);
    MemRead->dout(MemRead_out);

    ALUSrc = new regT<bool>("id_ex_ALUSrc");
    ALUSrc->clk(clk);
    ALUSrc->din(ALUSrc_in);
    ALUSrc->dout(ALUSrc_out);

    MemToReg = new regT<bool>("id_ex_MemToReg");
    MemToReg->clk(clk);
    MemToReg->din(MemToReg_in);
    MemToReg->dout(MemToReg_out);

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