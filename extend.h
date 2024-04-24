#ifndef EXTEND_H_
#define EXTEND_H_

#include <systemc.h>

SC_MODULE(extend)
{
  sc_in<sc_uint<16>> in;
  sc_out<sc_uint<32>> out;

  SC_CTOR(extend)
  {
    SC_METHOD(entry);
    sensitive << in;
  }

  void entry();
};

#endif