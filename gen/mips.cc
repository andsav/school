#include "Mips.h"

stringstream code;
int whereIs31;

void Mips::genCode() {
	prologue();
	body();
	epilogue();
}

void Mips::prologue() {
	offset += 2;
	whereIs31 = 4*offset-4;
	code << ".import print" << endl
		 << "lis $10" << endl << ".word print" << endl
		 << "sw $31, -" << whereIs31 << "($30)"<< endl
		 << "add $29, $30, $0" << endl
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
					else if(instr->args.size() == 2) {
						pointersFun(instr);
					}
					else {
						operation(instr);
					}
				break;
				case '@' :
					store(instr->var, instr->args);
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
		 << "lw $31, -" << whereIs31 << "($30)" << endl << "jr $31" << endl;
}

string Mips::whatIs(string& a, string alt) {
	if(!isVar(a) || isStored(a)) {
		is(alt, a);
		return alt;
	}
	stringstream ss;
	ss << getLocation(a);
	return string("$" + ss.str());
}

void Mips::store(string& a, Args& b) {
	string temp1 = whatIs(a, string("$5"));
	string temp2 = whatIs(b.var1, string("$6"));

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
				++offset;
				loc = -offset*4;

				code << "sub $30, $30, $4" << endl
					 << "sw $" << getLocation(instr->args.var1) << ", " << loc << "($29)" << endl;
				
				current->symbolsTable[instr->args.var1]->loc = loc;
			}

			ss << loc;

			is(string("$5"), ss.str());
			code << "add $5, $29, $5" << endl;
			
			is(instr->var, string("$5"));
		
		break;
		case '@' :
			is(string("$5"), instr->args.var1);
			code << "lw $5, 0($5)" << endl;
			is(instr->var, string("$5"));

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

bool Mips::isPointer(string& s) {
	if(s[0] == 'p')
		return 1;

	if(!isVar(s))
		return 0;
	
	if(!current->symbolsTable[s]->isInt)
		return 1;

	return 0;
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
