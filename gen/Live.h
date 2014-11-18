#ifndef LIVE_H
#define LIVE_H

#include "Graph.h"

class Live {
private:
	functions& f;
	table& symbols;

	string current;

	void genState(string);
	void genState();
public:
	Live(functions&, table&);

};

#endif