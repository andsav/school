#include "Mips.h"

stringstream code;

void Mips::genCode() {
	Instr* instr;
	FOREACH(program) {
		current = &program[i];

		code << endl << endl << "P" << current->name << ":" << endl;
		
		if(current->name != "wain") {
			functionPrologue();
		}
		else {
			prologue();
		}

		FOREACH_(current->instr, j) {
			instr = current->instr[j];

			if(instr->empty()) continue;

			code << endl << "; " << *instr;

			switch(instr->cmd) {
				case '=' :
					if(instr->args.size() == 1) {
						is(instr->var, instr->args.var1);
					}
					else if(instr->args.cmd == 'K') {
						procedureCall(instr);
					}
					else if(instr->args.size() == 2) {
						pointersFun(instr);
					}
					else {
						if(instr->args.cmd == '#') {
							stringstream ss(instr->args.var2);
							int num;
							ss >> num;
							num *= 4;

							if(isReg(instr->var)) {
								code << "lw " << instr->var << ", " << num << "(" << instr->args.var1 << ")" << endl;
							}
							else if(isStored(instr->var)) {
								code << "lw $5, " << num << "(" << instr->args.var1 << ")" << endl
									 << "sw $5, " << getLocation(instr->var) << "($29)" << endl;
							}
							else {
								code << "lw $" << getLocation(instr->var) << ", " << num << "(" << instr->args.var1 << ")" << endl;
							}
						}
						else
							operation(instr);
					}
				break;
				case '@' :
					store(instr->var, instr->args);
				break;
				case 'P' :
					code << "sub $30, $30, $4" << endl;
					if(instr->var != "$1") {	// Print takes in $1
						is(string("$1"), instr->var);
					}
					code << "jalr $10" << endl
						 << "add $30, $30, $4" << endl;
				break;

				case 'I' :
					genTest(instr->args, instr->var, string("else" + instr->var));
					code << "begin" << instr->var << ": " << endl;
				break;

				case 'l' :
					genTest(instr->args, instr->var, string("else" + instr->var), 1);
					code << "begin" << instr->var << ": " << endl;
				break;

				case 'e' :
					code << "beq $0, $0, end" << instr->var << endl
						 << "else" << instr->var << ": " << endl;
				break;

				case 'i' :
					code << "end" << instr->var << ": " << endl;
				break;

				case 'W' :
					code << "test" << instr->var << ": " << endl;

				break;

				case 'T' :
					genTest(instr->args, instr->var, string("end" + instr->var));
					code << "begin" << instr->var << ": " << endl;
				break;

				case 't' :
					genTest(instr->args, instr->var, string("end" + instr->var), 1);
					code << "begin" << instr->var << ": " << endl;
				break;

				case 'w':
					code << "beq $0, $0, test" << instr->var << endl
						 << "end" << instr->var << ": " << endl;
		
				break;

				case 'N' :
					genNew(instr);
				break;

				case 'D' :
					genDelete(instr->var);
				break;


				default :
					throw string("Something bad happened");
				break;
			}
		}
		if(current->name == "wain") {
			epilogue();
		}
		else {
			functionEpilogue();
		}
	}
}

void Mips::prologue() {
	if(isPrint) {
		code << ".import print" << endl;
	}
	if(isInit) {
		code << ".import init" << endl 
			 << ".import new" << endl
			 << ".import delete" << endl;
	}

	code << "lis $4" << endl << ".word 8" << endl
		 << "sub $29, $30, $4" << endl
		 << "sw $31, 4($29)"<< endl;

	code << "lis $11" << endl << ".word 1" << endl
		 << "lis $4" << endl 
		 << ".word " << (4*current->symbols.size()) << endl
	     << "sub $30, $29, $4" << endl
		 << "lis $4" << endl 
		 << ".word 4" << endl;


	if(isPrint) {
		code << "lis $10" << endl << ".word print" << endl;
	}
	if(isInit) {
		code << "lis $12" << endl << ".word init" << endl;

		code << "sub $30, $30, $4" << endl;

		if(twoInts) {
			code << "add $5, $2, $0" << endl
				 << "add $2, $0, $0" << endl
				 << "jalr $12" << endl
				 << "add $2, $5, $0" << endl;
		}
		else {
			code << "jalr $12" << endl;
		}

		code << "add $30, $30, $4" << endl;

		code << "lis $12" << endl << ".word new" << endl
		 	 << "lis $13" << endl << ".word delete" << endl;
	}

	code << ";;;;;;; END PROLOGUE ;;;;;;;" << endl << endl;
}

