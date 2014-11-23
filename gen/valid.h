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

	bool testCode(Tree*, string&, string&);
	void dclsCode(Tree*);
	void statementsCode(Tree*);

	void lvalueCode(const string&, Tree*);
	void ampLvalueCode(const string&, Tree*);
	void starFactorCode(const string&, Tree*);

	Args getLvalue(Tree*);
	Args getAmpLvalue(Tree*);
	Args getStarFactor(Tree*);
	Args getFactor(Tree*);
	Args getExpr(Tree*);

	bool isInt(Tree*);

	void exprCode(const string&, Tree*);

	void ifCode(Tree*);
	void loopCode(Tree*);

	string makeIf();
	string makeLoop();
	string makeTemp();
	string makePTemp();

}

#endif