#ifndef LOC_H
#define LOC_H

#include "Opt.h"

namespace Loc {
	void genLoc();

	void spill(string&, sortSymbol&);
	void expire(string&);
}

#endif