#pragma once

#include "BackpackProblem.h"
#include "SolutionData.h"

class ISolver
{
public:
	virtual SolutionData *solve(BackpackProblem* bpp) = 0;

private:

};