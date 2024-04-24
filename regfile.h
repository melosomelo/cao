#ifndef REGFILE_H_
#define REGFILE_H_

#include <systemc.h>
#include "mem.h"

class regfile : public sc_module, public mem
{

public:
  /** First register to be read*/
  sc_in<sc_uint<5>> read_reg1;
  /** Second register to be read*/
  sc_in<sc_uint<5>> read_reg2;
  /** Register to be written to */
  sc_in<sc_uint<5>> write_reg;
  /** Data to be written into specified register */
  sc_in<sc_uint<32>> data_in;
  /** Control signal to enable writing to the register file */
  sc_in<bool> write;
  /** Clock signal */
  sc_in<bool> clk;

  /** Value of register specified by reg1 */
  sc_out<sc_uint<32>> data_out1;
  /** Value of register specified by reg2 */
  sc_out<sc_uint<32>> data_out2;

  SC_CTOR(regfile)
  {
    SC_METHOD(access);
    sensitive << clk.pos() << read_reg1 << read_reg2;

    // Initializing the registers all to 0.
    for (int i = 0; i < 32; i++)
      memory.push_back(0);
  }

  void access();
};

#endif