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
  pc_add4->zero(dummy_alu_zero);

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
  // todo: bind regfile data_in
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
  branch_alu->zero(dummy_alu_zero);

  pc_src_mux = new mux2<sc_uint<32>>();
  pc_src_mux->in0(pc4);
  pc_src_mux->in1(branch_alu_result);
  pc_src_mux->sel(PCSrc);
  pc_src_mux->out(pc_src_mux_out);
  /*
    main_alu = new alu("main_alu");

    datamem = new dmem("datamem");



  */
}

void proc::dump_state()
{
  std::ofstream outfile("processor.dump");

  outfile << "pc: " << pc << endl;
  outfile << "pc4: " << pc4 << endl;
  outfile << "current_inst: 0x" << std::hex << current_inst.read() << std::dec << std::endl;

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