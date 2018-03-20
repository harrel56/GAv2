#pragma once

#include "ISelection.h"

#include <QtCore\qvector.h>
#include <QtCore\qstring.h>

struct TimePoint;

class SolutionData
{
public:
	SolutionData(ISelection *selection, int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions);

	const QString& getName();
	void setName(const QString& name);
	const QString& getSelectionName();
	const QVector<std::pair<QString, QString>>& getSelectionParams();
	int getPopSize();
	int getGenerationCount();
	double getMutationChance();
	double getCrossChance();
	int getCrossPoints();
	int getRepetitions();
	QVector<TimePoint> *getTimePoints();

	~SolutionData();

private:
	QString name;
	QString selectionName;
	QVector<std::pair<QString, QString>> selectionParams;
	int popSize;
	int generationCount;
	double mutationChance;
	double crossChance;
	int crossPoints;
	int repetitions;
	QVector<TimePoint> *timePoints;

};

struct TimePoint
{
	TimePoint() = default;
	TimePoint(int bVal, double avgVal, int wVal, bool *bData, bool *wData);

	int bestValue;
	double avgValue;
	int worstValue;
	double deviationValue;

	bool *bestData;
	bool *worstData;

};
