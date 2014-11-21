#include "Mips.h"

stringstream code;

void Mips::genCode() {
	prologue();
	body();
	epilogue();
}

void Mips::prologue() {
	offset += 2;
	code << ".import print" << endl
		 << "sw $31, -" << (4*offset-4) << "($30)"<< endl
		 << "add $29, $30, $0" << endl
		 << "lis $10" << endl << ".word print" << endl
		 << "lis $11" << endl << ".word 1" << endl
		 << "lis $4" << endl 
		 << ".word " << (4*offset) << endl
	     << "sub $30, $30, $4" << endl
		 << "lis $4" << endl 
		 << ".word 4" << endl
		 << ";;;;;;; END PROLOGUE ;;;;;;;" << endl << endl;
}

void Mips::body() {
	Instr* instr;
	FOREACH(program) {
		current = &program[i];

		code << "P" << current->name << ":" << endl;
		FOREACH_(current->instr, j) {
			instr = current->instr[j];
			code << endl << "; " << *instr;

			switch(instr->cmd) {
				case '=' :
					if(instr->args.size() == 1) {
						is(instr->var, instr->args.var1);
					}
					else {
						operation(instr);
					}
				break;
				case 'P' :
					if(instr->var != "$1") {	// Print takes in $1
						is(string("$1"), instr->var);
					}
					code << "jalr $10" << endl;
				break;

				case 'I' :
					genTest(instr->args, instr->var, string("else" + instr->var));
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

				case 'w':
					code << "beq $0, $0, test" << instr->var << endl
							  << "end" << instr->var << ": " << endl;
		
				break;

				default :
				break;
			}
		}
	}
}

void Mips::epilogue() {
	code << endl << ";;;;;;; BEGIN EPILOGUE ;;;;;;;" << endl
		 << "lis $4" << endl 
		 << ".word " << (4*offset) << endl
	     << "add $30, $30, $4" << endl
		 << "lw $31, -" << (4*offset-4) << "($30)" << endl << "jr $31" << endl;
}

void Mips::operation(Instr* instr) {
	int a = getLocation(instr->args.var1);
	int b = getLocation(instr->args.var2);
	int var = getLocation(instr->var);

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

	if(var == 7) {
		code << "sw $7, " << getLocation(instr->var) << "($29)" << endl;
	}
}
void Mips::is(string a, string b) {
	if(isReg(a)) {
		if(isReg(b)) {
			code << "add " << a << ", $0, " << b << endl;
		}
		else if(isNum(b)) {
			code << "lis " << a << endl
				 << ".word " << b << endl;
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
				code << "lis $5" << endl
					 << ".word " << b << endl
					 << "sw $5, " << getLocation(a) << "($29)" << endl;
			}
			else {
				code << "lis $" << getLocation(a) << endl
				     << ".word " << b << endl;

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
					code << "lw $5, " << getLocation(b) << "($29)" << endl 
				  	  	 << "add $" << getLocation(a) << ", $0, $5" << endl;
				}
				else {
					code << "add $" << getLocation(a) << ", $0, $" << getLocation(b) << endl;
				}
			}
		}
	}
}
void Mips::genTest(Args& test, string id, string go) {
	int a = getLocation(test.var1);
	int b = getLocation(test.var2);

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
			code << "slt $7, $" << a << ", $" << b << endl
				 << "beq $7, $0, " << go << endl;
		break;
		case 'L' :
			code << "beq $" << a << ", $" << b << ", begin" << id << endl
				 << "slt $7, $" << a << ", $" << b << endl
				 << "beq $7, $0, " << go << endl;
		break;
		case '>' :
			code << "beq $" << a << ", $" << b << ", " << go << endl
				 << "slt $7, $" << a << ", $" << b << endl
			  	 << "bne $7, $0, " << go << endl;
		break;
		case 'G' :
			code << "slt $7, $" << a << ", $" << b << endl
				 << "bne $7, $0, " << go << endl;
		break;
	}
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
