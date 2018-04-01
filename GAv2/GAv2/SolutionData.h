#pragma once

#include "ISelection.h"

#include <QtCore\qvector.h>
#include <QtCore\qstring.h>

struct TimePoint;

class SolutionData
{
public:
	SolutionData(ISelection *selection, int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions, double penaltyFixed, double penaltyParam, double penaltyPower);
	SolutionData(const QString& name, const QString& selectionName, QVector<std::pair<QString, QString>> selectionParams, QVector<TimePoint> *timePoints,
		int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions, double penaltyFixed, double penaltyParam, double penaltyPower);

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
	double getPenaltyFixed();
	double getPenaltyParam();
	double getPenaltyPower();

	QVector<double> getBestData();
	QVector<double> getAverageData();
	QVector<double> getWorstData();
	QVector<double> getDeviationData();

	QVector<bool*> getBestIndividuals();

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

	//Penalty function
	double penaltyFixed;
	double penaltyParam;
	double penaltyPower;

};

struct TimePoint
{
	TimePoint() = default;
	TimePoint(const TimePoint& tp) = default;
	TimePoint(TimePoint&& tp);
	TimePoint(int bVal, double avgVal, int wVal, bool *bData, bool *wData);
	TimePoint(int bVal, double avgVal, int wVal, double deviationValue, bool *bData, bool *wData);

	const TimePoint& operator=(TimePoint&& tp);

	~TimePoint();

	int bestValue;
	double avgValue;
	int worstValue;
	double deviationValue;

	bool *bestData;
	bool *worstData;

};