void Mips::functionPrologue() {
	code << "lis $5" << endl << ".word 8" << endl
		 << "sub $29, $30, $5" << endl
		 << "sw $31, 4($29)"<< endl
		 << "lis $5" << endl 
		 << ".word " << (4*(current->symbols.size() + current->usedRegs.size()+1)) << endl
	     << "sub $30, $29, $5" << endl;

	FOREACH(current->usedRegs) {
		int ofst = 4*(current->symbols.size()+i+1);
		code << "sw $" << current->usedRegs[i] << ", -" << ofst << "($29)" << endl;
	}
}

void Mips::epilogue() {
	code << endl << ";;;;;;; BEGIN EPILOGUE ;;;;;;;" << endl
		 << "lw $31, 4($29)" << endl << "jr $31" << endl
		 << endl << ";;;;;;;; END EPILOGUE ;;;;;;;;" << endl;
}

void Mips::functionEpilogue() {
	FOREACH(current->usedRegs) {
		int ofst = 4*(current->symbols.size()+i+1);
		code << "lw $" << current->usedRegs[i] << ", -" << ofst << "($29)" << endl;
	}
	code << "lis $5" << endl 
		 << ".word 8" << endl
	     << "add $30, $29, $5" << endl
		 << "lw $31, 4($29)" << endl
		 << "jr $31";
}

void Mips::procedureCall(Instr* instr) {
	push(29);
	code << "lis $5" << endl
		 << ".word " << instr->args.var1 << endl
		 << "jalr $5" << endl;
	pop(29);

	if(instr->args.var2 != "0" && instr->args.var2 != "1" && instr->args.var2 != "2") {
		stringstream ss(instr->args.var2);
		int num;
		ss >> num;
		num *= 4;

		stringstream ss2;
		ss2 << num;

		is(string("$5"), ss2.str());
		code << "add $30, $30, $5" << endl;
	}
	
	is(instr->var, string("$3"));
}

string Mips::whatIs(string& a, string alt) {
	if(isReg(a)) {
		return a;
	}
	else if(!isVar(a) || isStored(a)) {
		is(alt, a);
		return alt;
	}
	else {
		stringstream ss;
		ss << getLocation(a);
		return string("$" + ss.str());
	}
}

void Mips::store(string& a, Args& b) {
	string temp1 = "$5";
	string temp2 = "$6";

	is(temp1, a);

	if(b.cmd == '#') {
		stringstream ss(b.var2);
		int num;
		ss >> num;
		num *= 4;
		
		code << "sw " << temp1 << ", " << num << "(" << b.var1 << ")" << endl;
		return;
	}

	is(temp2, b.var1);

	if(b.cmd == '@') {
		code << "lw " << temp2 << ", 0(" << temp2 << ")" << endl;
	}

	code << "sw " << temp2 << ", 0(" << temp1 << ")" << endl;
}


