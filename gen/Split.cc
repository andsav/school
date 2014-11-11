#include "Split.h"

/*
Block::Block() { }

Instr* Block::entry() {
	return this->instr[0];
}

Instr* Block::exit() {
	return this->instr[this->instr().size()-1];
}

functions split(procedures& p) {
	functions ret;

	for(procedures::iterator it = p.begin(); it!= p.end(); ++p) {
		ret[it->first] = Block();

		Block& current = ret[it->first];
		vector<Instr>& instr = it->second;

		for(int i=0; i < instr.size(); ++i) {
			switch(instr.cmd) {
				case 'W' :
					current.instr.push_back(instr);
					///////
				break;
				case 'I' :
					current.instr.push_back(instr);
					/////
				break;
				default:
					current.instr.push_back(instr);
				break;
			}
		}
	}

	return ret;
}

*/