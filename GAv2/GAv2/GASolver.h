#pragma once

#include "ISolver.h"
#include "ISelection.h"
#include "SolutionData.h"
#include "ProgressWindow.h"
#include "ProbabilityGenerator.h"

#include <QtWidgets\qprogressbar.h>

class GASolver
{
public:
	GASolver(int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions, double funParam1, double funParam2, double funParam3);

	SolutionData *solve(BackpackProblem *bpp, ProgressWindow *bar);

	void setSelection(ISelection *selection);
	const ProbabilityGenerator& getGenerator();

	static bool *copyArray(bool *data, int size);

	const double fixedPenalty;
	const double progressParam;
	const double progressPower;

private:
	ISelection *selection;
	int popSize;
	int generationCount;
	double mutationChance;
	double crossChance;
	int crossPoints;
	int repetitions;

	ProbabilityGenerator generator;

};