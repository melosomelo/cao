#include "alu.h"

void alu::compute()
{
  switch (op.read())
  {
  case alu_op::and_op:
    result.write(a.read() & b.read());
    break;
  case alu_op::or_op:
    result.write(a.read() | b.read());
    break;
  case alu_op::xor_op:
    result.write(a.read() ^ b.read());
    break;
  case alu_op::not_op:
    result.write(~a.read());
    break;
  case alu_op::add_op:
    result.write(a.read() + b.read());
    break;
  case alu_op::sub_op:
    result.write(a.read() - b.read());
    break;
  case alu_op::less_op:
    result.write(a.read() < b.read() ? 1 : 0);
    break;
  }
  zero.write(result.read() == 0);
}