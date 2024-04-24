#include "decode.h"

void decode::entry()
{
  sc_uint<32> temp_inst;
  temp_inst = instruction.read();

  opcode.write(temp_inst.range(31, 26));
  rs.write(temp_inst.range(25, 21));
  rt.write(temp_inst.range(20, 16));
  rd.write(temp_inst.range(15, 11));
  shamt.write(temp_inst.range(10, 6));
  funct.write(temp_inst.range(5, 0));
  offset.write(temp_inst.range(15, 0));
}