void Mips::pointersFun(Instr* instr) {
	stringstream ss;
	int loc;

	switch(instr->args.cmd) {
		case '&' :
			
			loc = getLocation(instr->args.var1);

			if(loc > 0) {
				++current->offset;
				loc = current->offset*-4;

				code << "sw $" << getLocation(instr->args.var1) << ", " << loc << "($29)" << endl;
				
				current->symbolsTable[instr->args.var1]->loc = loc;
			}

			ss << loc;

			is(string("$5"), ss.str());
			code << "add $5, $29, $5" << endl;
			
			is(instr->var, string("$5"));
		
		break;
		case '@' :
			if(isReg(instr->args.var1)) {
				if(isReg(instr->var)) {
					code << "lw " << instr->var << ", 0(" << instr->args.var1 << ")" << endl;
				}
				else if(!isStored(instr->var)) {
					code << "lw $" << getLocation(instr->var) << ", 0(" << instr->args.var1 << ")" << endl;
				}
				else {
					code << "lw $5, 0(" << instr->args.var1 << ")" << endl
						 << "sw $5," << getLocation(instr->var) << "($29)"<< endl;
				}
			}
			else if(!isStored(instr->args.var1)) {
				if(isReg(instr->var)) {
					code << "lw " << instr->var << ", 0($" << getLocation(instr->args.var1) << ")" << endl;
				}
				else if(!isStored(instr->var)) {
					code << "lw $" << getLocation(instr->var) << ", 0($" << getLocation(instr->args.var1) << ")" << endl;
				}
				else {
					code << "lw $5, 0($" << getLocation(instr->args.var1) << ")" << endl
						 << "sw $5," << getLocation(instr->var) << "($29)"<< endl;
				}
			}
			else {
				is(string("$8"), instr->args.var1);
				code << "lw $8, 0($8)" << endl;
				is(instr->var, string("$8"));
			}

		break;
	}
}

