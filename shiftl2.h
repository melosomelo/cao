#ifndef SHIFTL2_H_
#define SHIFTL2_H_

#include <systemc.h>

SC_MODULE(shiftl2)
{
  sc_in<sc_uint<32>> in;
  sc_out<sc_uint<32>> out;

  SC_CTOR(shiftl2)
  {
    SC_METHOD(entry);
    sensitive << in;
  }
  void entry();
};

#endif