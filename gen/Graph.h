#ifndef SPLIT_H
#define SPLIT_H

#include "Mini.h"

struct Block {
	vector<Instr> instr;
	
	vector<Block*> in;
	vector<Block*> out;

	Block();
	Block(Block*);
	Block(Block*, Block*);

	Instr* entry();
	Instr* exit();
};

typedef map<string, Block> functions;


class Graph {

//friend ostream& operator<<(ostream&, Graph&);

private:	
	procedures& p;
	functions f;
	string current;

	Block* currentBlock;

	stack<Block> blocksPile;

public:
	Graph(procedures&);
	
	void split(vector<Instr>&);
	void split(string);

	functions* getFunctions();

	Block* createBlock(Block*);
	Block* createBlock(Block*, Block*);
};

#endif