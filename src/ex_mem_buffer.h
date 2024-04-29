#ifndef EX_MEM_BUFFER_H_
#define EX_MEM_BUFFER_H_

#include <systemc.h>
#include "reg.h"
#include "regT.h"

SC_MODULE(EX_MEM_buffer)
{
  sc_in<bool> clk, main_alu_zero_in,
      Branch_in,
      Jump_in,
      RegWrite_in,
      MemWrite_in,
      MemRead_in,
      MemToReg_in;
  sc_in<sc_uint<32>> branch_alu_result_in, main_alu_result_in, reg2_in;
  sc_in<sc_uint<5>> write_reg_mux_out_in;

  sc_out<bool> main_alu_zero_out, Branch_out, Jump_out, RegWrite_out, MemWrite_out, MemRead_out, MemToReg_out;
  sc_out<sc_uint<32>> branch_alu_result_out, main_alu_result_out, reg2_out;
  sc_out<sc_uint<5>> write_reg_mux_out_out;

  regT<bool> *main_alu_zero;
  regT<bool> *Branch, *Jump, *RegWrite, *MemWrite, *MemRead, *MemToReg;
  regT<sc_uint<32>> *branch_alu_result, *main_alu_result, *reg2;
  regT<sc_uint<5>> *write_reg_mux_out;

  SC_CTOR(EX_MEM_buffer)
  {
    main_alu_zero = new regT<bool>("ex_mem_main_alu_zero");
    main_alu_zero->clk(clk);
    main_alu_zero->din(main_alu_zero_in);
    main_alu_zero->dout(main_alu_zero_out);

    branch_alu_result = new regT<sc_uint<32>>("ex_mem_branch_alu_result");
    branch_alu_result->clk(clk);
    branch_alu_result->din(branch_alu_result_in);
    branch_alu_result->dout(branch_alu_result_out);

    main_alu_result = new regT<sc_uint<32>>("ex_mem_main_alu_result");
    main_alu_result->clk(clk);
    main_alu_result->din(main_alu_result_in);
    main_alu_result->dout(main_alu_result_out);

    reg2 = new regT<sc_uint<32>>("ex_mem_reg2");
    reg2->clk(clk);
    reg2->din(reg2_in);
    reg2->dout(reg2_out);

    write_reg_mux_out = new regT<sc_uint<5>>("ex_mem_write_reg_mux_out");
    write_reg_mux_out->clk(clk);
    write_reg_mux_out->din(write_reg_mux_out_in);
    write_reg_mux_out->dout(write_reg_mux_out_out);

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

    MemToReg = new regT<bool>("id_ex_MemToReg");
    MemToReg->clk(clk);
    MemToReg->din(MemToReg_in);
    MemToReg->dout(MemToReg_out);
  }
};

#endif