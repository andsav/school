#include "Mips.h"

int main() {
	try{

		Valid::genParseTree();

		//cout << parseTree;

		Valid::genCode();

		FOREACH(current->symbols) {
			current->symbols[i]->use.second = current->instr.size()-1;
		}

		Loc::genLoc();

		/*
		FOREACH(current->symbols) {
			cout << current->symbols[i]->name << " "
				 << current->symbols[i]->def.first << " - " << current->symbols[i]->use.second
				 << " ::" << current->symbols[i]->loc << endl;
		}
		*/

		Mips::genCode();

		cout << code.rdbuf();

	}
	catch(string s) {
		cerr << "ERROR: " << s << endl;
	}
}

#include "Mips.h"