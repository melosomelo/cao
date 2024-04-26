#ifndef AND_GATE_H_
#define AND_GATE_H_

#include <systemc.h>

SC_MODULE(andgate)
{

  sc_in<bool> in1;
  sc_in<bool> in2;
  sc_out<bool> out;

  SC_CTOR(andgate)
  {
    SC_METHOD(entry);
    sensitive << in1 << in2;
  }

  void entry();
};

#endif