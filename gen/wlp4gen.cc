#include "valid.h"
#include "Mips.h"m

int main() {

	Tree parseTree = readTree();
	table* symbols;
	vector<string>* order;
	cell* signature;

	try{
		symbols = genSymbols(parseTree);
		order = getOrder();
		signature = getSignature();
		//testTypes(parseTree);

		Mini m(parseTree, *symbols, *order);
		//cout << m;
		
		Graph graph(*m.getCode());
		cout << graph;

		//
		//
		//procedures = optimize(split(m.getCode()));
		//procedures p = m.getCode();
		//Loc loc(p);
		//locTable l = loc.getLocation();

		//Mips mips(p, l, loc.getOffset());
		//cout << mips;
	}
	catch(string err) {
		cerr << "ERROR (" << err << ")" << endl;
	}
	
}

// 632696