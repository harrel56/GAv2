#pragma once

#include "Solution.h"

class Individual : public Solution
{
public:
	Individual(BackpackProblem *bpp);
	Individual(Individual&& ind);
	Individual(const Individual& ind);

	void crossover(Individual& partner, int pts);
	void mutate(double chance);
	int getTotalValue() const;

	Individual& operator=(const Individual& ind);

	static void setPenaltyParams(double fixed, double param, double penaltyPower);

	static vector<Individual> initializePop(int size, BackpackProblem *bpp);
	static const Individual *getBestIndividual(const vector<Individual>& pop);
	static const Individual *getWorstIndividual(const vector<Individual>& pop);
	static const double getAverageValue(const vector<Individual>& pop);

private:

	static double fixedPenalty;
	static double progressParam;
	static double progressPower;

};
