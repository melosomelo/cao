#ifndef MEM_WB_BUFFER_H_
#define MEM_WB_BUFFER_H_

#include <systemc.h>
#include "reg.h"
#include "regT.h"

SC_MODULE(MEM_WB_buffer)
{
  sc_in<bool> clk, MemToReg_in, RegWrite_in;
  sc_in<sc_uint<32>> dmem_out_in, main_alu_result_in;
  sc_in<sc_uint<5>> write_reg_mux_out_in;

  sc_out<bool> MemToReg_out, RegWrite_out;
  sc_out<sc_uint<32>> dmem_out_out, main_alu_result_out;
  sc_out<sc_uint<5>> write_reg_mux_out_out;

  regT<bool> *MemToReg, *RegWrite;
  regT<sc_uint<32>> *dmem_out, *main_alu_result;
  regT<sc_uint<5>> *write_reg_mux_out;

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

    dmem_out = new regT<sc_uint<32>>("mem_wb_dmem_out");
    dmem_out->clk(clk);
    dmem_out->din(dmem_out_in);
    dmem_out->dout(dmem_out_out);

    main_alu_result = new regT<sc_uint<32>>("mem_wb_main_alu_result");
    main_alu_result->clk(clk);
    main_alu_result->din(main_alu_result_in);
    main_alu_result->dout(main_alu_result_out);

    write_reg_mux_out = new regT<sc_uint<5>>("mem_wb_write_reg_mux_out");
    write_reg_mux_out->clk(clk);
    write_reg_mux_out->din(write_reg_mux_out_in);
    write_reg_mux_out->dout(write_reg_mux_out_out);
  };
};

#endif