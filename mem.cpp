#include "mem.h"

using std::string;
using std::vector;

void mem::init_memory(string filename)
{
  ifstream file(filename);

  if (!file)
  {
    std::cerr << "ERROR: Could not find file " << filename << std::endl;
    exit(1);
  }

  std::string line;
  while (std::getline(file, line))
  {
    size_t comment_start = line.find('#');
    // Line has a comment. We ignore it.
    if (comment_start != std::string::npos)
    {
      line = line.substr(0, comment_start);
    }
    // Trimming the string.
    size_t start = 0;
    size_t end = line.length() - 1;
    while (start < end && std::isspace(line[start]))
    {
      start++;
    }
    while (end > start && std::isspace(line[end]))
    {
      end--;
    }
    line = line.substr(start, end - start + 1);
    // Parsing the hexadecimal value and placing it into the memory
    unsigned int hex_val;
    if (sscanf(line.c_str(), "%x", &hex_val) == 1)
    {
      memory.push_back(hex_val);
    }
  }
}

unsigned int mem::size()
{
  // Since each element of memory is 32-bits (4 bytes) long
  // the total length in bytes is the amount of memory elements times 4.
  return memory.size() * 4;
}

sc_uint<32> mem::at(sc_uint<32> addr)
{
  // Since each word is 32 bits (4 bytes) long, to get the proper index
  // we need to divide the parameter by 4.
  // Ex: mem.at(4) returns the element at index 1.
  assert(addr < size() && addr % 4 == 0);
  return memory[addr / 4];
}
