#ifndef SPLIT_H
#define SPLIT_H

#include "Mini.h"

struct Block {
	vector<Instr*> instr;
	
	vector<Block*> in;
	vector<Block*> out;

	Block();
	Block(Block*);
	Block(Block*, Block*);

	Instr* entry();
	Instr* exit();
};

struct Graph {
	vector<Instr> instr;
	
	stack<Block> blocksPile;

	map<Instr*, int> instrId;

	Block b;

	Graph();
	Graph(vector<Instr>&);

	vector<Instr*> next(Instr*);
	vector<Instr*> prev(Instr*);

	Block* createBlock(Block*);
	Block* createBlock(Block*, Block*);

};

typedef map<string, Graph> functions;

class Split {
private:	
	procedures& p;
	functions f;
	
	string current;

	Block* currentBlock;

	void gen(vector<Instr>&);
	void gen(string);

public:
	Split(procedures&);

	functions* getFunctions();
};

#endif