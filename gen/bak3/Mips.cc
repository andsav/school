#include "Mips.h"

Mips::Mips(procedures& p, locTable& l, int offset) 
	: p(p), l(l), offset(offset) {

	this->prologue();
	this->genCode();
	this->epilogue();
}

void Mips::genComment(Instr& instr) {
	this->out << endl << ";" << instr;
}

void Mips::prologue() {
	this->offset += 2;
	this->out << ".import print" << endl << endl
			  << "sw $31, -" << (4*this->offset-4) << "($30)"<< endl
			  << "add $29, $30, $0" << endl
			  << "lis $10" << endl << ".word print" << endl
			  << "lis $11" << endl << ".word 1" << endl;

    if(4*this->offset == 4) {
    	this->out << "lis $4" << endl 
    			  << ".word 4" << endl
    			  << "sub $30, $30, $4";
    }
    else {
    	this->out << "lis $4" << endl 
    			  << ".word " << (4*this->offset) << endl
    			  << "sub $30, $30, $4" << endl
    			  << "lis $4" << endl 
    			  << ".word 4" << endl;
    }

    this->out << endl << ";;;;;;; END PROLOGUE ;;;;;;;" << endl << endl;
}

void Mips::epilogue() {
	this->out << ";;;;;;; BEGIN EPILOGUE ;;;;;;;" << endl
			  << "lis $4" << endl 
    		  << ".word " << (4*this->offset) << endl
    	      << "add $30, $30, $4" << endl
			  << "lw $31, -" << (4*this->offset-4) << "($30)" << endl << "jr $31" << endl
			  << ";;;;;;;; END EPILOGUE ;;;;;;;;" << endl;
}

void Mips::genCode() {
	this->out << ";;;;;;; BEGIN CODE ;;;;;;;" << endl;
	FOREACH_PROCEDURE(this->genCode);
	this->out << ";;;;;;;; END Code ;;;;;;;;" << endl << endl << endl;
}

void Mips::genCode(string current) {
	this->out << "P" << current << ":" << endl << endl;
	this->current = current;

	for(int i=0; i<this->p[current].size(); ++i) {
		Instr& instr = this->p[current][i];

		this->genComment(instr);
		switch(instr.cmd) {
			case '=' :
				if(instr.args.size() == 1) {
					this->is(instr.var, instr.args[0]);
				}
				else {
					this->operation(instr);
				}
			break;
			case 'P' :
				if(instr.var != "$1") {
					this->is(string("$1"), instr.var);
				}
				this->out << "jalr $10" << endl;
			break;

			case 'I' :
				this->genTest(instr.args, instr.var, string("else" + instr.var));
				this->out << "begin" << instr.var << ": " << endl;
			break;

			case 'e' :
				this->out << "beq $0, $0, end" << instr.var << endl
						  << "else" << instr.var << ": " << endl;
			break;

			case 'i' :
				this->out << "end" << instr.var << ": " << endl;
			break;

			case 'W' :
				this->out << "test" << instr.var << ": " << endl;

			break;

			case 'T' :
				this->genTest(instr.args, instr.var, string("end" + instr.var));
				this->out << "begin" << instr.var << ": " << endl;
			break;

			case 'w':
				this->out << "beq $0, $0, test" << instr.var << endl
						  << "end" << instr.var << ": " << endl;
	
			break;

			default :
			break;
		}
	}
}

void Mips::genTest(vector<string>& t, string id, string go) {
	int a = this->getLocation(t[0]);
	int b = this->getLocation(t[2]);
	if(a < 0) {
		this->out << "lw $5, " << a << "($29)" << endl;
		a = 5;
	}
	if(b < 0) {
		this->out << "lw $6, " << b << "($29)" << endl;
		b = 6;
	}

	if(t[1] == "==") {
		this->out << "bne $" << a << ", $" << b << ", " << go << endl;
	}
	else if(t[1] == "!=") {
		this->out << "beq $" << a << ", $" << b << ", " << go << endl;
	}
	else if(t[1] == "<") {
		this->out << "slt $7, $" << a << ", $" << b << endl
				  << "beq $7, $0, " << go << endl;
	}
	else if(t[1] == "<=") {
			this->out << "beq $" << a << ", $" << b << ", begin" << id << endl
					  << "slt $7, $" << a << ", $" << b << endl
				  	  << "beq $7, $0, " << go << endl;
	}
	else if(t[1] == ">") {
		this->out << "beq $" << a << ", $" << b << ", " << go << endl
				  << "slt $7, $" << a << ", $" << b << endl
			  	  << "bne $7, $0, " << go << endl;
	}
	else if(t[1] == ">=") {
		this->out << "slt $7, $" << a << ", $" << b << endl
				  << "bne $7, $0, " << go << endl;
	}
}

