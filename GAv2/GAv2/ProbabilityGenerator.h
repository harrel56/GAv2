#pragma once

#include <random>

using namespace std;

class ProbabilityGenerator
{
public:
	ProbabilityGenerator();
	ProbabilityGenerator(const ProbabilityGenerator&) = delete;
	ProbabilityGenerator(ProbabilityGenerator&&) = delete;
	void operator=(const ProbabilityGenerator&) = delete;

	double nextProbability() const;

private:

	random_device rd;
	mutable default_random_engine rng;
	uniform_real_distribution<double> rand;
};