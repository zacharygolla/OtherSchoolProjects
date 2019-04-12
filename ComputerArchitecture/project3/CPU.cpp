/******************************
 * Zachary Golla
 * CS 3339 Section 263 - Spring 2018
 * Assignment 2
 ******************************/
#include "CPU.h"

const string CPU::regNames[] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3",
                                "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
                                "$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
                                "$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

CPU::CPU(uint32_t pc, Memory &iMem, Memory &dMem) : pc(pc), iMem(iMem), dMem(dMem) {
  for(int i = 0; i < NREGS; i++) {
    regFile[i] = 0;
  }
  hi = 0;
  lo = 0;
  regFile[28] = 0x10008000; // gp
  regFile[29] = 0x10000000 + dMem.getSize(); // sp

  instructions = 0;
  stop = false;
}

void CPU::run() {
  while(!stop) {
    instructions++;

	stats.clock();
    fetch();
    decode();
    execute();
    mem();
    writeback();

    D(printRegFile());
  }
}

void CPU::fetch() {
  instr = iMem.loadWord(pc);
  pc = pc + 4;
}

/////////////////////////////////////////
// ALL YOUR CHANGES GO IN THIS FUNCTION 
/////////////////////////////////////////
void CPU::decode() {
  uint32_t opcode;      // opcode field
  uint32_t rs, rt, rd, r;  // register specifiers
  uint32_t shamt;       // shift amount (R-type)
  uint32_t funct;       // funct field (R-type)
  uint32_t uimm;        // unsigned version of immediate (I-type)
  int32_t simm;         // signed version of immediate (I-type)
  uint32_t addr;        // jump address offset field (J-type)

  opcode = instr >> 26;
  rs = (instr >> 21) & 0x1f;
  rt = (instr >> 16) & 0x1f;
  rd = (instr >> 11) & 0x1f;
  shamt = (instr >> 6) & 0x1f;
  funct = (instr & 0x3f);
  uimm = instr & 0xffff;
  simm = (int16_t)uimm;
  addr = instr & 0x3ffffff;
    
  // Hint: you probably want to give all the control signals some "safe"
  // default value here, and then override their values as necessary in each
  // case statement below!

  writeDest = false;
  opIsStore = false;
  opIsLoad = false;
  opIsMultDiv = false;
  aluOp = ADD;
  destReg = regFile[REG_ZERO];
  aluSrc1 = regFile[REG_ZERO];
  aluSrc2 = regFile[REG_ZERO];
  storeData = 0;

  D(cout << "  " << hex << setw(8) << pc - 4 << ": ");
  switch(opcode) {
    case 0x00:
      switch(funct) {
        case 0x00: D(cout << "sll " << regNames[rd] << ", " << regNames[rs] << ", " << dec << shamt);
                   writeDest = true;
                   aluOp = SHF_L;
                   destReg = rd;
				   stats.registerDest(destReg);
                   aluSrc1 = regFile[rs];
				   stats.registerSrc(rs);
                   aluSrc2 = shamt;
				   stats.registerSrc(REG_ZERO);
                   break;
        case 0x03: D(cout << "sra " << regNames[rd] << ", " << regNames[rs] << ", " << dec << shamt);
                   writeDest = true;
                   aluOp = SHF_R;
                   destReg = rd;
                   stats.registerDest(destReg);
                   aluSrc1 = regFile[rs];
				   stats.registerSrc(rs);
                   aluSrc2 = shamt;
				   stats.registerSrc(REG_ZERO);
                   break;
        case 0x08: D(cout << "jr " << regNames[rs]);
                   writeDest = false;   // do not write alu result to a register
				   stats.registerDest(REG_ZERO);
                   pc = regFile[rs];    // set the next pc location to the address in rs
				   stats.registerSrc(rs);
				   stats.registerSrc(REG_ZERO);
                   break;
        case 0x10: D(cout << "mfhi " << regNames[rd]);
                   writeDest=true;
                   aluOp = ADD;
                   destReg=rd;
				   stats.registerDest(destReg);
                   aluSrc1=hi;
				   stats.registerSrc(REG_HILO);
                   aluSrc2=regFile[REG_ZERO];
				   stats.registerSrc(REG_ZERO);
                   break;
        case 0x12: D(cout << "mflo " << regNames[rd]);
                   writeDest = true;
                   aluOp = ADD;
                   destReg = rd;
				   stats.registerDest(destReg);
                   aluSrc1 = lo;
				   stats.registerSrc(REG_HILO);
                   aluSrc2 = regFile[REG_ZERO];
				   stats.registerSrc(REG_ZERO);
                   break;
        case 0x18: D(cout << "mult " << regNames[rs] << ", " << regNames[rt]);
                   opIsMultDiv = true;
                   writeDest = false;
                   aluOp = MUL;
				   stats.registerDest(REG_HILO);
                   aluSrc1 = regFile[rs];
				   stats.registerSrc(rs);
                   aluSrc2 = regFile[rt];
				   stats.registerSrc(rt);
                   break;
        case 0x1a: D(cout << "div " << regNames[rs] << ", " << regNames[rt]);
                   writeDest = false;
                   opIsMultDiv = true;
                   aluOp = DIV;
				   stats.registerDest(REG_HILO);
                   aluSrc1 = regFile[rs];
				   stats.registerSrc(rs);
                   aluSrc2 = regFile[rt];
				   stats.registerSrc(rt);
                   break;
        case 0x21: D(cout << "addu " << regNames[rd] << ", " << regNames[rs] << ", " << regNames[rt]);
                   writeDest = true;
                   aluOp = ADD;
                   destReg = rd;
				   stats.registerDest(destReg);
                   aluSrc1 = regFile[rs];
				   stats.registerSrc(rs);
                   aluSrc2 = regFile[rt];
				   stats.registerSrc(rt);
                   break;
        case 0x23: D(cout << "subu " << regNames[rd] << ", " << regNames[rs] << ", " << regNames[rt]);
                   writeDest = true;
                   aluOp = ADD;
                   destReg = rd;
				   stats.registerDest(destReg);
                   aluSrc1 = regFile[rs];
				   stats.registerSrc(rs);
                   aluSrc2 = -regFile[rt];
				   stats.registerSrc(-rt);
                   break;
        case 0x2a: D(cout << "slt " << regNames[rd] << ", " << regNames[rs] << ", " << regNames[rt]);
                   writeDest = true;
                   aluOp = CMP_LT;
                   destReg = rd;
				   stats.registerDest(destReg);
                   aluSrc1 = regFile[rd];
				   stats.registerSrc(rd);
                   aluSrc2 = regFile[rt];
				   stats.registerSrc(rt);
                   break;
        default: cerr << "unimplemented instruction: pc = 0x" << hex << pc - 4 << endl;
      }
      break;
    case 0x02: D(cout << "j " << hex << ((pc & 0xf0000000) | addr << 2)); // P1: pc + 4
               writeDest = false;   // do not write registers
			   stats.registerDest(REG_ZERO);
			   stats.registerSrc(REG_ZERO);
			   stats.registerSrc(REG_ZERO);
               pc = (pc & 0xf0000000) | addr << 2; // save address field in pc to jump to the location
			   stats.flush(2);
               break;
    case 0x03: D(cout << "jal " << hex << ((pc & 0xf0000000) | addr << 2)); // P1: pc + 4
               writeDest = true; destReg = REG_RA; // writes PC+4 to $ra
               aluOp = ADD; // ALU should pass pc thru unchanged
			   stats.registerDest(destReg);
               aluSrc1 = pc;
			   stats.registerSrc(REG_ZERO);
               aluSrc2 = regFile[REG_ZERO]; // always reads zero
			   stats.registerSrc(REG_ZERO);
               pc = (pc & 0xf0000000) | addr << 2;
			   stats.flush(2);
               break;
    case 0x04: D(cout << "beq " << regNames[rs] << ", " << regNames[rt] << ", " << pc + (simm << 2));
			   stats.registerDest(REG_ZERO);
			   stats.registerSrc(rs);
			   stats.registerSrc(rt);
               if(regFile[rs]==regFile[rt])
               {
                  pc = pc + (simm << 2);
				  stats.flush(2);
				  stats.countTaken();
               }
			   stats.countBranch();
               break;
    case 0x05: D(cout << "bne " << regNames[rs] << ", " << regNames[rt] << ", " << pc + (simm << 2));
			   stats.registerDest(REG_ZERO);
			   stats.registerSrc(rs);
			   stats.registerSrc(rt);
               if(regFile[rs]!=regFile[rt])
               {
                   pc = pc + (simm << 2);
				   stats.flush(2);
				   stats.countTaken();
               }
			   stats.countBranch();
               break;
    case 0x09: D(cout << "addiu " << regNames[rt] << ", " << regNames[rs] << ", " << dec << simm);
               writeDest = true;
               aluOp = ADD;
               destReg = rt;
			   stats.registerDest(destReg);
               aluSrc1 = regFile[rs];
			   stats.registerSrc(rs);
               aluSrc2 = simm;
			   stats.registerSrc(REG_ZERO);
               break;
    case 0x0c: D(cout << "andi " << regNames[rt] << ", " << regNames[rs] << ", " << dec << uimm);
               writeDest = true;
               aluOp = AND;
               destReg = rt;
			   stats.registerDest(destReg);
               aluSrc1 = regFile[rs];
			   stats.registerSrc(rs);
               aluSrc2 = uimm;
			   stats.registerSrc(REG_ZERO);
               break;
    case 0x0f: D(cout << "lui " << regNames[rt] << ", " << dec << simm);
               writeDest = true;
               aluOp = SHF_L;
               destReg = rt;
			   stats.registerDest(destReg);
               aluSrc1 = simm;
			   stats.registerSrc(REG_ZERO);
               aluSrc2 = 16;
			   stats.registerSrc(REG_ZERO);
               break;
    case 0x1a: D(cout << "trap " << hex << addr);
               switch(addr & 0xf) {
                 case 0x0: cout << endl; break;
                 case 0x1: cout << " " << (signed)regFile[rs];
				           stats.registerSrc(rs);
						   stats.registerSrc(REG_ZERO);
                           break;
                 case 0x5: cout << endl << "? "; cin >> regFile[rt];
				           stats.registerDest(rt);
                           break;
                 case 0xa: stop = true; break;
                 default: cerr << "unimplemented trap: pc = 0x" << hex << pc - 4 << endl;
                          stop = true;
               }
               break;
    case 0x23: D(cout << "lw " << regNames[rt] << ", " << dec << simm << "(" << regNames[rs] << ")");
               opIsLoad = true;
               writeDest = true;
               aluOp = ADD;
               destReg = rt;
			   stats.registerDest(destReg);
               aluSrc1 = regFile[rs];
			   stats.registerSrc(rs);
               aluSrc2 = simm;
			   stats.registerSrc(REG_ZERO);
			   stats.countMemOp();
               break;
    case 0x2b: D(cout << "sw " << regNames[rt] << ", " << dec << simm << "(" << regNames[rs] << ")");
               opIsStore = true;
               writeDest = false;
			   stats.registerDest(REG_ZERO);
               storeData = regFile[rt];
               aluOp = ADD;
               aluSrc1 = regFile[rs];
			   stats.registerSrc(rs);
               aluSrc2 = simm;
			   stats.registerSrc(rt);
			   stats.countMemOp();
               break;
    default: cerr << "unimplemented instruction: pc = 0x" << hex << pc - 4 << endl;
               destReg = rt;
               break;
  }
  D(cout << endl);
}

