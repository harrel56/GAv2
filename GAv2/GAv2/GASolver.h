#pragma once

#include "ISolver.h"
#include "ISelection.h"

class GASolver : public ISolver
{
public:
	GASolver();

	Solution solve(BackpackProblem& bpp);

private:
	ISelection *selection;
	int popSize;
	int generationCount;
	double mutationChance;
	double crossChance;
	int crossPoints;

};