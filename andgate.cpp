#include "andgate.h"

void andgate::entry()
{
  out.write(in1.read() && in2.read());
}
