#include "Loc.h"												

sortByEndPoint::sortByEndPoint(Loc* l) : l(l) {}

bool sortByEndPoint::operator() (const string& a, const string& b) {
	return (l->alive[l->current][a].second < l->alive[l->current][b].second);
}

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

void Loc::expire(string var) {
	for(int i=0; i<this->active.size(); ++i) {
		if(this->alive[this->current][this->active[i]].second >= this->alive[this->current][var].first) {
			break;
		}

		this->active.erase(
			remove(this->active.begin(), this->active.end(), this->active[i]),
			this->active.end());
		this->freeRegs.push(this->location[this->current][this->active[i]]);
	}
}

void Loc::spill(string var) {
	string spill = active[active.size()-1];
	sortByEndPoint s(this);

	if(this->alive[this->current][spill].second > this->alive[this->current][var].second) {
		this->location[this->current][var] = this->location[this->current][spill];
		offset++;
		this->location[this->current][spill] = -4*offset;
		active.pop_back();
		active.push_back(var);
		sort(active.begin(), active.end(), s);
	}
	else {
		offset++;
		this->location[this->current][var] = -4*offset;
	}
}

void Loc::genLocations(string current) {
	sortByEndPoint s(this);
	this->current = current;
	
	for(map<string, pair<int, int> >::iterator it = this->alive[current].begin(); it != this->alive[current].end(); ++it) {
		this->expire(it->first);
		if(this->active.size() == 14) {
			this->spill(it->first);
		}
		else {
			this->location[current][it->first] = this->freeRegs.top();
			this->freeRegs.pop();
			active.push_back(it->first);
			sort(active.begin(), active.end(), s);
		}
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

locTable Loc::getLocation() {
	return this->location; 
}

int Loc::getOffset() {
	return this->offset;
}

ostream& operator<<(ostream& out, Loc& l) {
	for(map<string, pair<int, int> >::iterator it = l.alive["wain"].begin(); it != l.alive["wain"].end(); ++it) {
		int i = it->second.first;
		Instr& instr = l.p["wain"][i];

		out << endl << ";; " << instr.var << " " << instr.cmd;
		for(int j=0; j<instr.args.size(); ++j) {
			out << " " << instr.args[j];
		}
		out << endl;
		out << it->first << ": " << it->second.first << " " << it->second.second << endl;
	}

	out << endl << endl;

	for(int i=0; i < l.instrVars["wain"].size(); ++i) {
		out << i;
		for(int j=0; j< l.instrVars["wain"][i].size(); ++j) {
			out << " " << l.instrVars["wain"][i][j];
		}
		out << endl;
	}

	out << endl << endl;

	for(map<string, int>::iterator it = l.location["wain"].begin(); it != l.location["wain"].end(); ++it) {
		if(isReg(it->first)) continue;
		out << it->first << ": " << it->second << endl;
	}
}