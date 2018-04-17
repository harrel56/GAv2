#pragma once

#include <QtCore\qstring.h>
#include "ISelection.h"

using namespace std;

class GASolver;

class StochasticUniformSelection : public ISelection
{
public:
	static const QString NAME;

	void makeSelection(const vector<Individual>& pop, vector<Individual>& selected);
	QVector<pair<QString, QString>> getParams();
	const QString& getName();

private:

};
