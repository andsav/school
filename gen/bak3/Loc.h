#ifndef LOC_H
#define LOC_H

#include <algorithm>
#include "Live.h"


class Loc {

friend struct sortByEndPoint;
friend struct sortByStartPoint;

private:
	procedures& p;

	liveTable		alive;

	int 			offset;
	stack<int> 		freeRegs;
	string 			current;

	locTable 		location;

	vector<string> 	active;
	table& symbols;

	map<string, vector<string> > order;

	void expire(string);
	void spill(string);
	void genLocations(string);
	void genLocations();

	void bullshit(string);

public:
	Loc(procedures&, table& symbols);
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