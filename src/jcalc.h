#ifndef JCALC_H_
#define JCALC_H_

#include <systemc.h>

SC_MODULE(jcalc)
{
  sc_in<sc_uint<26>> jumpaddr26;
  sc_in<sc_uint<32>> pc4;
  sc_out<sc_uint<32>> result;

  SC_CTOR(jcalc)
  {
    SC_METHOD(entry);
    sensitive << jumpaddr26 << pc4;
  }

  void entry();
};

#endif