void Mips::operation(Instr* instr) {
	int a = getLocation(instr->args.var1);
	int b = getLocation(instr->args.var2);
	int var = getLocation(instr->var);
	bool flag = 0;

	if(a < 0) {
		code << "lw $5, " << a << "($29)" << endl;
		a = 5;
	}
	if(b < 0) {
		code << "lw $6, " << b << "($29)" << endl;
		b = 6;
	}
	if(var < 0) {
		var = 7;
		flag = 1;
	}

	switch(instr->args.cmd) {
		case '+':
			code << "add $" << var << ", $" << a << ", $" << b << endl;
		break;
		case '-' :
			code << "sub $" << var << ", $" << a << ", $" << b << endl;
		break;
		case '*' :
			code << "mult $" << a << ", $" << b << endl
				 << "mflo $" << var << endl;
		break;
		case '/' :
			code << "div $" << a << ", $" << b << endl
			 	 << "mflo $" << var << endl;
		break;
		case '%' :
			code << "div $" << a << ", $" << b << endl
			 	 << "mfhi $" << var << endl;
		break;
	}

	if(flag) {
		code << "sw $7, " << getLocation(instr->var) << "($29)" << endl;
	}
}
void Mips::is(string a, string b) {
	if(b == "NULL")
		b = "1";

	if(isReg(a)) {
		if(isReg(b)) {
			code << "add " << a << ", $0, " << b << endl;
		}
		else if(isNum(b)) {
			if(b == "1")
				code << "add " << a << ", $11, $0" << endl;
			else if(b == "0")
				code << "add " << a << ", $0, $0" << endl;
			else if(b == "4")
				code << "add " << a << ", $4, $0" << endl;
			else {
				code << "lis " << a << endl
					 << ".word " << b << endl;
			}
		}
		else {
			if(isStored(b)) {
				code << "lw " << a << ", " << getLocation(b) << "($29)" << endl;
			}
			else {
				code << "add " << a << ", $0, $" << getLocation(b) << endl;
			}
		}
	}
	else {
		if(isReg(b)) {
			if(isStored(a)) {
				code << "sw " << b << ", " << getLocation(a) << "($29)" << endl;
			}
			else {
				code << "add $" << getLocation(a) << ", $0, " << b << endl;
			}
		}
		else if(isNum(b)) {
			if(isStored(a)) {
				if(b == "1")
					code << "sw $11, " << getLocation(a) << "($29)" << endl;
				else if(b == "0")
					code << "sw $0, " << getLocation(a) << "($29)" << endl;
				else if(b == "4")
					code << "sw $4, " << getLocation(a) << "($29)" << endl;
				else {
					code << "lis $5" << endl
						 << ".word " << b << endl
						 << "sw $5, " << getLocation(a) << "($29)" << endl;
				}
			}
			else {
				if(b == "1")
					code << "add $" << getLocation(a) << ", $11, $0" << endl;
				else if(b == "0")
					code << "add $" << getLocation(a) << ", $0, $0" << endl;
				else if(b == "4")
					code << "add $" << getLocation(a) << ", $4, $0" << endl;
				else {
					code << "lis $" << getLocation(a) << endl
					     << ".word " << b << endl;
			 	}
			}
		}
		else {
			if(isStored(a)) {
				if(isStored(b)) {
					code << "lw $5, " << getLocation(b) << "($29)" << endl 
				  	  	 << "sw $5, " << getLocation(a) << "($29)" << endl;
			
				}
				else {
					code << "sw $" << getLocation(b) << ", " << getLocation(a) << "($29)" << endl;
				}
			}
			else {
				if(isStored(b)) {
					code << "lw $" << getLocation(a) << ", " << getLocation(b) << "($29)" << endl;
				}
				else {
					code << "add $" << getLocation(a) << ", $0, $" << getLocation(b) << endl;
				}
			}
		}
	}
}
void Mips::genTest(Args& test, string id, string go, bool pointers) {
	int a = getLocation(test.var1);
	int b = getLocation(test.var2);

	string slt = (pointers ? "sltu" : "slt");

	if(a < 0) {
		code << "lw $5, " << a << "($29)" << endl;
		a = 5;
	}
	if(b < 0) {
		code << "lw $6, " << b << "($29)" << endl;
		b = 6;
	}

	switch(test.cmd) {
		case '=' :
			code << "bne $" << a << ", $" << b << ", " << go << endl;
		break;
		case 'N' :
			code << "beq $" << a << ", $" << b << ", " << go << endl;
		break;
		case '<' :
			code << slt << " $7, $" << a << ", $" << b << endl
				 << "beq $7, $0, " << go << endl;
		break;
		case 'L' :
			code << "beq $" << a << ", $" << b << ", begin" << id << endl
				 << slt << " $7, $" << a << ", $" << b << endl
				 << "beq $7, $0, " << go << endl;
		break;
		case '>' :
			code << "beq $" << a << ", $" << b << ", " << go << endl
				 << slt << " $7, $" << a << ", $" << b << endl
			  	 << "bne $7, $0, " << go << endl;
		break;
		case 'G' :
			code << slt << " $7, $" << a << ", $" << b << endl
				 << "bne $7, $0, " << go << endl;
		break;
	}
}

void Mips::genNew(Instr* instr) {
	is(string("$1"), instr->args.var1);
	code << "sub $30, $30, $4" << endl
		 << "jalr $12" << endl
		 << "add $30, $30, $4" << endl
		 << "bne $3, $0, 1" << endl
		 << "add $3, $11, $0" << endl;
		 
	is(instr->var, string("$3"));
}

void Mips::genDelete(string& s) {
	is(string("$1"), s);
	code << "beq $11, $1, 3" << endl;
	code << "sub $30, $30, $4" << endl
		 << "jalr $13" << endl
		 << "add $30, $30, $4" << endl;
}

void Mips::push(int i) {
	code << "sw $" << i << ", -4($30)" << endl
		 << "sub $30, $30, $4" << endl;
}

void Mips::pop(int i) {
	code << "add $30, $30, $4" << endl
		 << "lw $" << i << ", -4($30)" << endl;
}

bool Mips::isStored(string& s) {
	return (getLocation(s) < 0);
}

int Mips::getLocation(string &s) {
	if(isReg(s)) {
		stringstream ss(s.substr(1));
		int num;
		ss >> num;
		return num;
	}
	return current->symbolsTable[s]->loc;
}
