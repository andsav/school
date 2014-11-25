#ifndef MIPS_H
#define MIPS_H

#include "Loc.h"

namespace Mips {
	void genCode();

	void prologue();
	void epilogue();
	void functionPrologue();
	void functionEpilogue();

	string whatIs(string&, string);

	void genNew(Instr*);
	void genDelete(string&);

	void genTest(Args&, string, string, bool pointers = 0);
	void pointersFun(Instr*);

	void store(string&, Args&);
	void is(string, string);
	void operation(Instr*);

	void pop(int);
	void push(int);

	bool isStored(string&);
	int getLocation(string&);
}

#endif