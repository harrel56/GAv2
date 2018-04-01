#pragma once

#include "ISolver.h"
#include "ISelection.h"
#include "SolutionData.h"
#include "ProgressWindow.h"

#include <QtWidgets\qprogressbar.h>

class GASolver
{
public:
	GASolver(int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions, double funParam1, double funParam2, double funParam3);

	SolutionData *solve(BackpackProblem *bpp, ProgressWindow *bar);

	void setSelection(ISelection *selection);
	void setCancelled(bool state);

	static bool *copyArray(bool *data, int size);

private:
	ISelection *selection;
	int popSize;
	int generationCount;
	double mutationChance;
	double crossChance;
	int crossPoints;
	int repetitions;

	double funParam1;
	double funParam2;
	double funParam3;

	bool cancelled;

};