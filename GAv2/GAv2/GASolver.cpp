#include "GASolver.h"
#include "Individual.h"
#include "TournamentSelection.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <algorithm>
#include <math.h>
#include <QtCore\qmetaobject.h>

using namespace std;

GASolver::GASolver(int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions, double funParam1, double funParam2, double funParam3) :
	popSize(popSize), generationCount(generationCount), mutationChance(mutationChance), crossChance(crossChance), crossPoints(crossPoints), repetitions(repetitions),
	fixedPenalty(funParam1), progressParam(funParam2), progressPower(funParam3)
{

}

void GASolver::setSelection(ISelection *selection) { this->selection = selection; }

const ProbabilityGenerator & GASolver::getGenerator()
{
	return generator;
}

bool *GASolver::copyArray(bool *data, int size)
{
	bool *result = new bool[size];
	for (int i = 0; i < size; ++i)
	{
		result[i] = data[i];
	}
	return result;
}

SolutionData *GASolver::solve(BackpackProblem *bpp, ProgressWindow *bar)
{
	selection->setSolver(this);

	//Initialize 2 sets of individuals to avoid further individual object creations
	auto population = Individual::initializePop(popSize, bpp, this);
	auto selectedPopulation = Individual::initializePop(popSize, bpp, this);

	SolutionData *result = new SolutionData(selection, popSize, generationCount, mutationChance, crossChance, crossPoints, repetitions, fixedPenalty, progressParam, progressPower);
	QVector<QVector<TimePoint>> timePointsData(repetitions);

	bar->setMaxValue(generationCount * repetitions);

	for (int r = 0; r < repetitions; ++r)
	{
		timePointsData[r].reserve(generationCount);
		for (int n = 0; n < generationCount; ++n)
		{
			auto indMax = Individual::getBestIndividual(population);
			int valMax = indMax->getTotalValue();
			auto indMin = Individual::getWorstIndividual(population);
			int valMin = indMin->getTotalValue();
			double avgVal = Individual::getAverageValue(population);
			timePointsData[r].append(TimePoint(valMax < 0 ? 0 : valMax, avgVal, valMin < 0 ? 0 : valMin, copyArray(indMax->getData(), bpp->getItems().size()), copyArray(indMin->getData(), bpp->getItems().size())));

			//Put best individuals in 'selectedPopulation'
			selection->makeSelection(population, selectedPopulation);

			//Do the crossovers - selectedPop is in a random order so we can just iterate through it
			for (unsigned int i = 0; i < selectedPopulation.size() - 1; i += 2)
			{
				if (i + 1 < selectedPopulation.size() && generator.nextProbability() < crossChance)
					selectedPopulation[i].crossover(selectedPopulation[i + 1], crossPoints);
			}

			//Do the mutations on each individual
			for (unsigned int i = 0; i < selectedPopulation.size(); ++i)
				selectedPopulation[i].mutate(mutationChance);

			//Swap containers for next iteration (selectedPop will be overwritten)
			selectedPopulation.swap(population);

			//Update GUI thread through meta object call and check if user has cancelled this action
			QMetaObject::invokeMethod(bar, "update", Q_ARG(int, r), Q_ARG(int, n), Q_ARG(int, r * generationCount + n));
			if (bar->isCancelled())
			{
				QMetaObject::invokeMethod(bar, "failed");
				return nullptr;
			}
		}
	}
	QMetaObject::invokeMethod(bar, "update", Q_ARG(int, repetitions - 1), Q_ARG(int, generationCount), Q_ARG(int, repetitions * generationCount));

	//Fill the result
	for (int n = 0; n < generationCount; ++n)
	{
		double sumBest = 0.0;
		double sumWorst = 0.0;
		double sumAvg = 0.0;

		bool *bestData;
		bool *worstData;
		double max = numeric_limits<double>::min();
		double min = numeric_limits<double>::max();
		for (int r = 0; r < repetitions; ++r)
		{
			sumBest += timePointsData[r][n].bestValue;
			if (timePointsData[r][n].bestValue > max)
			{
				max = timePointsData[r][n].bestValue;
				bestData = timePointsData[r][n].bestData;
			}

			sumWorst += timePointsData[r][n].worstValue;
			if (timePointsData[r][n].worstValue < min)
			{
				min = timePointsData[r][n].worstValue;
				worstData = timePointsData[r][n].worstData;
			}

			sumAvg += timePointsData[r][n].avgValue;
		}

		(*result->getTimePoints())[n].bestValue = sumBest / repetitions;
		(*result->getTimePoints())[n].worstValue = sumWorst / repetitions;
		(*result->getTimePoints())[n].bestData = copyArray(timePointsData[0][n].bestData, bpp->getItems().size());
		(*result->getTimePoints())[n].worstData = copyArray(worstData, bpp->getItems().size());
		double avgAvg = sumAvg / repetitions;
		(*result->getTimePoints())[n].avgValue = avgAvg;

		sumAvg = 0.0;
		for (int r = 0; r < repetitions; ++r)
		{
			sumAvg += pow(timePointsData[r][n].avgValue - avgAvg, 2);
		}

		(*result->getTimePoints())[n].deviationValue = sqrt(sumAvg / repetitions);
	}

	QMetaObject::invokeMethod(bar, "succeeded");
	return result;
}