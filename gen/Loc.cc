#include "Loc.h"

#define FOREACH_PROCEDURE(call) { 					\
													\
	(call)(string("wain"));							\
													\
	for(procedures::iterator it = this->p.begin(); 	\
		it != this->p.end(); 						\
		++it) {										\
		if(it->first == "wain") continue;			\
		(call)(it->first);							\
	}												\
}													

/*
*	SORT BY TIME ALIVE
*/

/*
sortByAlive::sortByAlive(Loc* l) : l(l) {}

bool sortByAlive::operator() (const string& a, const string& b) {
	return ((l->alive[l->current][a].second - l->alive[l->current][a].first) 
			> (l->alive[l->current][b].second - l->alive[l->current][b].first));
}
*/

/*
*	REGISTER ALLOCATION
*/

Loc::Loc(procedures& p) : p(p) {
	for(int i=28; i>14; --i) {
		this->freeRegs.push(i);
	}

	this->genAlive();
	this->genLocations();
}

void Loc::genLocations(string current) {
	map<int, vector<int> > freeAt;
	
	for(int i=0; i < this->instrVars[current].size(); ++i) {

		// There are registers that become free at this instruction
		if(freeAt.count(i) != 0) {
			for(int k=0; k<freeAt[i].size(); ++k) {
				this->freeRegs.push(freeAt[i][k]);
			}
		}

		for(int j=0; j< this->instrVars[current][i].size(); ++j) {
			string& var = instrVars[current][i][j];

			// Location already attributed to this symbol
			if(this->location[current].count(var) != 0) continue;
			
			// No free regs, we must use memory
			if(this->freeRegs.empty()) {
				offset++;
				this->location[current][var] = -4*offset;
			}
			// Use the first reg available
			else {
				this->location[current][var] = this->freeRegs.top();
				freeAt[this->alive[current][var].second + 1].push_back(this->freeRegs.top());
				this->freeRegs.pop();
			}
		}
	}
	// Registers have location pointed to themselves
	for(int i=1; i<31; ++i) {
		stringstream ss;
		ss << i;
		this->location[current][string("$" + ss.str())] = i;
	}
}

void Loc::genLocations() {
	FOREACH_PROCEDURE(this->genLocations);
}

void Loc::genAlive(string current) {
	this->current = current;
	vector<string> vars;

	// Build the alive table
	// alive[procedure][symbol] = {first use, last use}
	//
	for(int i=0; i < this->p[current].size(); ++i) {
		Instr &instr = this->p[current][i];
		if(instr.empty()) continue;

		vars = this->varsUsed(instr);
		for(int j=0; j<vars.size(); ++j) {
			this->addAlive(vars[j], i);
		}
	}

	// Build the instrVars table
	// instrVars[procedure][ith instr] = {symbol1, symbol2, ...., symboln}
	//
	// this will make sure no symbols overlap in the same location
	//
	typedef map<string, pair<int, int> >::iterator liveIterator;
	for(liveIterator it = this->alive[current].begin(); it != this->alive[current].end(); ++it) {
		for(int i=it->second.first; i<it->second.second; ++i) {
			this->instrVars[current][i].push_back(it->first);
		}
	}

	/*
	// For each instruction, sort the sybmols by their alive time
	//
	// this will give symbols their allocation priority
	//
	sortByAlive s(this);
	for(int i=0; i < this->instrVars[current].size(); ++i) {
		sort(this->instrVars[current][i].begin(), this->instrVars[current][i].end(), s);
	}
	*/

}

void Loc::genAlive() {
	FOREACH_PROCEDURE(this->genAlive);
}

void Loc::addAlive(string& var, int i) {
	if(!isReg(var)) {
		if(this->alive[this->current].count(var) == 0) {
			this->alive[this->current][var] = make_pair(i, i);
		}
		else {
			this->alive[this->current][var].second = i;
		}
	}
}

vector<string> Loc::varsUsed(Instr& instr) {
	vector<string> ret;

	if(!instr.var.empty() && !isReg(instr.var)) {
		ret.push_back(instr.var);
	}

	if(instr.args.size() == 3) {
		if(!isReg(instr.args[0]))
			ret.push_back(instr.args[0]);
		if(!isReg(instr.args[2]))
			ret.push_back(instr.args[2]);
	}
	else if(instr.args.size() == 1 && !isNum(instr.args[0]) && !isReg(instr.args[0])) {
		ret.push_back(instr.args[0]);
	}

	return ret;
}

ostream& operator<<(ostream& out, Loc& l) {
	for(int i=0; i < l.instrVars["wain"].size(); ++i) {
		out << i;
		for(int j=0; j< l.instrVars["wain"][i].size(); ++j) {
			out << " " << l.instrVars["wain"][i][j];
		}
		out << endl;
	}
}