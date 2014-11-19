#include "Live.h"

Live::Live(functions& f, table& symbols) : f(f), symbols(symbols) { 
	FOREACH_FUNCTION(gen);
	FOREACH_FUNCTION(finalize);

	FOREACH_SYMBOL() {
		cout << it->first << ": " << this->live[this->current][it->first].first << " - " << this->live[this->current][it->first].second << endl;
	}
}

void Live::gen(string current) {
	this->current = current;

	FOREACH_SYMBOL() {
		this->currentSymbol = it->first;
		do{
			this->change = 0;
			this->genState(&this->f[current], this->f[current].flag);
		}
		while(this->change == 1);
	}

	FOREACH_SYMBOL() {
		this->currentSymbol = it->first;
		this->fold(&this->f[current], this->f[current].flag);
	}

	FOREACH_SYMBOL() {
		this->currentSymbol = it->first;
		this->fold(&this->f[current], this->f[current].flag);
	}

	FOREACH_SYMBOL() {
		this->currentSymbol = it->first;
		do{
			this->change = 0;
			this->genLive(this->f[current].last, this->f[current].last->flag);
		}
		while(this->change == 1);
	}

	FOREACH_SYMBOL() {
		// Variable is alive from?
		this->live[current][this->currentSymbol] = make_pair(0, 0);

		this->currentSymbol = it->first;
		
		this->clear(&this->f[current], this->f[current].flag);
	}
}

void Live::genState(Graph* g, bool origin) {
	g->flag = !origin;

	pair<State, State> ret = g->stateTable[this->currentSymbol];
	
	if(ret.first.t != TOP) {
		// RULE 1,2,3,4
		State lub = State(BOT);
		FOREACH(g->in) {
			lub = lub.lub(g->in[i]->stateTable[this->currentSymbol].second);
		}
		ret.first = lub;
	}
	
	if(ret.first != g->stateTable[this->currentSymbol].first) {
		this->change = 1;
		g->stateTable[this->currentSymbol].first = ret.first;
	}

	if(ret.second.t != TOP) {
		// RULE 5
		if(g->stateTable[this->currentSymbol].first.t == BOT) {
			ret.second = State(BOT);
		}
		// RULE 6
		else if(g->instr.cmd == '=' && 
				g->instr.var == this->currentSymbol && 
				g->instr.args.size() == 1 &&
				!isReg(g->instr.args[0])) {

			ret.second = State(K, g->instr.args[0]);
		
		}
		// RULE 7
		else if(g->instr.cmd == '=' && 
				g->instr.var == this->currentSymbol) {
			
			ret.second = State(TOP);
		
		}
		// RULE 8
		else {
			ret.second = g->stateTable[this->currentSymbol].first;
		}
	}

	if(ret.second != g->stateTable[this->currentSymbol].second) {
		this->change = 1;
		g->stateTable[this->currentSymbol].second = ret.second;
	}

	FOREACH(g->out) {
		if(g->out[i]->flag != origin) continue;
		this->genState(g->out[i], origin);
	}
}

void Live::fold(Graph* g, bool origin) {
	g->flag = !origin;

	if(g->stateTable[this->currentSymbol].first.t == K) {
		FOREACH(g->instr.args) {
			if(g->instr.args[i] == this->currentSymbol) {
				g->instr.args[i] = g->stateTable[this->currentSymbol].first.val;
			}
		}
	}

	FOREACH(g->out) {
		if(g->out[i]->flag != origin) continue;
		this->fold(g->out[i], origin);
	}
}

void Live::genLive(Graph* g, bool origin) {
	g->flag = !origin;

	pair<bool, bool> ret = g->liveTable[this->currentSymbol];

	// Rule 1
	if(ret.second != true) {
		bool l = false;
		FOREACH(g->out) {
			l = l || g->out[i]->liveTable[this->currentSymbol].first;
		}
		ret.second = l;
	}

	if(ret.first != true) {
		// Rule 3
		if(g->instr.var == this->currentSymbol) {
			ret.first = false;
		}
		// Rule 4
		else {
			ret.first = ret.second;
		}

		// Rule 2
		FOREACH(g->instr.args) {
			if(g->instr.args[i] == this->currentSymbol) {
				ret.first = true;
				break;
			}
		}
	}

	if(ret != g->liveTable[this->currentSymbol]) {
		this->change = 1;
		g->liveTable[this->currentSymbol] = ret;
	}

	FOREACH(g->in) {
		if(g->in[i]->flag != origin) continue;
		this->genLive(g->in[i], origin);
	}
}

void Live::clear(Graph* g, bool origin) {
	g->flag = !origin;

	if(g->instr.var == this->currentSymbol && !g->liveTable[this->currentSymbol].second) {
		g->del();
	}

	FOREACH(g->out) {
		if(g->out[i]->flag != origin) continue;
		this->clear(g->out[i], origin);
	}
}

void Live::optimize(string current) {
	// TODO
}

void Live::finalize(string current) {
	this->current = current;

	this->genProcedures(&this->f[this->current], this->f[this->current].flag);

	FOREACH_SYMBOL() {
		for(int i=this->p[current].size()-1; i>=0; --i) {
			if(this->p[current][i]->liveTable[it->first].second) {
				this->live[current][it->first].second = i;
				break;
			}
		}
		if(this->live[current][it->first].second == 0) {
			this->live[current].erase(it->first);
			this->symbols[current].erase(it);
		}
		else {
			FOREACH(this->p[current]) {
				if(this->p[current][i]->liveTable[it->first].second) {
					this->live[current][it->first].first = i;
					break;
				}
			}
		}
	}


}

void Live::genProcedures(Graph* g, bool origin) {
	g->flag = !origin;

	if(!g->instr.empty()){
		this->p[this->current].push_back(g);
	}

	FOREACH(g->out) {
		if(g->out[i]->flag != origin) continue;
		this->genProcedures(g->out[i], origin);
	}
}

map<string, vector<Graph*> >* Live::getProcedures() {
	return &this->p;
}

liveTable* Live::getLive() {
	return &this->live;
}