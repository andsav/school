#ifndef LOC_H
#define LOC_H

#include <algorithm>
#include "Opt.h"

typedef map<string, map<string, int> > 				locTable;
typedef map<string, map<string, pair<int, int> > > 	liveTable;
typedef map<string, map<int, vector<string> > > 	instrVarsTable;

class Loc{

//friend struct sortByAlive;
friend ostream& operator<<(ostream&, Loc&);

private:
	procedures& 	p;

	int 			offset;
	stack<int> 		freeRegs;
	string 			current;

	locTable 		location;
	liveTable 		alive;
	instrVarsTable 	instrVars;


	void addAlive(string&, int);

	void genAlive(string);
	void genAlive();

	void genLocations(string);
	void genLocations();

	vector<string> varsUsed(Instr&);

public:
	Loc(procedures&);

};

/*
struct sortByAlive {
	Loc* l;
	sortByAlive(Loc*);
	bool operator() (const string&, const string&);
};
*/

#endif