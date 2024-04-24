#include "dmem.h"

void dmem::read()
{
  if (MemRead.read())
  {
    if (addr.read() < size() && addr.read() % 4 == 0)
      data_out.write(memory[addr.read() / 4]);
    else
      data_out.write(0);
  }
}

void dmem::write()
{
  if (MemWrite.read())
  {
    if ((addr.read() < size() && addr.read() % 4 == 0))
    {
      memory[addr.read() / 4] = data_in.read();
    }
  }
}