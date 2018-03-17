#pragma once

#include <QtCore\qstring.h>
#include "ISelection.h"

using namespace std;

class TournamentSelection : public ISelection
{
public:
	static const QString NAME;

	TournamentSelection() = delete;
	TournamentSelection(int s, double prob);

	void makeSelection(const vector<Individual>& pop, vector<Individual>& selected);

	~TournamentSelection();

private:
	int size;
	double* probabilities;
};