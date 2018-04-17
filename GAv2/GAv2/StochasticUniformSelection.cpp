#include "StochasticUniformSelection.h"

#include <random>
#include <iostream>

const QString StochasticUniformSelection::NAME = "Stochastic uniform selection";

void StochasticUniformSelection::makeSelection(const vector<Individual>& pop, vector<Individual>& selected)
{
	double *fitnessTab = new double[pop.size()];
	for (int i = 0; i < pop.size(); ++i)
		fitnessTab[i] = pop[i].getTotalValue();

	double min = *std::min_element(fitnessTab, fitnessTab + pop.size()) - 1;
	int fitnessSum = 0;
	for (int i = 0; i < pop.size(); ++i)
		fitnessSum += fitnessTab[i] - min;

	double interval = fitnessSum / (double)pop.size();
	random_device rd;
	default_random_engine rng(rd());
	uniform_real_distribution<double> rand(0, interval);
	int val = rand(rng);

	for (int j = 0; j < selected.size(); ++j)
	{
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
		val += interval;
	}
	delete fitnessTab;
}

QVector<pair<QString, QString>> StochasticUniformSelection::getParams()
{
	return QVector<pair<QString, QString>>();
}

const QString& StochasticUniformSelection::getName() { return StochasticUniformSelection::NAME; }


