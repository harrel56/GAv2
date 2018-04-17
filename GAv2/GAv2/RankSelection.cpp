#include "RankSelection.h"

#include <random>
#include <iostream>

const QString RankSelection::NAME = "Rank selection";

void RankSelection::makeSelection(const vector<Individual>& pop, vector<Individual>& selected)
{
	double *fitnessTab = new double[pop.size()];
	for (int i = 0; i < pop.size(); ++i)
		fitnessTab[i] = pop[i].getTotalValue();

	int *indexTab = new int[pop.size()];
	int fitnessSum = 0;
	for (int i = 0; i < pop.size(); ++i)
	{
		indexTab[i] = i;
		fitnessSum += indexTab[i];
	}

	std::sort(indexTab, indexTab + pop.size(), [&fitnessTab](int a1, int a2)
	{
		return fitnessTab[a1] < fitnessTab[a2];
	});	

	random_device rd;
	default_random_engine rng(rd());
	uniform_int_distribution<int> rand(0, fitnessSum);

	for (int j = 0; j < selected.size(); ++j)
	{
		int val = rand(rng);
		fitnessSum = 0;
		for (int i = 0; i < pop.size(); ++i)
		{
			fitnessSum += indexTab[i];
			if (val <= fitnessSum)
			{
				selected[j] = pop[indexTab[i]];
				break;
			}
		}
	}
	delete fitnessTab;
	delete indexTab;
}

QVector<pair<QString, QString>> RankSelection::getParams()
{
	return QVector<pair<QString, QString>>();
}

const QString& RankSelection::getName() { return RankSelection::NAME; }


