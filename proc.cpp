#include "proc.h"

void proc::init()
{
  pc_reg = new reg("pc_reg");
  pc_reg->in(pc4);
  pc_reg->reset(reset);
  pc_reg->clk(clk);
  pc_reg->out(pc);

  inst_mem = new imem("imem");
  inst_mem->addr(pc);
  inst_mem->inst(current_inst);

  add4 = new alu("add4");
  add4->a(pc);
  add4->b(const4);
  add4->op(add4_op);
  add4->result(pc4);
  add4->zero(dummy_zero_add4);
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