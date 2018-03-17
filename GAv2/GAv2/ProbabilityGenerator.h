#pragma once

#include <random>

using namespace std;

class ProbabilityGenerator
{
public:
	ProbabilityGenerator(const ProbabilityGenerator&) = delete;
	ProbabilityGenerator(ProbabilityGenerator&&) = delete;
	void operator=(const ProbabilityGenerator&) = delete;

	static ProbabilityGenerator& getInstance();

	double operator()();

private:
	ProbabilityGenerator();

	random_device rd;
	default_random_engine rng;
	uniform_real_distribution<double> rand;
};