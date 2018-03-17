#include "ProbabilityGenerator.h"

ProbabilityGenerator::ProbabilityGenerator() : rng(rd()) {}

ProbabilityGenerator& ProbabilityGenerator::getInstance()
{
	static ProbabilityGenerator rand;
	return rand;
}

double ProbabilityGenerator::operator()()
{
	return rand(rng);
}