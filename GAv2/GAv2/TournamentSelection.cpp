#include "TournamentSelection.h"
#include "ProbabilityGenerator.h"

#include <algorithm>
#include <math.h>

using namespace std;

const QString TournamentSelection::NAME = "Tournament selection";

TournamentSelection::TournamentSelection(int s, double prob) : size(s) 
{
	probabilities = new double[size];
	for (int i = 0; i < size - 1; i++)
	{
		probabilities[i] = prob * pow(1 - prob, i);
	}
	probabilities[size - 1] = 1.0;
}

void TournamentSelection::makeSelection(const vector<Individual>& pop, vector<Individual>& selected)
{
	//Init 2 RNGs
	ProbabilityGenerator& probRand = ProbabilityGenerator::getInstance();
	random_device rd;
	default_random_engine rng(rd());
	uniform_int_distribution<int> rand(0, pop.size() - 1);

	//Init vector for tournament participants
	vector<const Individual*> parts(size, nullptr);

	for (unsigned int i = 0; i < pop.size(); i++)
	{
		//Choose participants randomly
		for (int j = 0; j < size; j++)
		{
			parts[j] = &pop[rand(rng)];
		}

		//Sort participant to easily determine winner
		sort(parts.begin(), parts.end(), [](const Individual* a, const Individual* b)
		{
			return a->getTotalValue() < b->getTotalValue();
		});

		//Determine winner and add him to selected vector
		double prob = probRand();
		for (int j = 0; j < size; j++)
		{
			if (prob < probabilities[j])
				selected[i] = *parts[j];
		}
	}
}

TournamentSelection::~TournamentSelection() { delete probabilities; }