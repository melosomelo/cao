#include "jcalc.h"

void jcalc::entry()
{
  sc_uint<32> temp_result;
  sc_uint<26> temp_jumpaddr26 = jumpaddr26.read();
  sc_uint<4> pc4_upper4 = pc4.read().range(31, 28);
  // First 4 upper bits come from the 4 upper bits of PC + 4
  for (int i = 0; i < 4; i++)
  {
    temp_result[31 - i] = pc4_upper4[3 - i];
  }
  // Next 26 bits come from the jumpaddr26
  for (int i = 0; i < 26; i++)
  {
    temp_result[27 - i] = temp_jumpaddr26[25 - i];
  }
  // Last 2 bits are 0.
  temp_result[1] = 0;
  temp_result[0] = 0;
  result.write(temp_result);
}