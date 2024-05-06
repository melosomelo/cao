#include "imem.h"

void imem::entry()
{
  if (addr.read() >= size())
  {
    // NOP instruction
    inst.write(0x00000000);
    return;
  }
  assert(addr.read() % 4 == 0);
  inst.write(memory[addr.read() / 4]);
}