#ifndef LIVE-H
#define LIVE_H

#include "Graph.h"

/*
enum tt { TOP, K, BOT };

struct State {
	tt t;
	int val;

	State(tt);
	State(tt, int);
};
*/

/*
*	in PROCEDURE, SYMBOL, flows at INSTR from STATE to STATE
*	Feed to optimization
*/
//typedef map<string, map<string, vector<pair<State, State> > > > stateTable; 

/*
*	in PROCEDURE, SYMBOL is alive from INSTR# to INSTR#
*	Feed to register allocation
*/
//typedef map<string, map<string, pair<int, int> > > 	liveTable;

/*
class Live {
private:
	functions& f;
	table& symbols;

	string current;

	liveTable alive;
	stateTable state;

	State lub(State&, State&);

	void genState(string);
	void genState();
public:
	Live(functions&, table&);
	liveTable* getAlive();
	stateTable* getState();

};
*/

#endif