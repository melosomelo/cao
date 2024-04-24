#include "extend.h"

void extend::entry()
{
  sc_int<16> aux16;
  sc_int<32> aux32;
  sc_uint<32> auxu;

  aux16 = in.read();
  aux32 = aux16;
  auxu = aux32;

  out.write(auxu);
}