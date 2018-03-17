#pragma once

#include "Solution.h"

using namespace std;

class Individual : public Solution
{
public:
	Individual(BackpackProblem& bpp);
	Individual(Individual&& ind);
	Individual(const Individual& ind);

	void crossover(Individual& partner, int pts);
	void mutate(double chance);
	int getTotalValue() const;

	Individual& operator=(const Individual& ind);

	static vector<Individual> initializePop(int size, BackpackProblem& bpp);

private:

};
