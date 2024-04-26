#include "alu.h"

void alu::compute()
{
  sc_uint<32> temp_result;
  switch (op.read())
  {
  case alu_op::and_op:
    temp_result = a.read() & b.read();
    break;
  case alu_op::or_op:
    temp_result = a.read() | b.read();
    break;
  case alu_op::xor_op:
    temp_result = a.read() ^ b.read();
    break;
  case alu_op::not_op:
    temp_result = ~a.read();
    break;
  case alu_op::add_op:
    temp_result = a.read() + b.read();
    break;
  case alu_op::sub_op:
    temp_result = a.read() - b.read();
    break;
  case alu_op::less_op:
    temp_result = a.read() < b.read() ? 1 : 0;
    break;
  }
  zero.write(result.read() == 0);
}