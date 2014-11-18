#ifndef SPLIT_H
#define SPLIT_H

#include "Mini.h"

enum tt { TOP, K, BOT };

struct State {
	tt t;
	string val;

	State();
	State(tt);
	State(tt, string);

	State lub(const State&);

	bool operator==(const State&) const;
	bool operator!=(const State&) const;
};

struct Graph {
	bool flag;
	Instr instr;
	Graph* last;

	// At INSTR, SYMBOL flows from STATE to STATE
	map<string, pair<State, State> > stateTable;

	// At INSTR, is SYMBOL alive?
	map<string, pair<bool, bool> > liveTable;

	vector<Graph*> in;
	vector<Graph*> out;

	Graph();
	Graph(Graph*);
	Graph(Graph*, Graph*);
	~Graph();

	void del();

	vector<Instr*> getFlat();
};

typedef map<string, Graph> functions;

class Split {
private:	
	procedures& p;
	functions f;
	table& symbols;

	string current;
	Graph* currentGraph;

	void gen(vector<Instr>&);
	void gen(string);

	Graph* makeGraph(Graph*);
	Graph* makeGraph(Graph*, Graph*);

	stack<Graph*> pile;

public:
	Split(procedures&, table&);
	~Split();
	functions* getFunctions();
};

#endif