#include "regfile.h"

void regfile::access()
{
  if (clk.event() && clk.read() && RegWrite.read())
  {
    memory[write_reg.read()] = data_in.read();
  }
  data_out1.write(memory[read_reg1.read()]);
  data_out2.write(memory[read_reg2.read()]);
}
