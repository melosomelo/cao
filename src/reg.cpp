#include "reg.h"

void reg::entry()
{
  if (reset)
  {
    val = 0;
    out.write(val);
  }
  else if (clk.event() && clk)
  {
    val = in.read();
    out.write(val);
  }
}