void CPU::execute() {
  aluOut = alu.op(aluOp, aluSrc1, aluSrc2);
}

void CPU::mem() {
  if(opIsLoad)
    writeData = dMem.loadWord(aluOut);
  else
    writeData = aluOut;

  if(opIsStore)
    dMem.storeWord(storeData, aluOut);
}

void CPU::writeback() {
  if(writeDest && destReg > 0) // skip if write is to reg 0
    regFile[destReg] = writeData;
  
  if(opIsMultDiv) {
    hi = alu.getUpper();
    lo = alu.getLower();
  }
}

void CPU::printRegFile() {
  cout << hex;
  for(int i = 0; i < NREGS; i++) {
    cout << "    " << regNames[i];
    if(i > 0) cout << "  ";
    cout << ": " << setfill('0') << setw(8) << regFile[i];
    if( i == (NREGS - 1) || (i + 1) % 4 == 0 )
      cout << endl;
  }
  cout << "    hi   : " << setfill('0') << setw(8) << hi;
  cout << "    lo   : " << setfill('0') << setw(8) << lo;
  cout << dec << endl;
}

void CPU::printFinalStats() {
  cout << "Program finished at pc = 0x" << hex << pc << "  ("
       << dec << instructions << " instructions executed)" << endl;
  cout << endl;
  cout << "Cycles: " << stats.getCycles() << endl;
  cout << "CPI: " << fixed << setprecision(2) << (stats.getCycles()/static_cast<double>(instructions)) << endl;
  cout << endl;
  cout << "Bubbles: " << stats.getBubbles() << endl;
  cout << "Flushes: " << stats.getFlushes() << endl;
  cout << endl;
  cout << "Mem ops: " << fixed << setprecision(1) << 100.0 * stats.getMemOps()/instructions << "%" << " of instrctions" << endl;
  cout << "Branches: " << fixed << setprecision(1) << 100.0 * stats.getBranches()/instructions << "%" << " of instructions" << endl;
  cout << " %" << " Taken: " << fixed << setprecision(1) << 100.0 * stats.getTaken()/stats.getBranches() << endl;  
}
