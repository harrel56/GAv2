#include "Individual.h"
#include "ProbabilityGenerator.h"

#include <iostream>
#include <conio.h>
#include <algorithm>
#include <limits>
#include <math.h>

using namespace std;

double Individual::fixedPenalty;
double Individual::progressParam;
double Individual::progressPower;

Individual::Individual(BackpackProblem *bpp) : Solution(bpp)
{
	ProbabilityGenerator& rand = ProbabilityGenerator::getInstance();
	for (int i = 0; i < size; i++)
	{
		if (rand() > 0.5)
			data[i] = true;
	}
}

Individual::Individual(Individual&& ind) : Solution(move(ind)) {}

Individual::Individual(const Individual& ind) : Solution(ind) {}

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

	//cout << "Crosspoints: ";
	//for (int i = 0; i < pts; i++)
	//	cout << points[i] << ", ";
	//cout << endl << endl << "Parents: " << endl;
	//for (int i = 0; i < size; i++)
	//	cout << data[i];
	//cout << endl;
	//for (int i = 0; i < size; i++)
	//	cout << partner.data[i];
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
	//cout << endl << "Children: " << endl;
	//for (int i = 0; i < size; i++)
	//	cout << data[i];
	//cout << endl;
	//for (int i = 0; i < size; i++)
	//	cout << partner.data[i];
	//cout << endl << endl;
	//_getch();
}

void Individual::mutate(double chance)
{
	ProbabilityGenerator& rand = ProbabilityGenerator::getInstance();
	for (int i = 0; i < size; i++)
	{
		if (rand() < chance)
			data[i] = !data[i];
	}
}

int Individual::getTotalValue() const
{
	int value = Solution::getTotalValue();
	int weight = Solution::getTotalWeight();
	if (problem->getBackpackCapacity() > weight)
		return value;
	else
		return value - (value * Individual::fixedPenalty - Individual::progressParam * std::pow(problem->getBackpackCapacity() - weight, Individual::progressPower));
}

Individual& Individual::operator=(const Individual& ind)
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

void Individual::setPenaltyParams(double fixed, double param, double penaltyPower)
{
	Individual::fixedPenalty = fixed;
	Individual::progressParam = param;
	Individual::progressPower = penaltyPower;
}

vector<Individual> Individual::initializePop(int size, BackpackProblem *bpp)
{
	vector<Individual> result;
	result.reserve(size);

	for (int i = 0; i < size; i++)
	{
		result.push_back(Individual(bpp));
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

	for (const Individual& ind : pop)
	{
		if (ind.isValid())
		{
			sum += ind.getTotalValue();
		}
	}
	return sum / pop.size();
}
