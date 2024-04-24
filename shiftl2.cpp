#include "shiftl2.h"

void shiftl2::entry()
{
  out.write(in.read() << 2);
}