#include "proc.h"

void proc::init()
{
  pc_reg = new reg("pc_reg");
  pc_reg->in(pc_src_mux_out);
  pc_reg->reset(reset);
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
  dcode->instruction(current_inst);
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
  rfile->RegWrite(RegWrite);
  rfile->read_reg1(rs);
  rfile->read_reg2(rt);
  rfile->write_reg(write_reg_mux_out);
  rfile->data_in(rfile_data_in);
  rfile->data_out1(rfile_out1);
  rfile->data_out2(rfile_out2);

  extend32 = new extend("extend32");
  extend32->in(offset);
  extend32->out(extended_offset);

  sl2 = new shiftl2("sl2");
  sl2->in(extended_offset);
  sl2->out(sl2_extended_offset);

  branch_alu = new alu("branch_alu");
  branch_alu->a(pc4);
  branch_alu->b(sl2_extended_offset);
  branch_alu->op(const_add_op);
  branch_alu->result(branch_alu_result);
  branch_alu->zero(dummy_branch_alu_zero);

  branch_alu_pc4_mux = new mux2<sc_uint<32>>("branch_alu_pc4_mux");
  branch_alu_pc4_mux->in0(pc4);
  branch_alu_pc4_mux->in1(branch_alu_result);
  branch_alu_pc4_mux->sel(take_branch);
  branch_alu_pc4_mux->out(branch_alu_pc4_mux_out);

  main_alu_b_mux = new mux2<sc_uint<32>>("main_alu_b_mux");
  main_alu_b_mux->in0(rfile_out2);
  main_alu_b_mux->in1(extended_offset);
  main_alu_b_mux->sel(ALUSrc);
  main_alu_b_mux->out(main_alu_b_mux_out);

  main_alu = new alu("main_alu");
  main_alu->a(rfile_out1);
  main_alu->b(main_alu_b_mux_out);
  main_alu->op(ALUOp);
  main_alu->result(main_alu_result);
  main_alu->zero(main_alu_zero);

  datamem = new dmem("datamem");
  datamem->addr(main_alu_result);
  datamem->data_in(rfile_out2);
  datamem->MemWrite(MemWrite);
  datamem->MemRead(MemRead);
  datamem->clk(clk);
  datamem->data_out(dmem_out);

  rfile_data_in_mux = new mux2<sc_uint<32>>("rfile_data_in_mux");
  rfile_data_in_mux->in0(main_alu_result);
  rfile_data_in_mux->in1(dmem_out);
  rfile_data_in_mux->sel(MemtoReg);
  rfile_data_in_mux->out(rfile_data_in);

  write_reg_mux = new mux2<sc_uint<5>>("write_reg_mux");
  write_reg_mux->in0(rt);
  write_reg_mux->in1(rd);
  write_reg_mux->sel(RegDst);
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
  Branch_and_main_alu_zero->in1(Branch);
  Branch_and_main_alu_zero->in2(main_alu_zero);
  Branch_and_main_alu_zero->out(take_branch);

  j_calc = new jcalc("j_calc");
  j_calc->jumpaddr26(jumpaddr26);
  j_calc->pc4(pc4);
  j_calc->result(jumpaddr32);

  pc_src_mux = new mux2<sc_uint<32>>("pc_src_mux");
  pc_src_mux->in0(branch_alu_pc4_mux_out);
  pc_src_mux->in1(jumpaddr32);
  pc_src_mux->sel(Jump);
  pc_src_mux->out(pc_src_mux_out);
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

int sc_main(int argc, char *argv[])
{
  sc_clock clk("clk", 1, SC_NS);
  sc_signal<bool> reset;

  proc proc("proc");

  proc.clk(clk);
  proc.reset(reset);

  // Manually setting the contents of the register (for now)
  proc.rfile->memory[reg_num::$t0] = 0;
  proc.rfile->memory[reg_num::$t1] = 10;
  proc.rfile->memory[reg_num::$t2] = 20;
  proc.rfile->memory[reg_num::$t3] = 30;

  proc.load_instruction_memory(std::vector<sc_uint<32>>{
      0xad090000, // sw $t1,0($t0)
      0xad0a0004, // sw $t2,4(t$0)
      0xad0b0008, // sw $t3,8(t$0)
      0x8d0c0008, // lw $t4,8($t0)
      0x01296820, // add $t5,$t1,$t1
      0x01697022, // sub $t6,$t3,$t1
      0x01697824, // and $t7,$t3,$t1
      0x01498025, // or $s0,$t2,$t1
      0x012a882a, // slt $s1,$t1,$t2
      0x11290002, // beq $t1,$t1,2
      0x01697824, // and $t7,$t3,$t1
      0x01498025, // or $s0,$t2,$t1
      0x012a882a, // slt $s1,$t1,$t2
      0x08000001  // j 1
  });

  proc.load_data_memory(std::vector<sc_uint<32>>{
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

  for (int i = 0; i < 13; ++i)
  {
    sc_start(1, SC_NS);
  }

  proc.dump_state();

  return 0;
}

void proc::load_instruction_memory(std::vector<sc_uint<32>> instructions)
{
  for (auto i : instructions)
  {
    this->inst_mem->memory.push_back(i);
  }
}

void proc::load_data_memory(std::vector<sc_uint<32>> data)
{
  for (auto v : data)
  {
    this->datamem->memory.push_back(v);
  }
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