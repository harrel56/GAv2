#include "Individual.h"
#include "ProbabilityGenerator.h"
#include "GASolver.h"

#include <iostream>
#include <conio.h>
#include <algorithm>
#include <limits>
#include <math.h>

using namespace std;

Individual::Individual(BackpackProblem *bpp, GASolver *solver) : Solution(bpp), solver(solver)
{
	for (int i = 0; i < size; i++)
	{
		if (solver->getGenerator().nextProbability() > 0.5)
			data[i] = true;
	}
}

Individual::Individual(Individual&& ind) : Solution(move(ind)), solver(ind.solver) {}

Individual::Individual(const Individual& ind) : Solution(ind), solver(ind.solver) {}

void Individual::crossover(Individual& partner, int pts)
{
	//Init RNG
	random_device rd;
	default_random_engine rng(rd());
	uniform_int_distribution<int> rand(1, size - 1);

	//Init cross points randomly without repetition
	if (pts > size)
		pts = size;
	int* points = new int[pts];
	for (int i = 0; i < pts; i++)
	{
		bool unique = true;
		do
		{
			unique = true;
			points[i] = rand(rng);
			for (int j = 0; j < i; j++)
			{
				if (points[i] == points[j])
				{
					unique = false;
					break;
				}
			}
		} while (!unique);
	}

	//Change data in each individual based on cross points
	bool inverted = false;
	for (int i = 0; i < size; i++)
	{
		if (find(points, points + pts, i) != points + pts)
			inverted = !inverted;

		if (inverted)
		{
			swap(this->data[i], partner.data[i]);
		}
	}

	delete points;
}

void Individual::mutate(double chance)
{
	for (int i = 0; i < size; i++)
	{
		if (solver->getGenerator().nextProbability() < chance)
			data[i] = !data[i];
	}
}

int Individual::getTotalValue() const
{
	int value = Solution::getTotalValue();
	int weight = Solution::getTotalWeight();
	if (problem->getBackpackCapacity() >= weight)
		return value;
	else
		return value - (value * solver->fixedPenalty - solver->progressParam * std::pow(problem->getBackpackCapacity() - weight, solver->progressPower));
}

const Individual& Individual::operator=(const Individual& ind)
{
	if (this != &ind)
	{
		problem = ind.problem;
		if (size != ind.size)
		{
			delete data;
			size = ind.getSize();
			data = new bool[size];
		}
		for (int i = 0; i < size; i++)
			data[i] = ind.data[i];
	}
	return *this;
}

const Individual & Individual::operator=(Individual && ind)
{
	if (this->data != nullptr)
		delete this->data;

	this->data = ind.data;
	ind.data = nullptr;

	return *this;
}

vector<Individual> Individual::initializePop(int size, BackpackProblem *bpp, GASolver *solver)
{
	vector<Individual> result;
	result.reserve(size);

	for (int i = 0; i < size; i++)
	{
		result.push_back(Individual(bpp, solver));
	}

	return result;
}

const Individual *Individual::getBestIndividual(const vector<Individual>& pop)
{
	int max = 0;
	const Individual *best = nullptr;
	int temp;
	if (!pop.empty())
		best = &pop[0];

	for (const Individual& ind : pop)
	{
		if (ind.isValid())
		{
			temp = ind.getTotalValue();
			if (temp > max)
			{
				max = temp;
				best = &ind;
			}
		}
	}
	return best;
}

const Individual * Individual::getWorstIndividual(const vector<Individual>& pop)
{
	int min = numeric_limits<int>::max();
	const Individual *worst = nullptr;
	int temp;
	if (!pop.empty())
		worst = &pop[0];

	for (const Individual& ind : pop)
	{
		if (ind.isValid())
		{
			temp = ind.getTotalValue();
			if (temp < min)
			{
				min = temp;
				worst = &ind;
			}
		}
	}
	return worst;
}

const double Individual::getAverageValue(const vector<Individual>& pop)
{
	double sum = 0.0;
	int validCount = 0;

	for (const Individual& ind : pop)
	{
		if (ind.isValid())
		{
			sum += ind.getTotalValue();
			++validCount;
		}
	}
	return validCount > 0 ? sum / validCount : 0.0;
}
