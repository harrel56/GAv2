#include <random>

#include "BackpackProblem.h"
#include "SolutionData.h"

BackpackProblem::BackpackProblem(int id, const QString& name, int cap) : id(id), name(name), backpackCapacity(cap) {}

BackpackProblem::BackpackProblem(int id, const QString& name, int cap, QVector<Item>&& ritems, QVector<SolutionData*>&& solutions) : id(id), name(name), backpackCapacity(cap), items(ritems), solutions(solutions) {}

BackpackProblem *BackpackProblem::generateNew(const QString& name, int cap, int itemCount, int maxWeight, int maxValue)
{
	BackpackProblem *result = new BackpackProblem(-1, name, cap);
	result->items.reserve(itemCount);
	uniform_int_distribution<int> distributionWeight(1, maxWeight);
	uniform_int_distribution<int> distributionValue(1, maxValue);
	random_device rd;
	default_random_engine rng(rd());
	for (int i = 0; i < itemCount; i++)
	{
		result->items.append(Item(distributionWeight(rng), distributionValue(rng)));
	}

	return result;
}

void BackpackProblem::setId(int id) { this->id = id; }

int BackpackProblem::getId() const { return id; }

const QString& BackpackProblem::getName() const { return name; }

int BackpackProblem::getBackpackCapacity() const { return backpackCapacity; }

const QVector<Item>& BackpackProblem::getItems() const { return items; }

const QVector<SolutionData*>& BackpackProblem::getSolutions() const { return solutions; }

void BackpackProblem::addSolution(SolutionData *solutionData) { solutions.append(solutionData); }

//BackpackProblem::~BackpackProblem() { delete items; }
