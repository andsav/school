#ifndef LIVE_H
#define LIVE_H

#include "Graph.h"

typedef map<string, map<string, pair<int, int> > > 	liveTable;

class Live {
private:
	functions& f;
	table& symbols;

	string current;
	string currentSymbol;

	liveTable live;
	map<string, vector<Graph*> > p;

	bool change;

	void genState(Graph*, bool);
	void genLive(Graph*, bool);

	void gen(string);

	void fold(Graph*, bool);
	void clear(Graph*, bool);

	void optimize(string);
	void finalize(string);

	void genProcedures(Graph*, bool);

public:
	Live(functions&, table&);

	map<string, vector<Graph*> >* getProcedures();
	liveTable* getLive();
};

#endif