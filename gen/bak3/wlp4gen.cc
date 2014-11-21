#include "valid.h"
#include "Mips.h"

int main() {

	Tree parseTree = readTree();
	table* symbols;
	vector<string>* order;
	cell* signature;

	try{
		/*
		*
		*	SEMANTIC TESTING (A8)
		*
		*/
		symbols = genSymbols(parseTree);
		order = getOrder();
		signature = getSignature();
		//testTypes(parseTree);

		/*
		*
		*	COMPILING (A9 - A11)
		*		
		*/

		// 1) Generate intermediate code
		Mini mini(parseTree, *symbols, *order);
		//cout << mini;
		//cout << endl << endl;

		// 2) Build control flow graph
		//Split split(*mini.getCode(), *mini.getFullSymbols());

		// 3) Liveness analysis, constant propagation, optimize
		//Live live(*split.getFunctions(), *mini.getFullSymbols());
		//cout << live;
		//cout << endl << endl;



		// 4) Register allocation
		//Loc loc(*mini.getCode(), *live.getLive());
		Loc loc(*mini.getCode(), *mini.getFullSymbols());

		// 5) Generate MIPS code


		//
		//
		//procedures = optimize(split(m.getCode()));
		//procedures p = m.getCode();
		//Loc loc(p);
		//locTable l = loc.getLocation();

		Mips mips(*mini.getCode(), *loc.getLocation(), loc.getOffset());
		cout << mips;
	}
	catch(string err) {
		cerr << "ERROR (" << err << ")" << endl;
	}
	
}

// 632696