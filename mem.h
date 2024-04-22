#ifndef MEM_H_
#define MEM_H_

#include <systemc.h>
#include <vector>
#include <string>

using std::string;
using std::vector;

/**
 * Base class of all memory elements. Just holds a vector of 32-bit entries.
 */
class mem
{
public:
  /** Initialization method that reads the memory's initial content from a file */
  void init_memory(string filename);

  /** Size of the memory in bytes */
  unsigned int size();

  /** Returns the word that starts at address addr. **/
  sc_uint<32> at(sc_uint<32> addr);

protected:
  vector<sc_uint<32>> memory;
};

#endif