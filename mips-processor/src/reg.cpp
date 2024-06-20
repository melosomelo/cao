#include "reg.h"

void reg::entry()
{
  if (clk.event() && clk)
  {
    val = in.read();
    out.write(val);
  }
}