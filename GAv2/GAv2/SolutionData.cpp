#include "SolutionData.h"

SolutionData::SolutionData(ISelection * selection, int popSize, int generationCount, double mutationChance, double crossChance, int crossPoints, int repetitions) : 
	selectionName(selection->getName()), selectionParams(selection->getParams()), popSize(popSize), generationCount(generationCount), mutationChance(mutationChance), crossChance(crossChance), crossPoints(crossPoints), repetitions(repetitions),
	timePoints(new QVector<TimePoint>(generationCount))
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

SolutionData::~SolutionData()
{
	delete timePoints;
}

TimePoint::TimePoint(int bVal, double avgVal, int wVal, bool * bData, bool * wData) :
	bestValue(bVal), avgValue(avgVal), worstValue(wVal), bestData(bData), worstData(wData)
{

}
