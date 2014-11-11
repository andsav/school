#ifndef MIPS_H
#define MIPS_H

#include "Loc.h"

class Mips {

friend ostream& operator<<(ostream&, Mips&);

private:
	int 	offset;
	string 	current;

	procedures& 	p;
	locTable&		l;
	stringstream	out;

	void prologue();
	void epilogue();
	
	void genCode(string);
	void genCode();

	void genComment(Instr&);

public:
	Mips(procedures&, locTable&, int);

};

#endif