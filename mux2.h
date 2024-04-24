#ifndef MUX2_H_
#define MUX2_H_

#include <systemc.h>

template <class T>
class mux2 : public sc_module
{
public:
  sc_in<T> in0;
  sc_in<T> in1;
  sc_in<bool> sel;
  sc_out<T> out;

  SC_CTOR(mux)
  {
    SC_METHOD(entry);
    sensitive << in0 << in1 << sel;
  }

  void entry();
};

template <class T>
void mux2<T>::entry()
{
  if (sel.read())
    out.write(in1.read());
  else
    out.write(in0.read());
}

#endif