#include "alu.h"

void ALU::compute()
{
  auto b_value = b.read();
  if (b_invert.read())
  {
    b_value = ~b_value;
  }

  switch (op.read())
  {
  case ALUOperation::AND:
    cout.write(0);
    result.write(a.read() & b_value);
    break;
  case ALUOperation::OR:
    cout.write(0);
    result.write(a.read() | b_value);
    break;
  case ALUOperation::XOR:
    cout.write(0);
    result.write(a.read() ^ b_value);
    break;
  case ALUOperation::ADD:
    cout.write((b_value & cin.read()) | (a.read() & cin.read()) | (a.read() & b_value));
    result.write(a.read() ^ b_value ^ cin.read());
    break;
  }
}

int sc_main(int, char *[])
{
}