#ifndef LIVE_H
#define LIVE_H

#include "Graph.h"

class Live {
private:
	functions& f;
	table& symbols;

	string current;
	string currentSymbol;
	bool change;

	void genState(Graph*, bool);
	void genLive(Graph*, bool);

	void gen(string);

	void fold(Graph*, bool);
	void clear(Graph*, bool);

public:
	Live(functions&, table&);

};

#endif