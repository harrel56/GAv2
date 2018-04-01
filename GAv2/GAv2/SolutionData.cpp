#include "SolutionData.h"
#include <iostream>

SolutionData::SolutionData(ISelection * selection, int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions, double penaltyFixed, double penaltyParam, double penaltyPower) :
	selectionName(selection->getName()), selectionParams(selection->getParams()), popSize(popSize), generationCount(generationCount), mutationChance(mutationChance), crossChance(crossChance), crossPoints(crossPoints), repetitions(repetitions),
	timePoints(new QVector<TimePoint>(generationCount)), penaltyFixed(penaltyFixed), penaltyParam(penaltyParam), penaltyPower(penaltyPower)
{

}

SolutionData::SolutionData(const QString & name, const QString & selectionName, QVector<std::pair<QString, QString>> selectionParams, QVector<TimePoint>* timePoints,
	int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions, double penaltyFixed, double penaltyParam, double penaltyPower) :
	name(name), selectionName(selectionName), selectionParams(selectionParams), timePoints(timePoints),
	popSize(popSize), generationCount(generationCount), mutationChance(mutationChance), crossChance(crossChance), crossPoints(crossPoints), repetitions(repetitions),
	penaltyFixed(penaltyFixed), penaltyParam(penaltyParam), penaltyPower(penaltyPower)
{
}

const QString& SolutionData::getName()
{
	return name;
}

void SolutionData::setName(const QString& name)
{
	this->name = name;
}

const QString & SolutionData::getSelectionName()
{
	return selectionName;
}

const QVector<std::pair<QString, QString>>& SolutionData::getSelectionParams()
{
	return selectionParams;
}

int SolutionData::getPopSize()
{
	return popSize;
}

int SolutionData::getGenerationCount()
{
	return generationCount;
}

double SolutionData::getMutationChance()
{
	return mutationChance;
}

double SolutionData::getCrossChance()
{
	return crossChance;
}

int SolutionData::getCrossPoints()
{
	return crossPoints;
}

int SolutionData::getRepetitions()
{
	return repetitions;
}

QVector<TimePoint>* SolutionData::getTimePoints()
{
	return timePoints;
}

double SolutionData::getPenaltyFixed()
{
	return penaltyFixed;
}

double SolutionData::getPenaltyParam()
{
	return penaltyParam;
}

double SolutionData::getPenaltyPower()
{
	return penaltyPower;
}

QVector<double> SolutionData::getBestData()
{
	QVector<double> result;
	result.reserve(timePoints->size());
	for (const TimePoint& tp : *timePoints)
	{
		result.append(tp.bestValue);
	}
	return result;
}

QVector<double> SolutionData::getAverageData()
{
	QVector<double> result;
	result.reserve(timePoints->size());
	for (const TimePoint& tp : *timePoints)
	{
		result.append(tp.avgValue);
	}
	return result;
}

QVector<double> SolutionData::getWorstData()
{
	QVector<double> result;
	result.reserve(timePoints->size());
	for (const TimePoint& tp : *timePoints)
	{
		result.append(tp.worstValue);
	}
	return result;
}

QVector<double> SolutionData::getDeviationData()
{
	QVector<double> result;
	result.reserve(timePoints->size());
	for (const TimePoint& tp : *timePoints)
	{
		result.append(tp.deviationValue);
	}
	return result;
}

QVector<bool*> SolutionData::getBestIndividuals()
{
	QVector<bool*> result;
	result.reserve(timePoints->size());
	for (const TimePoint& tp : *timePoints)
	{
		result.append(tp.bestData);
	}
	return result;
}

SolutionData::~SolutionData()
{
	delete timePoints;
}

TimePoint::TimePoint(TimePoint&& tp) : 
	bestValue(tp.bestValue), avgValue(tp.avgValue), worstValue(tp.worstValue), deviationValue(tp.deviationValue), bestData(tp.bestData), worstData(tp.worstData)
{
	tp.bestData = nullptr;
	tp.worstData = nullptr;
}

TimePoint::TimePoint(int bVal, double avgVal, int wVal, bool * bData, bool * wData) :
	bestValue(bVal), avgValue(avgVal), worstValue(wVal), bestData(bData), worstData(wData)
{

}

TimePoint::TimePoint(int bVal, double avgVal, int wVal, double deviationValue, bool * bData, bool * wData) :
	bestValue(bVal), avgValue(avgVal), worstValue(wVal), deviationValue(deviationValue), bestData(bData), worstData(wData)
{
}

const TimePoint& TimePoint::operator=(TimePoint&& tp)
{
	bestValue = tp.bestValue;
	avgValue = tp.avgValue;
	worstValue = tp.worstValue;
	deviationValue = tp.deviationValue;
	bestData = tp.bestData;
	worstData = tp.worstData;
	tp.bestData = nullptr;
	tp.worstData = nullptr;
	return *this;
}

TimePoint::~TimePoint()
{
	delete bestData;
	delete worstData;
}
