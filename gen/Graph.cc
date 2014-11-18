#include "Graph.h" 

/*
*	STATE
*/

State::State() { }
State::State(tt t) : t(t) { }
State::State(tt t, int val) : t(t), val(val) { }

/*
*	GRAPH
*/

Graph::Graph() : flag(0) { }

Graph::Graph(Graph* origin) : flag(0) {
	this->in.push_back(origin);
}

Graph::Graph(Graph* origin1, Graph* origin2) : flag(0) {
	this->in.push_back(origin1);
	this->in.push_back(origin2);
}

Graph::~Graph() { }

/*
*	SPLIT
*/

Split::Split(procedures& p, table& symbols) : p(p), symbols(symbols) {
	FOREACH_PROCEDURE(this->gen);

}

void Split::gen(string current) {
	this->current = current;

	this->f[current] = Graph();
	this->currentGraph = &this->f[current];

	FOREACH_SYMBOL() {
		this->currentGraph->liveTable[it->first] = false;
		this->currentGraph->stateTable[it->first] = make_pair(State(TOP), State(BOT));
	}

	this->gen(this->p[current]);
}

void Split::gen(vector<Instr>& instr) {
	vector<Instr>::iterator begin, end;
	vector<Instr> subInstr;

	FOREACH(instr) {
		Instr& thisInstr = instr[i];
		this->currentGraph->instr = thisInstr;

		if(thisInstr.cmd == 'I') {
			++i;
			Graph* ifBlock = this->makeGraph(this->currentGraph);
			Graph* elseBlock = this->makeGraph(this->currentGraph);

			this->currentGraph = ifBlock;

			begin = instr.begin() + i;
			for(end = begin-1; end->var != thisInstr.var || end->cmd != 'e'; ++end) {
				++i;
			}
			++end;

			subInstr = vector<Instr>(begin, end);
			this->gen(subInstr);

			ifBlock = this->currentGraph;
			this->currentGraph = ifBlock;

			for(begin = end+1; end->var != thisInstr.var || end->cmd != 'i'; ++end) {
				++i;
			}
			++end;

			subInstr = vector<Instr>(begin, end);
			this->gen(subInstr);

			elseBlock = this->currentGraph;
			this->currentGraph = this->makeGraph(ifBlock, elseBlock);
		}
		else if(thisInstr.cmd == 'W') {
			++i;

			Graph* loopBlock = this->makeGraph(this->currentGraph);
			Graph* testBlock = this->makeGraph(loopBlock);

			loopBlock->out.push_back(testBlock);

			this->currentGraph = testBlock;

			begin = instr.begin() + i;
			for(end = begin-1; end->var != thisInstr.var || end->cmd != 'T'; ++end) {
				++i;
			}
			++end;

			subInstr = vector<Instr>(begin, end);
			this->gen(subInstr);

			testBlock = this->currentGraph;
			this->currentGraph = loopBlock;

			for(begin = end+1; end->var != thisInstr.var || end->cmd != 'w'; ++end) {
				++i;
			}
			++end;

			subInstr = vector<Instr>(begin, end);
			this->gen(subInstr);

			loopBlock = this->currentGraph;
			this->currentGraph = this->makeGraph(testBlock, loopBlock);
		}
		else if(i < instr.size()-1) {	
			this->currentGraph = this->makeGraph(this->currentGraph);
		}
	}
}

Graph* Split::makeGraph(Graph* origin) {
	Graph* newGraph = new Graph(origin);

	origin->out.push_back(newGraph);

	pile.push(newGraph);
	
	FOREACH_SYMBOL() {
		newGraph->liveTable[it->first] = false;
		newGraph->stateTable[it->first] = make_pair(State(BOT), State(BOT));
	}

	return newGraph;
}

Graph* Split::makeGraph(Graph* origin, Graph* origin2) {
	Graph* newGraph = new Graph(origin, origin2);

	origin->out.push_back(newGraph);
	origin2->out.push_back(newGraph);

	pile.push(newGraph);

	FOREACH_SYMBOL() {
		newGraph->liveTable[it->first] = false;
		newGraph->stateTable[it->first] = make_pair(State(BOT), State(BOT));
	}

	return origin->out.back();
}

functions* Split::getFunctions() {
	return &this->f;
}

Split::~Split() {
	while(!this->pile.empty()) {
		delete this->pile.top();
		this->pile.pop();
	}
}