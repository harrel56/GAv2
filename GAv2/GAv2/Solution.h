#pragma once

#include "BackpackProblem.h"

class Solution
{
public:
	Solution(const Solution& sol);
	Solution(Solution&& sol);
	Solution(BackpackProblem& bpp);

	bool isValid() const;
	int getTotalValue() const;
	int getTotalWeight() const;

	Solution& operator=(const Solution& sol);
	Solution& operator=(Solution&& sol);

	int getSize() const;
	bool* getData() const;
	const BackpackProblem& getProblem() const;

	~Solution();

protected:
	int size;
	bool* data;
	BackpackProblem& problem;

};