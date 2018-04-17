#pragma once

#include "Solution.h"

class GASolver;

class Individual : public Solution
{
public:
	Individual(BackpackProblem *bpp, GASolver *solver);
	Individual(Individual&& ind);
	Individual(const Individual& ind);

	void crossover(Individual& partner, int pts);
	void mutate(double chance);
	int getTotalValue() const;

	const Individual& operator=(const Individual& ind);
	const Individual& operator=(Individual&& ind);

	static vector<Individual> initializePop(int size, BackpackProblem *bpp, GASolver *solver);
	static const Individual *getBestIndividual(const vector<Individual>& pop);
	static const Individual *getWorstIndividual(const vector<Individual>& pop);
	static const double getAverageValue(const vector<Individual>& pop);

private:
	GASolver *solver;

};