void Mips::operation(Instr& instr) {
	int a = this->getLocation(instr.args[0]);
	int b = this->getLocation(instr.args[2]);
	int var = this->getLocation(instr.var);

	if(a < 0) {
		this->out << "lw $5, " << a << "($29)" << endl;
		a = 5;
	}
	if(b < 0) {
		this->out << "lw $6, " << b << "($29)" << endl;
		b = 6;
	}
	if(var < 0) {
		var = 7;
	}

	if(instr.args[1] == "+") {
		this->out << "add $" << var << ", $" << a << ", $" << b << endl;
	}
	else if(instr.args[1] == "-") {
		this->out << "sub $" << var << ", $" << a << ", $" << b << endl;
	}
	else if(instr.args[1] == "*") {
		this->out << "mult $" << a << ", $" << b << endl
				  << "mflo $" << var << endl;
	}
	else if(instr.args[1] == "/") {
		this->out << "div $" << a << ", $" << b << endl
				  << "mflo $" << var << endl;
	}
	else if(instr.args[1] == "%") {
		this->out << "div $" << a << ", $" << b << endl
				  << "mfhi $" << var << endl;
	}

	if(var == 7) {
		this->out << "sw $7, " << this->getLocation(instr.var) << "($29)" << endl;
	}
}

void Mips::is(string a, string b) {
	if(isReg(a)) {
		if(isReg(b)) {
			this->out << "add " << a << ", $0, " << b << endl;
		}
		else if(isNum(b)) {
			this->out << "lis " << a << endl
					  << ".word " << b << endl;
		}
		else {
			if(this->isStored(b)) {
				this->out << "lw " << a << ", " << this->getLocation(b) << "($29)" << endl;
			}
			else {
				this->out << "add " << a << ", $0, $" << this->getLocation(b) << endl;
			}
		}
	}
	else {
		if(isReg(b)) {
			if(this->isStored(a)) {
				this->out << "sw " << b << ", " << this->getLocation(a) << "($29)" << endl;
			}
			else {
				this->out << "add $" << this->getLocation(a) << ", $0, " << b << endl;
			}
		}
		else if(isNum(b)) {
			if(this->isStored(a)) {
				this->out << "lis $5" << endl
						  << ".word " << b << endl
						  << "sw $5, " << this->getLocation(a) << "($29)" << endl;
			}
			else {
				this->out << "lis $" << this->getLocation(a) << endl
						  << ".word " << b << endl;

			}
		}
		else {
			if(this->isStored(a)) {
				if(this->isStored(b)) {
					this->out << "lw $5, " << this->getLocation(b) << "($29)" << endl 
				  	  	  	  << "sw $5, " << this->getLocation(a) << "($29)" << endl;
			
				}
				else {
					this->out << "sw $" << this->getLocation(b) << ", " << this->getLocation(a) << "($29)" << endl;
				}
			}
			else {
				if(this->isStored(b)) {
					this->out << "lw $5, " << this->getLocation(b) << "($29)" << endl 
				  	  	  	  << "add $" << this->getLocation(a) << ", $0, $5" << endl;
				}
				else {
					this->out << "add $" << this->getLocation(a) << ", $0, $" << this->getLocation(b) << endl;
				}
			}
		}
	}
}

// Azucar
bool Mips::isStored(string& s) {
	return (this->getLocation(s) < 0);
}

int Mips::getLocation(string &s) {
	if(isReg(s)) {
		stringstream ss(s.substr(1));
		int num;
		ss >> num;
		return num;
	}
	return this->l[this->current][s];
}

ostream& operator<<(ostream& out, Mips& m) {
	out << m.out.rdbuf();
}