#include <vector>
#include <iostream>

#include "Solution.h"

using namespace std;

Solution::Solution(const Solution& sol) : problem(sol.problem), size(sol.size)
{
	data = new bool[size] {false};
	for (int i = 0; i < size; i++)
		data[i] = sol.data[i];
}

Solution::Solution(Solution&& sol) : problem(sol.problem), size(sol.size)
{
	data = sol.data;
	sol.data = nullptr;
}

Solution::Solution(BackpackProblem *bpp) : problem(bpp), size(bpp->getItems().size()) 
{
	data = new bool[size] {false};
}

bool Solution::isValid() const
{
	auto items = problem->getItems();
	int cap = problem->getBackpackCapacity();
	int sum = 0;

	for (int i = 0; i < size; i++)
	{
		if (data[i])
			sum += items.at(i).getWeight();
		if (sum > cap)
			return false;
	}
	return true;
}

int Solution::getTotalValue() const
{
	auto items = problem->getItems();
	int val = 0;

	for (int i = 0; i < size; i++)
	{
		if (data[i])
			val += items.at(i).getValue();
	}
	return val;
}

int Solution::getTotalWeight() const
{
	auto items = problem->getItems();
	int result = 0;

	for (int i = 0; i < size; i++)
	{
		if (data[i])
			result += items.at(i).getWeight();
	}
	return result;		
}

Solution& Solution::operator=(const Solution& sol)
{
	if (this != &sol)
	{
		problem = sol.problem;
		if (size != sol.size)
		{
			delete data;
			size = sol.getSize();
			data = new bool[size];
		}
		for (int i = 0; i < size; i++)
			data[i] = sol.data[i];
	}
	return *this;
}

Solution& Solution::operator=(Solution&& sol)
{
	if (this != &sol)
	{
		delete data;
		size = sol.size;
		data = sol.data;
		sol.data = nullptr;
	}
	return *this;
}

int Solution::getSize() const { return size; }

bool* Solution::getData() const { return data; }

const BackpackProblem *Solution::getProblem() const { return problem; }

Solution::~Solution() { delete data; }


