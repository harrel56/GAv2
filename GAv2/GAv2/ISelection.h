#pragma once

#include "Individual.h"

using namespace std;

class ISelection
{
public:
	virtual void makeSelection(const vector<Individual>& pop, vector<Individual>& selected) = 0;
	virtual QVector<pair<QString, QString>> getParams() = 0;
	virtual QString getName() = 0;

private:

};