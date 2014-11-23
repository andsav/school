#ifndef MIPS_H
#define MIPS_H

#include "Loc.h"

namespace Mips {
	void genCode();

	void prologue();
	void body();
	void epilogue();

	string whatIs(string&, string);

	void operation(Instr*);
	void is(string, string);
	void genTest(Args&, string, string);
	void pointersFun(Instr*);

	void store(string&, Args&);

	bool isPointer(string&);
	bool isStored(string&);
	int getLocation(string&);
}

#endif