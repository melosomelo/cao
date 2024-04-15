#include <systemc.h>

using namespace sc_core;

SC_MODULE(OneBitFullAdder)
{
  sc_in<bool> a, b, cin;
  sc_out<bool> sum, cout;

  void compute()
  {
    cout.write((b & cin) | (a & cin) | (a & b));
    sum.write((a & !b & !cin) | (!a & b & !cin) | (!a & !b & cin) | (a & b & cin));
  }

  SC_CTOR(OneBitFullAdder)
  {
    SC_METHOD(compute);
    sensitive << a << b << cin;
  }
};

int sc_main(int, char *[])
{
  sc_signal<bool> a, b, cin, sum, cout;

  OneBitFullAdder adder("adder");
  adder.a(a);
  adder.b(b);
  adder.cin(cin);
  adder.sum(sum);
  adder.cout(cout);

  a.write(0);
  b.write(0);
  cin.write(0);

  sc_start();

  std::cout << "a: " << a.read() << endl;
  std::cout << "b: " << b.read() << endl;
  std::cout << "cin: " << cin.read() << endl;
  std::cout << "sum: " << sum.read() << endl;
  std::cout << "cout: " << cout.read() << endl;

  return 0;
}