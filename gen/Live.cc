#include "Live.h"

Live::Live(functions& f, table& symbols) : f(f), symbols(symbols) { 
	this->genState();
}

void Live::genState(string current) {
	
}

void Live::genState() {
	FOREACH_FUNCTION(genState);
}