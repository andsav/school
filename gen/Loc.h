#ifndef LOC_H
#define LOC_H

#include <algorithm>
#include "Live.h"

typedef map<string, map<string, int> > locTable;


class Loc {

friend struct sortByEndPoint;
friend struct sortByStartPoint;

private:
	map<string, vector<Graph*> >& p;

	liveTable& 		alive;

	int 			offset;
	stack<int> 		freeRegs;
	string 			current;

	locTable 		location;

	vector<string> 	active;

	map<string, vector<string> > order;

	void expire(string);
	void spill(string);
	void genLocations(string);
	void genLocations();

public:
	Loc(map<string, vector<Graph*> >&, liveTable&);
	locTable* getLocation();
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