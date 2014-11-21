#ifndef VALID_H
#define VALID_H

#include "Def.h"

namespace Valid {

	char operation(const string&);
	bool isTerminal(const string&);

	// stdin -> parseTree
	Tree* readTree();
	void genParseTree();

	// parseTree -> program
	void genCode(Tree*);
	void genCode();

	string getLvalue(Tree*);
	string getDcl(Tree*);

	void testCode(Tree*, string&, string&);
	void dclsCode(Tree*);
	void statementsCode(Tree*);
	void exprCode(const string&, Tree*);

	void ifCode(Tree*);
	void loopCode(Tree*);

	string makeIf();
	string makeLoop();
	string makeTemp();
	string makePTemp();

}

#endif