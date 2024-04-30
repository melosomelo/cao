#ifndef MEM_WB_BUFFER_H_
#define MEM_WB_BUFFER_H_

#include <systemc.h>
#include "reg.h"
#include "regT.h"

SC_MODULE(MEM_WB_buffer)
{
  sc_in<bool> clk, MemToReg_in, RegWrite_in, Jump_in;
  sc_in<sc_uint<32>> dmem_out_in, main_alu_result_in;

  sc_out<bool> MemToReg_out, RegWrite_out, Jump_out;
  sc_out<sc_uint<32>> dmem_out_out, main_alu_result_out;

  regT<bool> *MemToReg, *RegWrite, *Jump;
  regT<sc_uint<32>> *dmem_out, *main_alu_result;

  SC_CTOR(MEM_WB_buffer)
  {
    MemToReg = new regT<bool>("mem_wb_MemToReg");
    MemToReg->clk(clk);
    MemToReg->din(MemToReg_in);
    MemToReg->dout(MemToReg_out);

    RegWrite = new regT<bool>("mem_wb_RegWrite");
    RegWrite->clk(clk);
    RegWrite->din(RegWrite_in);
    RegWrite->dout(RegWrite_out);

    Jump = new regT<bool>("mem_wb_Jump");
    Jump->clk(clk);
    Jump->din(Jump_in);
    Jump->dout(Jump_out);

    dmem_out = new regT<sc_uint<32>>("mem_wb_dmem_out");
    dmem_out->clk(clk);
    dmem_out->din(dmem_out_in);
    dmem_out->dout(dmem_out_out);

    main_alu_result = new regT<sc_uint<32>>("mem_wb_main_alu_result");
    main_alu_result->clk(clk);
    main_alu_result->din(main_alu_result_in);
    main_alu_result->dout(main_alu_result_out);
  };
};

#endif