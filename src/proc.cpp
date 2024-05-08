#include "proc.h"

std::vector<std::string> split_on_whitespace(std::string str)
{
  std::vector<std::string> tokens{};
  std::string acc = "";
  for (int i = 0; i < str.size(); i++)
  {
    // We don't need any empty strings.
    if (str[i] == ' ' && acc.size() > 0)
    {
      tokens.push_back(acc);
      acc.clear();
    }
    else if (str[i] != ' ')
    {
      acc += str[i];
    }
  }
  if (acc.size() > 0)
    tokens.push_back(acc);
  return tokens;
}

std::string trim(std::string s)
{
  // Trimming the string.
  size_t start = 0;
  size_t end = s.length() - 1;
  while (start < end && std::isspace(s[start]))
  {
    start++;
  }
  while (end > start && std::isspace(s[end]))
  {
    end--;
  }
  if (start >= end)
    return "";
  return s.substr(start, end - start + 1);
}

void wrong_syntax(std::string cmd, std::string msg = "")
{
  if (msg.size() > 0)
  {
    std::cout << "error: invalid syntax for " << cmd << ": " << msg << std::endl;
  }
  else
  {
    std::cout << "error: invalid syntax for " << cmd << std::endl;
  }
}

int sc_main(int argc, char *argv[])
{
  sc_clock clk("clk", 1, SC_NS);
  // sc_signal<bool> reset;

  proc proc("proc");

  proc.clk(clk);
  // proc.reset(reset);

  std::string data_memory_filename = "datamem.dat";
  std::string instruction_memory_filename = "instmem.hex";

  for (int i = 0; i < argc; i++)
  {
    if (std::string{argv[i]} == "--data" && i != argc - 1)
    {
      data_memory_filename = std::string{argv[++i]};
    }
    if (std::string{argv[i]} == "--instruction" && i != argc - 1)
    {
      instruction_memory_filename = std::string{argv[++i]};
    }
  }

  proc.datamem->init_memory(data_memory_filename);
  proc.inst_mem->init_memory(instruction_memory_filename);
  proc.rfile->memory[1] = 20;
  proc.rfile->memory[2] = 10;

  std::string line;
  std::cout << "Instruction and data memory have been loaded and simulation is ready to begin." << std::endl
            << "Run 'help' to list the available commands and their syntax." << std::endl;
  while (true)
  {
    std::cout << "> ";
    std::getline(std::cin, line);
    line = trim(line);
    std::vector<std::string> tokens = split_on_whitespace(line);
    auto cmd = tokens.at(0);
    if (cmd == "exit")
    {
      std::cout << "Goodbye!\n";
      exit(0);
    }
    else if (cmd == "help")
    {
      std::cout << "List of commands:\n"
                << "\trun n - runs the simulation for n cycles\n"
                << "\treg n [b|d|x] - prints the value of register n in binary/decimal/hex base\n"
                << "\tregall [b|d|x] - prints the contents of all registers in binary/decimal/hex base\n"
                << "\tdata n [b|d|x] - prints the data word at position n in binary/decimal/hex base\n"
                << "\thelp - prints this message\n"
                << "\texit - stops the program\n";
    }
    else if (cmd == "run")
    {
      if (tokens.size() < 2)
      {
        wrong_syntax(cmd);
        continue;
      }
      int n = std::stoi(tokens.at(1));
      for (int i = 0; i < n; i++)
      {
        sc_start(1, SC_NS);
      }
      std::cout << "Simulation was run for " << n << " cycles.\n";
    }
    else if (cmd == "reg")
    {
      if (tokens.size() < 2)
      {
        wrong_syntax(cmd);
        continue;
      }
      int reg_n = std::stoi(tokens.at(1));
      if (reg_n < 0 || reg_n > 31)
      {
        std::cout << "error: invalid register number (must be between 0 and 31)\n";
        continue;
      }
      std::string base = "x";
      if (tokens.size() >= 3 && (tokens.at(2) == "x" or tokens.at(2) == "d" or tokens.at(2) == "b"))
      {
        base = tokens.at(2);
      }
      std::cout << "Value for register " << reg_n << ": ";

      if (base == "x")
        std::cout << std::hex << proc.rfile->memory[reg_n];
      else if (base == "d")
        std::cout << proc.rfile->memory[reg_n];
      else
        std::cout << std::bitset<32>(proc.rfile->memory[reg_n]);
      std::cout << std::dec << std::endl;
    }
    else if (cmd == "regall")
    {
      std::string base = "x";
      if (tokens.size() >= 2 && (tokens.at(1) == "x" or tokens.at(1) == "d" or tokens.at(1) == "b"))
      {
        base = tokens.at(1);
      }
      for (int i = 0; i < 32; i++)
      {
        std::cout << "Register " << i << ": ";
        if (base == "x")
          std::cout << std::hex << proc.rfile->memory[i];
        else if (base == "d")
          std::cout << proc.rfile->memory[i];
        else
          std::cout << std::bitset<32>(proc.rfile->memory[i]);
        std::cout << std::dec << std::endl;
      }
    }
    else if (cmd == "data")
    {
      if (tokens.size() < 2)
      {
        wrong_syntax(cmd);
        continue;
      }
      int word_n = std::stoi(tokens.at(1));
      if (word_n < 0 || word_n >= proc.datamem->memory.size())
      {
        std::cout << "error: invalid data word number (must be between 0 and " << proc.datamem->memory.size() << ")\n";
        continue;
      }
      std::string base = "x";
      if (tokens.size() >= 3 && (tokens.at(2) == "x" or tokens.at(2) == "d" or tokens.at(2) == "b"))
      {
        base = tokens.at(2);
      }
      std::cout << "Value for data word " << word_n << ": ";

      if (base == "x")
        std::cout << std::hex << proc.datamem->memory[word_n];
      else if (base == "d")
        std::cout << proc.datamem->memory[word_n];
      else
        std::cout << std::bitset<32>(proc.datamem->memory[word_n]);
      std::cout << std::dec << std::endl;
    }
    else
    {
      std::cout << "error: unknown command '" << cmd << "'" << std::endl;
    }
  }

  return 0;
}

