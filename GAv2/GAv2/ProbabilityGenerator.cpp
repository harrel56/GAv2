#include "ProbabilityGenerator.h"

ProbabilityGenerator::ProbabilityGenerator() : rng(rd()) {}

double ProbabilityGenerator::nextProbability() const
{
	return rand(rng);
}