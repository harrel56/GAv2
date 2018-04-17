#include "RouletteWheelSelection.h"

#include <random>
#include <iostream>

const QString RouletteWheelSelection::NAME = "Roulette wheel selection";

void RouletteWheelSelection::makeSelection(const vector<Individual>& pop, vector<Individual>& selected)
{
	double *fitnessTab = new double[pop.size()];
	for (int i = 0; i < pop.size(); ++i)
		fitnessTab[i] = pop[i].getTotalValue();

	double min = *std::min_element(fitnessTab, fitnessTab + pop.size()) - 1;
	int fitnessSum = 0;
	for (int i = 0; i < pop.size(); ++i)
		fitnessSum += fitnessTab[i] - min;

	random_device rd;
	default_random_engine rng(rd());
	uniform_int_distribution<int> rand(0, fitnessSum);

	for (int j = 0; j < selected.size(); ++j)
	{
		int val = rand(rng);
		fitnessSum = 0;
		for (int i = 0; i < pop.size(); ++i)
		{
			fitnessSum += fitnessTab[i] - min;
			if (val <= fitnessSum)
			{
				selected[j] = pop[i];
				break;
			}
		}
	}
	delete fitnessTab;
}

QVector<pair<QString, QString>> RouletteWheelSelection::getParams()
{
	return QVector<pair<QString, QString>>();
}

const QString& RouletteWheelSelection::getName() { return RouletteWheelSelection::NAME; }


