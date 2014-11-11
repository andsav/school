#include "Mips.h"

Mips::Mips(procedures& p, locTable& l, int offset) 
	: p(p), l(l), offset(offset) {

	this->prologue();
	this->genCode();
	this->epilogue();
}

void Mips::genComment(Instr& instr) {
	this->out << ";" << instr;
	this->out << endl;
}

void Mips::prologue() {
	this->offset++;
	this->out << ".import print" << endl << endl
			  << "sw $31, -" << (4*this->offset) << "($30)"<< endl
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
    			  << ".word " << (4*this->offset == 4) << endl
    			  << "sub $30, $30, $4" << endl
    			  << "lis $4" << endl 
    			  << ".word 4" << endl;
    }

    this->out << endl << ";;;;;;; END PROLOGUE ;;;;;;;" << endl << endl;
}

void Mips::epilogue() {
	this->out << ";;;;;;; BEGIN EPILOGUE ;;;;;;;" << endl
			  << "lw $31, -4($29)" << endl << "jr $31" << endl
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
			break;
			case 'P' :
				if(instr.var != "$1") {
					//this->is(string("$1"), instr.var);
				}
				this->out << "jalr $10" << endl;
			break;
			default :
			break;
		}
	}
}

ostream& operator<<(ostream& out, Mips& m) {
	out << m.out.rdbuf();
}