#ifndef DEF_H
#define DEF_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <deque>
#include <algorithm>

using namespace std;

#define FOREACH(obj) for(int i=0; i<obj.size(); ++i)
#define FOREACH_(obj, i) for(int i=0; i<obj.size(); ++i)
#define APPEND(v1, v2) v1.insert(v1.end(), v2.begin(), v2.end())

enum stateType 	{ TOP, K, BOT };

struct State;
struct Symbol;
struct Args;
struct Instr;
struct Procedure;
struct Tree;

struct State {
	bool 		live;
	stateType 	t;
	string 		val;

	State();
	State(stateType);
	State(stateType, string);

	State lub(const State&);

	bool operator==(const State&) const;
	bool operator!=(const State&) const;
};

struct Symbol {	
	Procedure*		proc;

	string 			name;
	bool 			isInt;
	bool			isArg;

	int 			loc;
	pair<int, int> 	def;
	pair<int, int>	use;
	vector<int> 	usedRegs;

	Symbol(string);
	Symbol(string, string);
	Symbol(string, bool);
};

struct Args {
	string		var1;
	char		cmd;
	string		var2;

	Args();
	Args(string&);
	Args(string&, char);
	Args(string, char, string);

	int size();

	friend ostream& operator<<(ostream&, Args&);
};

struct Instr {
	Procedure*		proc;

	string 			var;
	char 			cmd;
	Args 			args;

	vector<Instr*> 	in;
	vector<Instr*> 	out;

	// At INSTR, SYMBOL flows from STATE to STATE
	//map<Symbol*, pair<State, State> > stateTable;

	// At INSTR, is SYMBOL alive?
	//map<Symbol*, pair<bool, bool> > liveTable;

	bool empty();

	Instr();
	Instr(string, char, string = "");
	Instr(string, char, Args);
	Instr(char, string);
	Instr(char, Args);
	
	void del();

	friend ostream& operator<<(ostream&, Instr&);
};

struct Procedure {
	string									name;
	vector<Symbol*>							symbols;
	vector<Instr*>							instr;
	vector<vector<pair<State, State> > >	stateTable;

	map<string, Symbol*>					symbolsTable;
	vector<string>							symbolsList;

	vector<int>	usedRegs;

	int offset;

	Procedure();
	Procedure(string);
	~Procedure();

	void addSymbol(Symbol*);
	void addInstr(Instr*);
	void addInstr(Instr*, Instr*);
	void addInstr(Instr*, Instr*, Instr*);

	friend ostream& operator<<(ostream&, Procedure&);
	friend struct sortSymbol;
};


enum sortType { FIRST_DEF, LAST_DEF, FIRST_USE, LAST_USE };

struct sortSymbol {
	Procedure* p;
	sortType sort;

	sortSymbol(Procedure*, sortType);
	
	bool operator() (const string&, const string&);
};


struct Tree {
	string			lhs;
	vector<string>	rhs;
	string			type;

	vector<Tree*>	children;

	Tree();
	Tree(string&);
	Tree(string&, Tree*);
	Tree(string&, vector<Tree*>);
	~Tree();

	void init(string&);

	string toString();
	void printTree(ostream&, int);
	friend ostream& operator<<(ostream&, Tree&);

	bool operator==(const char*) const;
	bool operator!=(const char*) const;
};


// Global stuff because yolo

bool isNum(const string&);
bool isReg(const string&);
bool isVar(const string&);

extern deque<Procedure> program;
extern Procedure* current;

extern Tree parseTree;

extern int tempCount, ifCount, loopCount;

extern stringstream code;

extern bool isPrint, isInit, twoInts;

#endif