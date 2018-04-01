#include "GASolver.h"
#include "Individual.h"
#include "ProbabilityGenerator.h"
#include "TournamentSelection.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <algorithm>
#include <math.h>
#include <QtCore\qcoreapplication.h>

using namespace std;

GASolver::GASolver(int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions, double funParam1, double funParam2, double funParam3) :
	popSize(popSize), generationCount(generationCount), mutationChance(mutationChance), crossChance(crossChance), crossPoints(crossPoints), repetitions(repetitions), cancelled(false),
	funParam1(funParam1), funParam2(funParam2), funParam3(funParam3)
{

}

void GASolver::setSelection(ISelection *selection) { this->selection = selection; }

void GASolver::setCancelled(bool state)
{
	this->cancelled = state;
}

bool *GASolver::copyArray(bool *data, int size)
{
	bool *result = new bool[size];
	//std::copy(data, data + size, result);
	for (int i = 0; i < size; ++i)
	{
		result[i] = data[i];
	}
	return result;
}

SolutionData *GASolver::solve(BackpackProblem *bpp, ProgressWindow *bar)
{
	//Initialize 2 sets of individuals to avoid further individual object creations
	Individual::setPenaltyParams(funParam1, funParam2, funParam3);
	auto population = Individual::initializePop(popSize, bpp);
	auto selectedPopulation = Individual::initializePop(popSize, bpp);

	//Get custom RNG
	ProbabilityGenerator& rand = ProbabilityGenerator::getInstance();

	SolutionData *result = new SolutionData(selection, popSize, generationCount, mutationChance, crossChance, crossPoints, repetitions, funParam1, funParam2, funParam3);
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
				if (i + 1 < selectedPopulation.size() && rand() < crossChance)
					selectedPopulation[i].crossover(selectedPopulation[i + 1], crossPoints);
			}

			//Do the mutations on each individual
			for (unsigned int i = 0; i < selectedPopulation.size(); ++i)
				selectedPopulation[i].mutate(mutationChance);

			//Swap containers for next iteration (selectedPop will be overwritten)
			selectedPopulation.swap(population);

			if (n % 20 == 0)
			{
				bar->setRepetition(r);
				bar->setGeneration(n);
				bar->setValue(r * generationCount + n);
				QCoreApplication::processEvents(QEventLoop::AllEvents, 10000);
			}
		}

		if (cancelled)
		{
			cancelled = false;
			return nullptr;
		}
	}

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
		(*result->getTimePoints())[n].bestData = copyArray(bestData, bpp->getItems().size());
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

	bar->succeeded();
	return result;
}