#include "proc.h"

void proc::init()
{
  pc_reg = new reg("pc_reg");
  pc_reg->in(pc_src_mux_out);
  pc_reg->reset(reset);
  pc_reg->clk(clk);
  pc_reg->out(pc);

  inst_mem = new imem("imem");
  inst_mem->addr(pc);
  inst_mem->inst(current_inst);

  pc_add4 = new alu("add4");
  pc_add4->a(pc);
  pc_add4->b(const4);
  pc_add4->op(const_add_op);
  pc_add4->result(pc4);
  pc_add4->zero(dummy_pc4_zero);

  dcode = new decode("dcode");
  dcode->instruction(current_inst);
  dcode->opcode(opcode);
  dcode->rs(rs);
  dcode->rt(rt);
  dcode->rd(rd);
  dcode->shamt(shamt);
  dcode->funct(funct);
  dcode->offset(offset);

  rfile = new regfile("rfile");
  rfile->clk(clk);
  rfile->RegWrite(RegWrite);
  rfile->read_reg1(rs);
  rfile->read_reg2(rt);
  rfile->write_reg(rd);
  rfile->data_in(rfile_data_in);
  rfile->data_out1(rfile_out1);
  rfile->data_out2(rfile_out2);

  extend32 = new extend("extend32");
  extend32->in(offset);
  extend32->out(extended_offset);

  sl2 = new shiftl2("sl2");
  sl2->in(extended_offset);
  sl2->out(sl2_extended_offset);

  branch_alu = new alu("branch_alu");
  branch_alu->a(pc4);
  branch_alu->b(sl2_extended_offset);
  branch_alu->op(const_add_op);
  branch_alu->result(branch_alu_result);
  branch_alu->zero(dummy_branch_alu_zero);

  pc_src_mux = new mux2<sc_uint<32>>("pc_src_mux");
  pc_src_mux->in0(pc4);
  pc_src_mux->in1(branch_alu_result);
  pc_src_mux->sel(PCSrc);
  pc_src_mux->out(pc_src_mux_out);

  main_alu_b_mux = new mux2<sc_uint<32>>("main_alu_b_mux");
  main_alu_b_mux->in0(rfile_out2);
  main_alu_b_mux->in1(extended_offset);
  main_alu_b_mux->sel(ALUSrc);
  main_alu_b_mux->out(main_alu_b_mux_out);

  main_alu = new alu("main_alu");
  main_alu->a(rfile_out1);
  main_alu->b(main_alu_b_mux_out);
  main_alu->op(ALUOp);
  main_alu->result(main_alu_result);
  main_alu->zero(main_alu_zero);

  datamem = new dmem("datamem");
  datamem->addr(main_alu_result);
  datamem->data_in(rfile_out2);
  datamem->MemWrite(MemWrite);
  datamem->MemRead(MemRead);
  datamem->clk(clk);
  datamem->data_out(dmem_out);

  rfile_data_in_mux = new mux2<sc_uint<32>>("rfile_data_in_mux");
  rfile_data_in_mux->in0(dmem_out);
  rfile_data_in_mux->in1(main_alu_result);
  rfile_data_in_mux->sel(MemtoReg);
  rfile_data_in_mux->out(rfile_data_in);

  ctrl = new control("ctrl");
  ctrl->opcode(opcode);
  ctrl->funct(funct);
  ctrl->RegDst(RegDst);
  ctrl->Branch(Branch);
  ctrl->RegWrite(RegWrite);
  ctrl->MemWrite(MemWrite);
  ctrl->MemRead(MemRead);
  ctrl->PCSrc(PCSrc);
  ctrl->ALUSrc(ALUSrc);
  ctrl->MemtoReg(MemtoReg);
  ctrl->ALUOp(ALUOp);
}

void proc::dump_state()
{
  std::ofstream outfile("processor.dump");

  outfile.close();
}

int sc_main(int argc, char *argv[])
{
  sc_clock clk("clk", 1, SC_NS);
  sc_signal<bool> reset;

  proc proc("proc");

  proc.clk(clk);
  proc.reset(reset);

  // Simulation time variables
  sc_time sim_time = SC_ZERO_TIME;
  sc_time cycle_time = sc_time(1, SC_NS);

  // Simulate for 10 cycles
  for (int i = 0; i < 1; ++i)
  {
    // Wait for 1 second
    sc_start(cycle_time);

    // Update simulation time
    sim_time += cycle_time;
  }

  proc.dump_state();

  return 0;
}