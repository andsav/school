#ifndef LOC_H
#define LOC_H

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


#include <algorithm>
#include "Opt.h"

typedef map<string, map<string, int> > 				locTable;
typedef map<string, map<string, pair<int, int> > > 	liveTable;
typedef map<string, map<int, vector<string> > > 	instrVarsTable;

class Loc{

friend struct sortByEndPoint;
friend struct sortByStartPoint;
friend ostream& operator<<(ostream&, Loc&);

private:
	procedures& 	p;

	int 			offset;
	stack<int> 		freeRegs;
	string 			current;

	locTable 		location;
	liveTable 		alive;
	instrVarsTable 	instrVars;

	vector<string> 	active;

	map<string, vector<string> > order;

	void addAlive(string&, int);

	void expire(string);
	void spill(string);

	void genAlive(string);
	void genAlive();

	void genLocations(string);
	void genLocations();

	vector<string> varsUsed(Instr&);

public:
	Loc(procedures&);
	locTable getLocation();
	int getOffset();
};

struct sortByEndPoint {
	Loc* l;
	sortByEndPoint(Loc*);
	bool operator() (const string&, const string&);
};

struct sortByStartPoint {
	Loc* l;
	sortByStartPoint(Loc*);
	bool operator() (const string&, const string&);
};


#endif