#include "GASolver.h"
#include "Individual.h"
#include "ProbabilityGenerator.h"
#include "TournamentSelection.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

GASolver::GASolver() : selection(new TournamentSelection(5, 0.9)), popSize(100), generationCount(30), mutationChance(0.01), crossChance(1.0), crossPoints(2)
{

}

Solution GASolver::solve(BackpackProblem& bpp)
{
	//Initialize 2 sets of individuals to avoid further individual object creations
	auto population = Individual::initializePop(popSize, bpp);
	auto selectedPopulation = Individual::initializePop(popSize, bpp);

	//Get custom RNG
	ProbabilityGenerator& rand = ProbabilityGenerator::getInstance();

	for (int n = 0; n < generationCount; n++)
	{
		//Put best individuals in 'selectedPopulation'
		selection->makeSelection(population, selectedPopulation);

		//Do the crossovers - selectedPop is in a random order so we can just iterate through it
		for (unsigned int i = 0; i < selectedPopulation.size() - 1; i += 2)
		{
			if(i + 1 < selectedPopulation.size() && rand() < crossChance)
				selectedPopulation[i].crossover(selectedPopulation[i + 1], crossPoints);
		}

		//Do the mutations on each individual
		for (unsigned int i = 0; i < selectedPopulation.size(); i++)
			selectedPopulation[i].mutate(mutationChance);

		//Swap containers for next iteration (selectedPop will be overwritten)
		selectedPopulation.swap(population);

		//cout << "Gen no: " << n << endl;
		//for (Individual& ind : population)
		//	cout << setw(4) << ind.getTotalValue();
		//cout << endl;

	}

	//Find best solution and return it
	auto result = max_element(population.cbegin(), population.cend(), [](const Individual& a, const Individual& b)
	{
		int valA = a.isValid() ? a.getTotalValue() : 0;
		int valB = b.isValid() ? b.getTotalValue() : 0;
		return valA < valB ? true : false;
	});

	return *result;
}