#pragma once

#include "BackpackProblem.h"
#include "Solution.h"

class ISolver
{
public:
	virtual Solution solve(BackpackProblem& bpp) = 0;

private:

};