#include "Loc.h"												

sortByEndPoint::sortByEndPoint(Loc* l) : l(l) {}

bool sortByEndPoint::operator() (const string& a, const string& b) {
	return (l->alive[l->current][a].second < l->alive[l->current][b].second);
}

sortByStartPoint::sortByStartPoint(Loc* l) : l(l) {}

bool sortByStartPoint::operator() (const string& a, const string& b) {
	return (l->alive[l->current][a].first < l->alive[l->current][b].first);
}

/*
*	REGISTER ALLOCATION
*/

Loc::Loc(procedures& p, table& symbols) : symbols(symbols), p(p), offset(0) {

	this->bullshit(string("wain"));	


	for(int i=28; i>14; --i) {
		this->freeRegs.push(i);
	}

	this->genLocations();

	/*
	for(map<string, int>::iterator it = this->location["wain"].begin(); it != this->location["wain"].end(); ++it) {
		if(isReg(it->first)) continue;
		cout << it->first << ": " << it->second << endl;
	}
	*/
}

void Loc::bullshit(string current) {

	FOREACH_SYMBOL() {
		this->alive["wain"][it->first] = make_pair(0, this->p["wain"].size()-1);
	}
}

void Loc::expire(string var) {
	for(int i=0; i<this->active.size(); ++i) {
		if(this->alive[this->current][this->active[i]].second >= this->alive[this->current][var].first) {
			break;
		}

		this->freeRegs.push(this->location[this->current][this->active[i]]);
		this->active.erase(
			remove(this->active.begin(), this->active.end(), this->active[i]),
			this->active.end());
	}
}

void Loc::spill(string var) {
	string spill = active[active.size()-1];
	sortByEndPoint end(this);

	if(this->alive[this->current][spill].second > this->alive[this->current][var].second) {
		this->location[this->current][var] = this->location[this->current][spill];
		offset++;
		this->location[this->current][spill] = -4*offset;
		active.pop_back();
		active.push_back(var);
		sort(active.begin(), active.end(), end);
	}
	else {
		offset++;
		this->location[this->current][var] = -4*offset;
	}
}

void Loc::genLocations(string current) {
	sortByEndPoint end(this);
	sortByStartPoint start(this);

	for(map<string, pair<int, int> >::iterator it = this->alive[current].begin();
		it != this->alive[current].end(); ++it) {
		this->order[current].push_back(it->first);
	}

	sort(this->order[current].begin(), this->order[current].end(), start);
	this->current = current;
	
	for(int i=0; i<this->order[current].size(); ++i) {
		string& var = this->order[current][i];

		this->expire(var);

		if(this->freeRegs.empty()) {
			this->spill(var);
		}
		else {
			this->location[current][var] = this->freeRegs.top();
			this->freeRegs.pop();
			active.push_back(var);
			sort(active.begin(), active.end(), end);
		}
	}
}

void Loc::genLocations() {
	FOREACH_PROCEDURE(this->genLocations);
}

locTable* Loc::getLocation() {
	return &this->location; 
}

int Loc::getOffset() {
	return this->offset;
}