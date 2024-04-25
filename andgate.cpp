#include "andgate.h"

void andgate::entry()
{
  dout.write(din1.read() && din2.read());
}
