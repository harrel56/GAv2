#pragma once

#include "ISolver.h"
#include "ISelection.h"
#include "SolutionData.h"
#include "ProgressWindow.h"

#include <QtWidgets\qprogressbar.h>

class GASolver
{
public:
	GASolver(int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions);

	SolutionData *solve(BackpackProblem *bpp, ProgressWindow *bar);

	void setSelection(ISelection *selection);
	void setCancelled(bool state);

private:
	ISelection *selection;
	int popSize;
	int generationCount;
	double mutationChance;
	double crossChance;
	int crossPoints;
	int repetitions;

	bool cancelled;

};