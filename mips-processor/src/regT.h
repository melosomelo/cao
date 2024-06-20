#ifndef REGTMOD_H
#define REGTMOD_H

#include <systemc.h>

/**
 * regT template.
 * regT template implements a variable width register.
 * The type of data is selected by the template class T.
 * Synchronous on writes and resets.
 *
 *   - input ports
 *   	- \c T \c din		- input
 *   	- \c bool \c reset		- reset
 *   	- \c bool \c enable		- enable
 *   	- \c bool \c clk		- clock
 *   - output ports
 *   	- \c T \c dout		- output
 */

template <class T>
class regT : public sc_module
{
public:
  sc_in<T> din;
  sc_out<T> dout;

  sc_in<bool> clk;

  T val;

  SC_CTOR(regT)
  {
    SC_METHOD(entry);
    sensitive << clk.pos();
    val = 0;
  }

  void entry();
};

template <class T>
void regT<T>::entry()
{
  if (clk.event() && clk)
  {
    val = din.read();
    dout.write(val);
  }
}

#endif