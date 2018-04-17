#pragma once

#include "Individual.h"

using namespace std;

class GASolver;

class ISelection
{
public:
	virtual void makeSelection(const vector<Individual>& pop, vector<Individual>& selected) = 0;
	virtual QVector<pair<QString, QString>> getParams() = 0;
	virtual const QString& getName() = 0;
	void setSolver(GASolver *solver) { this->solver = solver; }

protected:
	GASolver *solver;

private:

};