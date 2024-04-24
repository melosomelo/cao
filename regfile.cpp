#include "regfile.h"

void regfile::access()
{
  // Write to the register file.
  if (clk.event() && clk.read() && write.read())
  {
    memory[write_reg.read()] = data_in.read();
    return;
  }
  // Otherwise, just read from it.
  data_out1.write(memory[read_reg1.read()]);
  data_out2.write(memory[read_reg2.read()]);
}

/*

int sc_main(int argc, char *argv[])
{
  // Signals
  sc_clock clk("clk", 1, SC_NS);
  sc_signal<bool> write;
  sc_signal<sc_uint<5>> read_reg1, read_reg2, write_reg;
  sc_signal<sc_uint<32>> data_in, data_out1, data_out2;

  // Module instantiation
  regfile rf("regfile");
  rf.clk(clk);
  rf.write(write);
  rf.read_reg1(read_reg1);
  rf.read_reg2(read_reg2);
  rf.write_reg(write_reg);
  rf.data_in(data_in);
  rf.data_out1(data_out1);
  rf.data_out2(data_out2);

  // Initialize signals
  write = 0;
  read_reg1 = 0;
  read_reg2 = 0;
  write_reg = 0;
  data_in = 100;

  // Run simulation
  for (int i = 0; i < 32; ++i)
  {
    // Write to register file
    write = 1;
    data_in = i + 1;
    write_reg = i;
    sc_start(1, SC_NS);

    // Read from register file
    write = 0;
    read_reg1 = i;
    read_reg2 = i;
    sc_start(1, SC_NS);

    // Print register values
    cout << "Cycle " << i << ": ";
    cout << "data_out1 = " << data_out1.read() << ", ";
    cout << "data_out2 = " << data_out2.read() << endl;
  }

  return 0;
}
*/