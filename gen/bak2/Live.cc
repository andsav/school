#include "Live.h"

State::State(tt t) : t(t) { }
State::State(tt t, int val) : t(t), val(val) { }

Live::Live(functions& f, table& symbols) : f(f), symbols(symbols) { 
	this->genState();
}

void Live::genState(string current) {
	this->current = current;
	vector<Block*> flat = this->f[current].flatten();

	map<Instr*, int> instrId;
	vector<vector<int> > prev;

	int id = 0;
	FOREACH(flat) {
		cout << endl << "BLOCK" << endl;
		FOREACH_(flat[i]->instr, j) {
			instrId[&flat[i]->instr[j]] = id;

			// If we're at block entry, previous get previous instructions from previous block 
			if(j == 0) {
				vector<int> push;
				FOREACH_(flat[i]->in, k) {
					push.push_back(instrId[flat[i]->in[k]->exit()]);
				}
				prev.push_back(push);
			}
			// Else, there is only one previous instruction;
			else {
				prev.push_back(vector<int>(1, id-1));
			}

			cout << flat[i]->instr[j];

			++id;

			for(cell::iterator it = this->symbols[current].begin(); it != this->symbols[current].end(); ++it) {
				this->state[current][it->first].push_back(make_pair(State(BOT), State(BOT)));
			}
		}
	}

	for(cell::iterator it = symbols[current].begin(); it != symbols[current].end(); ++it) {
		this->state[current][it->first][0].first.t = TOP;
	}
}

void Live::genState() {
	FOREACH_FUNCTION(genState);
}