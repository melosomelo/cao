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
    result.write(a.read() & b_value);
    break;
  case ALUOperation::OR:
    result.write(a.read() | b_value);
    break;
  case ALUOperation::XOR:
    result.write(a.read() ^ b_value);
    break;
  case ALUOperation::ADD:
    result.write(a.read() ^ b_value ^ cin.read());
    break;
  }
  zero.write(result.read() == 0);
  less.write(a < b);
}

int sc_main(int, char *[])
{
  sc_signal<sc_uint<32>> a, b, result;
  sc_signal<bool> cin, b_invert, cout;

  ALU alu("alu");
  alu.a(a);
  alu.b(b);
  alu.cin(cin);
  alu.b_invert(b_invert);
  alu.result(result);
  alu.cout(cout);
}