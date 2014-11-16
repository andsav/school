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
	return &this->instr[0];
}

Instr* Block::exit() {
	return &this->instr[this->instr.size()-1];
}


/*
*	GRAPH
*/

Graph::Graph(procedures& p) : p(p) {
	FOREACH_PROCEDURE(this->split);
}

void Graph::split(vector<Instr>& instr) {

	FOREACH(instr) {
		Block& graph = *this->currentBlock;
		Instr& thisInstr = instr[i];

		switch(thisInstr.cmd) {
			case 'I' :
			{
				graph.instr.push_back(thisInstr);

				Block* ifBlock = this->createBlock(&graph);
				Block* elseBlock = this->createBlock(&graph);

				this->currentBlock = ifBlock;

				vector<Instr>::iterator begin = instr.begin() + i + 1;
				vector<Instr>::iterator end = instr.begin() + i;

				do end++;
				while(++i < instr.size() && instr[i].var != thisInstr.var && instr[i].cmd != 'e');
				vector<Instr> subInstr(begin, end);
				this->split(subInstr);

				ifBlock = this->currentBlock;
				this->currentBlock = elseBlock;

				begin = end + 1;
				do end++;
				while(++i < instr.size() && instr[i].var != thisInstr.var && instr[i].cmd != 'i');
				vector<Instr> subInstr2(begin, end);

				this->split(subInstr2);

				elseBlock = this->currentBlock;
				Block* afterBlock = this->createBlock(ifBlock, elseBlock);
				this->currentBlock = afterBlock;
			}
			break;

			case 'W' :
			{
				graph.instr.push_back(instr[i]);

				Block* testBlock = this->createBlock(&graph);
				Block* loopBlock = this->createBlock(testBlock);

				loopBlock->out.push_back(loopBlock);
				
				this->currentBlock = testBlock;
				vector<Instr>::iterator begin = instr.begin() + i + 1;
				vector<Instr>::iterator end = instr.begin() + i;

				do end++;
				while(++i < instr.size() && instr[i].var != thisInstr.var && instr[i].cmd != 'T');
				vector<Instr> subInstr(begin, end);
				this->split(subInstr);

				testBlock = this->currentBlock;
				this->currentBlock = loopBlock;

				begin = end + 1;
				do end++;
				while(++i < instr.size() && instr[i].var != thisInstr.var && instr[i].cmd != 'w');
				vector<Instr> subInstr2(begin, end);

				this->split(subInstr2);

				loopBlock = this->currentBlock;
				Block* afterBlock = this->createBlock(testBlock, loopBlock);
				this->currentBlock = afterBlock;
			}
			break;
			
			default:
			{
				if(instr[i].cmd != 'e' && instr[i].cmd != 'w' && instr[i].cmd != 'i')
					graph.instr.push_back(instr[i]);
			}
			break;
		}
	}
}

void Graph::split(string current) {
	this->current = current;
	this->currentBlock = &this->f[current];
	
	this->split(this->p[current]);
}

functions* Graph::getFunctions() {
	return &this->f;
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
Run this if you like infinite loops :)
ostream& operator<<(ostream& out, Block& b) {
	FOREACH(b.instr) {
		out << b.instr[i];
	}
	FOREACH(b.out) {
		out << *b.out[i];
	}
}

ostream& operator<<(ostream& out, Graph& g) {
	out << g.f["wain"];
}
*/