void proc::init()
{
  pc_reg = new reg("pc_reg");
  pc_reg->in(pc_src_mux_out);
  // pc_reg->reset(reset);
  pc_reg->clk(clk);
  pc_reg->out(pc);

  inst_mem = new imem("imem");
  inst_mem->addr(pc);
  inst_mem->inst(current_inst);

  pc_add4 = new alu("add4");
  pc_add4->a(pc);
  pc_add4->b(const4);
  pc_add4->op(const_add_op);
  pc_add4->result(pc4);
  pc_add4->zero(dummy_pc4_zero);

  dcode = new decode("dcode");
  dcode->instruction(if_id_buffer_instruction_out);
  dcode->opcode(opcode);
  dcode->rs(rs);
  dcode->rt(rt);
  dcode->rd(rd);
  dcode->shamt(shamt);
  dcode->funct(funct);
  dcode->offset(offset);
  dcode->jumpaddr26(jumpaddr26);

  rfile = new regfile("rfile");
  rfile->clk(clk);
  rfile->RegWrite(mem_wb_RegWrite_out);
  rfile->read_reg1(rs);
  rfile->read_reg2(rt);
  rfile->write_reg(mem_wb_write_reg_mux_out_out);
  rfile->data_in(rfile_data_in);
  rfile->data_out1(rfile_out1);
  rfile->data_out2(rfile_out2);

  extend32 = new extend("extend32");
  extend32->in(offset);
  extend32->out(extended_offset);

  sl2 = new shiftl2("sl2");
  sl2->in(id_ex_buffer_extended_offset_out);
  sl2->out(sl2_extended_offset);

  branch_alu = new alu("branch_alu");
  branch_alu->a(id_ex_buffer_pc4_out);
  branch_alu->b(sl2_extended_offset);
  branch_alu->op(const_add_op);
  branch_alu->result(branch_alu_result);
  branch_alu->zero(dummy_branch_alu_zero);

  branch_alu_pc4_mux = new mux2<sc_uint<32>>("branch_alu_pc4_mux");
  branch_alu_pc4_mux->in0(pc4);
  branch_alu_pc4_mux->in1(ex_mem_branch_alu_result_out);
  branch_alu_pc4_mux->sel(take_branch);
  branch_alu_pc4_mux->out(branch_alu_pc4_mux_out);

  main_alu_b_mux = new mux2<sc_uint<32>>("main_alu_b_mux");
  main_alu_b_mux->in0(id_ex_buffer_reg2_out);
  main_alu_b_mux->in1(id_ex_buffer_extended_offset_out);
  main_alu_b_mux->sel(id_ex_ALUSrc_out);
  main_alu_b_mux->out(main_alu_b_mux_out);

  main_alu = new alu("main_alu");
  main_alu->a(id_ex_buffer_reg1_out);
  main_alu->b(main_alu_b_mux_out);
  main_alu->op(id_ex_ALUOp_out);
  main_alu->result(main_alu_result);
  main_alu->zero(main_alu_zero);

  datamem = new dmem("datamem");
  datamem->addr(ex_mem_main_alu_result_out);
  datamem->data_in(ex_mem_reg2_out);
  datamem->MemWrite(ex_mem_MemWrite_out);
  datamem->MemRead(ex_mem_MemRead_out);
  datamem->clk(clk);
  datamem->data_out(dmem_out);

  rfile_data_in_mux = new mux2<sc_uint<32>>("rfile_data_in_mux");
  rfile_data_in_mux->in0(mem_wb_main_alu_result_out);
  rfile_data_in_mux->in1(mem_wb_dmem_out_out);
  rfile_data_in_mux->sel(mem_wb_MemToReg_out);
  rfile_data_in_mux->out(rfile_data_in);

  write_reg_mux = new mux2<sc_uint<5>>("write_reg_mux");
  write_reg_mux->in0(id_ex_rt_out);
  write_reg_mux->in1(id_ex_rd_out);
  write_reg_mux->sel(id_ex_RegDst_out);
  write_reg_mux->out(write_reg_mux_out);

  ctrl = new control("ctrl");
  ctrl->opcode(opcode);
  ctrl->funct(funct);
  ctrl->RegDst(RegDst);
  ctrl->Branch(Branch);
  ctrl->RegWrite(RegWrite);
  ctrl->MemWrite(MemWrite);
  ctrl->MemRead(MemRead);
  ctrl->ALUSrc(ALUSrc);
  ctrl->MemtoReg(MemtoReg);
  ctrl->ALUOp(ALUOp);
  ctrl->Jump(Jump);

  Branch_and_main_alu_zero = new andgate("Branch_and_main_alu_zero");
  Branch_and_main_alu_zero->in1(ex_mem_Branch_out);
  Branch_and_main_alu_zero->in2(ex_mem_main_alu_zero_out);
  Branch_and_main_alu_zero->out(take_branch);

  j_calc = new jcalc("j_calc");
  j_calc->jumpaddr26(jumpaddr26);
  j_calc->pc4(if_id_buffer_pc4_out);
  j_calc->result(jumpaddr32);

  pc_src_mux = new mux2<sc_uint<32>>("pc_src_mux");
  pc_src_mux->in0(branch_alu_pc4_mux_out);
  pc_src_mux->in1(ex_mem_jumpaddr32_out);
  pc_src_mux->sel(ex_mem_Jump_out);
  pc_src_mux->out(pc_src_mux_out);

  if_id_buffer = new IF_ID_buffer("if_id_buffer");
  if_id_buffer->clk(clk);
  if_id_buffer->pc4_in(pc4);
  if_id_buffer->pc4_out(if_id_buffer_pc4_out);
  if_id_buffer->instruction_in(current_inst);
  if_id_buffer->instruction_out(if_id_buffer_instruction_out);

  id_ex_buffer = new ID_EX_buffer("id_ex_buffer");
  id_ex_buffer->clk(clk);
  id_ex_buffer->pc4_in(if_id_buffer_instruction_out);
  id_ex_buffer->reg1_in(rfile_out1);
  id_ex_buffer->reg2_in(rfile_out2);
  id_ex_buffer->extended_offset_in(extended_offset);
  id_ex_buffer->jumpaddr32_in(jumpaddr32);
  id_ex_buffer->RegDst_in(RegDst);
  id_ex_buffer->Branch_in(Branch);
  id_ex_buffer->Jump_in(Jump);
  id_ex_buffer->RegWrite_in(RegWrite);
  id_ex_buffer->MemWrite_in(MemWrite);
  id_ex_buffer->MemRead_in(MemRead);
  id_ex_buffer->ALUSrc_in(ALUSrc);
  id_ex_buffer->MemToReg_in(MemtoReg);
  id_ex_buffer->ALUOp_in(ALUOp);
  id_ex_buffer->rt_in(rt);
  id_ex_buffer->rd_in(rd);
  id_ex_buffer->pc4_out(id_ex_buffer_pc4_out);
  id_ex_buffer->reg1_out(id_ex_buffer_reg1_out);
  id_ex_buffer->reg2_out(id_ex_buffer_reg2_out);
  id_ex_buffer->extended_offset_out(id_ex_buffer_extended_offset_out);
  id_ex_buffer->jumpaddr32_out(id_ex_buffer_jumpaddr32_out);
  id_ex_buffer->RegDst_out(id_ex_RegDst_out);
  id_ex_buffer->Branch_out(id_ex_Branch_out);
  id_ex_buffer->Jump_out(id_ex_Jump_out);
  id_ex_buffer->RegWrite_out(id_ex_RegWrite_out);
  id_ex_buffer->MemWrite_out(id_ex_MemWrite_out);
  id_ex_buffer->MemRead_out(id_ex_MemRead_out);
  id_ex_buffer->ALUSrc_out(id_ex_ALUSrc_out);
  id_ex_buffer->MemToReg_out(id_ex_MemToReg_out);
  id_ex_buffer->ALUOp_out(id_ex_ALUOp_out);
  id_ex_buffer->rt_out(id_ex_rt_out);
  id_ex_buffer->rd_out(id_ex_rd_out);

  ex_mem_buffer = new EX_MEM_buffer("ex_mem_buffer");
  ex_mem_buffer->clk(clk);
  ex_mem_buffer->main_alu_zero_in(main_alu_zero);
  ex_mem_buffer->Branch_in(id_ex_Branch_out);
  ex_mem_buffer->Jump_in(id_ex_Jump_out);
  ex_mem_buffer->RegWrite_in(id_ex_RegWrite_out);
  ex_mem_buffer->MemWrite_in(id_ex_MemWrite_out);
  ex_mem_buffer->MemRead_in(id_ex_MemRead_out);
  ex_mem_buffer->MemToReg_in(id_ex_MemToReg_out);
  ex_mem_buffer->branch_alu_result_in(branch_alu_result);
  ex_mem_buffer->main_alu_result_in(main_alu_result);
  ex_mem_buffer->reg2_in(id_ex_buffer_reg2_out);
  ex_mem_buffer->write_reg_mux_out_in(write_reg_mux_out);
  ex_mem_buffer->jumpaddr32_in(id_ex_buffer_jumpaddr32_out);
  ex_mem_buffer->main_alu_zero_out(ex_mem_main_alu_zero_out);
  ex_mem_buffer->Branch_out(ex_mem_Branch_out);
  ex_mem_buffer->Jump_out(ex_mem_Jump_out);
  ex_mem_buffer->RegWrite_out(ex_mem_RegWrite_out);
  ex_mem_buffer->MemWrite_out(ex_mem_MemWrite_out);
  ex_mem_buffer->MemRead_out(ex_mem_MemRead_out);
  ex_mem_buffer->MemToReg_out(ex_mem_MemToReg_out);
  ex_mem_buffer->branch_alu_result_out(ex_mem_branch_alu_result_out);
  ex_mem_buffer->main_alu_result_out(ex_mem_main_alu_result_out);
  ex_mem_buffer->reg2_out(ex_mem_reg2_out);
  ex_mem_buffer->write_reg_mux_out_out(ex_mem_write_reg_mux_out_out);
  ex_mem_buffer->jumpaddr32_out(ex_mem_jumpaddr32_out);

  mem_wb_buffer = new MEM_WB_buffer("mem_wb_buffer");
  mem_wb_buffer->clk(clk);
  mem_wb_buffer->MemToReg_in(ex_mem_MemToReg_out);
  mem_wb_buffer->RegWrite_in(ex_mem_RegWrite_out);
  mem_wb_buffer->dmem_out_in(dmem_out);
  mem_wb_buffer->main_alu_result_in(ex_mem_main_alu_result_out);
  mem_wb_buffer->write_reg_mux_out_in(ex_mem_write_reg_mux_out_out);
  mem_wb_buffer->MemToReg_out(mem_wb_MemToReg_out);
  mem_wb_buffer->RegWrite_out(mem_wb_RegWrite_out);
  mem_wb_buffer->dmem_out_out(mem_wb_dmem_out_out);
  mem_wb_buffer->main_alu_result_out(mem_wb_main_alu_result_out);
  mem_wb_buffer->write_reg_mux_out_out(mem_wb_write_reg_mux_out_out);
}

