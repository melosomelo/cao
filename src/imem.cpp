#include "imem.h"

void imem::entry()
{
  assert(addr.read() % 4 == 0);
  inst.write(memory[addr.read() / 4]);
}