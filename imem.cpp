#include "imem.h"

void imem::entry()
{
  inst.write(memory.at(addr.read()));
}