proc::~proc()
{
  delete pc_reg;
  delete inst_mem;
  delete pc_add4;
  delete dcode;
  delete rfile;
  delete datamem;
  delete main_alu;
  delete branch_alu;
  delete extend32;
  delete sl2;
  delete branch_alu_pc4_mux;
  delete main_alu_b_mux;
  delete rfile_data_in_mux;
  delete ctrl;
}

void proc::dump_state()
{
  std::ofstream outfile("processor.dump");

  outfile << "SIGNALS:\n";
  outfile << "pc: " << this->pc << std::endl;
  outfile << "current instruction: " << std::hex << this->current_inst << std::dec << std::endl;
  outfile << "rfile out 1: " << this->rfile_out1 << std::endl;
  outfile << "rfile out 2: " << this->rfile_out2 << std::endl;
  outfile << "write_reg_mux_out: " << this->write_reg_mux_out << std::endl;
  outfile << "rfile_data_in: " << this->rfile_data_in << std::endl;
  outfile << "extended_offset: " << this->extended_offset << std::endl;
  outfile << "branch_alu_result: " << this->branch_alu_result << std::endl;
  outfile << "branch_alu_pc4_mux_out: " << this->branch_alu_pc4_mux_out << std::endl;
  outfile << "main_alu_b_mux_out: " << this->main_alu_b_mux_out << std::endl;
  outfile << "main_alu_result: " << this->main_alu_result << std::endl;
  outfile << "dmem_out: " << this->dmem_out << std::endl;
  outfile << "main_alu_zero: " << main_alu_zero << std::endl;
  outfile << "take_branch: " << take_branch << std::endl;
  outfile << "RegDst: " << RegDst << std::endl;
  outfile << "Branch: " << Branch << std::endl;
  outfile << "RegWrite: " << RegWrite << std::endl;
  outfile << "MemWrite: " << MemWrite << std::endl;
  outfile << "MemRead: " << MemRead << std::endl;
  outfile << "ALUSrc: " << ALUSrc << std::endl;
  outfile << "MemToReg: " << MemtoReg << std::endl;
  outfile << "ALUOp: " << ALUOp << std::endl;

  outfile << "\nREGISTERS:\n";
  for (int i = 0; i < rfile->memory.size(); i++)
  {
    outfile << "R" << i << ": " << rfile->memory[i] << std::endl;
  }

  outfile << "\nDATA MEMORY:\n";
  for (int i = 0; i < datamem->memory.size(); i++)
  {
    outfile << i << ": " << datamem->memory[i] << std::endl;
  }

  outfile.close();
}