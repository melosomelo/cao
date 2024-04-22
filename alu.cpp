#include "alu.h"

void ALU::compute()
{

  switch (op.read())
  {
  case ALUOperation::AND:
    result.write(a.read() & b.read());
    break;
  case ALUOperation::OR:
    result.write(a.read() | b.read());
    break;
  case ALUOperation::XOR:
    result.write(a.read() ^ b.read());
    break;
  case ALUOperation::NOT:
    result.write(~a.read());
    break;
  case ALUOperation::ADD:
    result.write(a.read() + b.read());
    break;
  case ALUOperation::SUB:
    result.write(a.read() - b.read());
    break;
  case ALUOperation::LESS:
    result.write(a.read() < b.read() ? 1 : 0);
    break;
  }
  zero.write(result.read() == 0);
}

int sc_main(int, char *[])
{
  sc_signal<sc_uint<32>> a, b, result;
  sc_signal<sc_uint<2>> op;
  sc_signal<bool> zero;

  ALU alu("alu");
  alu.a(a);
  alu.b(b);
  alu.result(result);
  alu.op(op);
  alu.zero(zero);
}