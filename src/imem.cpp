#include "imem.h"

void imem::entry()
{
  if (addr.read() >= size())
  {
    return sc_stop();
  }
  assert(addr.read() % 4 == 0);
  inst.write(memory[addr.read() / 4]);
}