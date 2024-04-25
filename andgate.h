#ifndef AND_GATE_H_
#define AND_GATE_H_

#include <systemc.h>

SC_MODULE(andgate)
{

  sc_in<bool> din1;
  sc_in<bool> din2;
  sc_out<bool> dout;

  SC_CTOR(andgate)
  {
    SC_METHOD(entry);
    sensitive << din1 << din2;
  }

  void entry();
};

#endif