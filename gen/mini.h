#ifndef MINI_H
#define MINI_H

#include "valid.h"

struct Instr {
	string var;
	char cmd;
	vector<string> args;

	Instr(string, char, string = "");
	Instr(char, string);
	Instr();

	void splitArgs(const string&);

	bool empty();

	friend ostream& operator<<(ostream&, Instr&);
};

typedef map<string, vector<Instr> > procedures;

/**
* Generates a sequence of Mini instructions
______________________________________________________________

a = $i 							#### COPY contents of $i at location of a

$i = a 							#### COPY conents of a at register $i
a  = b							#### COPY contents of b at location of a

$i = 5							#### PUT 5 in register $i
a  = 5							#### PUT 5 at location of a

a  = b [+|-|*|/|%] c			#### PUT result of operation at location of a
_____________________________________________________________

a P 							#### PRINT contents of a
_____________________________________________________________

1 I a [EQ|NE|LT|LE|GE|GT] b		#### I1: if(!test) goto e1
e 1								#### e1: if(test) goto i1
i 1								#### i1:

1 W a [EQ|NE|LT|LE|GE|GT] b		#### W1: if(!test) goto w1 
w 1								#### w1: goto W1
_____________________________________________________________

a N b							#### Return to b, beginning of a contiguous sequence of a words of memory
a D 							#### Assert a is returned from N; frees assignedblock of memory
_____________________________________________________________

*/

class Mini {

friend ostream& operator<<(ostream&, Mini&);

private:
	string current; 						// The current procedure (prevents needing to pass it around)
	int loopCount, ifCount;					// current loop and conditional id
	map<string, int> tempCount;

	Tree &parseTree;
	vector<string>& order;
	table &symbols;

	void genCode(Tree&);					// entry point, procedures, wain

	vector<Instr> exprCode(string, Tree&);	// expr, term, factor
	vector<Instr> statementsCode(Tree&);	// statements

	vector<Instr> dclsCode(Tree&);			// dcls
	string getLvalue(Tree&);				// lvalue
	string getDcl(Tree&);					// dcl

	vector<Instr> loopCode(Tree&);						// statement if
	vector<Instr> ifCode(Tree&);						// statement while
	vector<Instr> testCode(Tree&, string&, string&);	// test

	vector<Instr> newCode(Tree&);			// factor new
	vector<Instr> deleteCode(Tree&);		// statement delete

	string makeTemp();
	string makeIf();
	string makeLoop();

	procedures code;

public:
	Mini(Tree&, table&, vector<string>&);
	procedures* getCode();
	~Mini();

};

#endif