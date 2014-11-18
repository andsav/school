#include "Graph.h"

/*
*	BLOCK
*/

Block::Block() { }

Block::Block(Block* origin) {
	this->in.push_back(origin);
}

Block::Block(Block* origin1, Block* origin2) {
	this->in.push_back(origin1);
	this->in.push_back(origin2);
}

Instr* Block::entry() {
	return this->instr.front();
}

Instr* Block::exit() {
	return this->instr.back();
}

/*
*	GRAPH
*/

Graph::Graph() { }

Graph::Graph(vector<Instr>& instr) {
	FOREACH(instr) {
		this->instr.push_back(instr[i]);
		Instr* inst = &this->instr[i];
		this->instrId[inst] = i;
	}
}

Block* Graph::createBlock(Block* origin) {
	this->blocksPile.push(Block(origin));
	Block* ret = &this->blocksPile.top();

	origin->out.push_back(ret);
	
	return ret;
}

Block* Graph::createBlock(Block* origin1, Block* origin2) {
	this->blocksPile.push(Block(origin1, origin2));
	Block* ret = &this->blocksPile.top();

	origin1->out.push_back(ret);
	origin2->out.push_back(ret);

	return ret;
}


/*
*	SPLIT
*/

Split::Split(procedures &p) : p(p) {
	FOREACH_PROCEDURE(this->gen);
}

void Split::gen(vector<Instr>& instr) {
	vector<Instr>::iterator begin, end;
	vector<Instr> subInstr;

	FOREACH(instr) {
		Block &currentBlock = *this->currentBlock;
		Instr& thisInstr = instr[i];

		currentBlock.instr.push_back(&thisInstr);

		if(thisInstr.cmd == 'I') {
			++i;
			Block* ifBlock = this->f[current].createBlock(&currentBlock);
			Block* elseBlock = this->f[current].createBlock(&currentBlock);

			this->currentBlock = ifBlock;

			begin = instr.begin() + i;
			for(end = begin-1; end->var != thisInstr.var || end->cmd != 'e'; ++end) {
				++i;
			}
			++end;
				
			subInstr = vector<Instr>(begin, end);
			this->gen(subInstr);

			ifBlock = this->currentBlock;
			this->currentBlock = elseBlock;

			for(begin = end; end->var != thisInstr.var || end->cmd != 'i'; ++end) {
				++i;
			}
			++end;

			subInstr = vector<Instr>(begin, end);
			this->gen(subInstr);

			elseBlock = this->currentBlock;
			Block* afterBlock = this->f[current].createBlock(ifBlock, elseBlock);
			this->currentBlock = afterBlock;
		}
		else if(thisInstr.cmd == 'W') {
			++i;
			Block* testBlock = this->f[current].createBlock(&currentBlock);
			Block* loopBlock = this->f[current].createBlock(testBlock);

			loopBlock->out.push_back(testBlock);

			this->currentBlock = testBlock;

			begin = instr.begin() + i;
			for(end = begin-1; end->var != thisInstr.var || end->cmd != 'T'; ++end) {
				++i;
			}
			++end;

			cout << *begin;
			cout << *end;
			throw string("HELLO");

			subInstr = vector<Instr>(begin, end);
			this->gen(subInstr);

			testBlock = this->currentBlock;
			this->currentBlock = loopBlock;

			for(begin = end; end->var != thisInstr.var || end->cmd != 'w'; ++end) {
				++i;
			}
			++end;

			subInstr = vector<Instr>(begin, end);
			this->gen(subInstr);

			loopBlock = this->currentBlock;
			Block* afterBlock = this->f[current].createBlock(testBlock, loopBlock);
			this->currentBlock = afterBlock;
		}
	}
}

void Split::gen(string current) {
	this->current = current;

	this->f[current] = Graph(this->p[current]);
	this->currentBlock = &this->f[current].b;

	this->gen(this->p[current]);

	while(!this->f[current].blocksPile.empty()) {
		cout << endl << "BLOCK" << endl;
		FOREACH(this->f[current].blocksPile.top().instr) {
			cout << *this->f[current].blocksPile.top().instr[i];
		}
		this->f[current].blocksPile.pop();
	}
		
}

functions* Split::getFunctions() {
	return &this->f;
}