#include "Individual.h"
#include "ProbabilityGenerator.h"

#include <iostream>
#include <conio.h>
#include <algorithm>

using namespace std;

Individual::Individual(BackpackProblem& bpp) : Solution(bpp)
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
	if (problem.getBackpackCapacity() > weight)
		return value;
	else
		return value - (value / 10 - 10 * (problem.getBackpackCapacity() - weight));
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

vector<Individual> Individual::initializePop(int size, BackpackProblem& bpp)
{
	vector<Individual> result;
	result.reserve(size);

	for (int i = 0; i < size; i++)
	{
		result.push_back(Individual(bpp));
	}

	return result;
}