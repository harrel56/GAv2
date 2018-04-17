#include "TreeItemForPlot.h"

TreeItemForPlot::TreeItemForPlot(const QString& fullName) : fullName(fullName), internalState(false), graph(nullptr)
{

}

TreeItemForPlot::~TreeItemForPlot()
{

}

bool TreeItemForPlot::hasCheckStateChanged()
{
	return checkState(1) == Qt::Checked ? !internalState : internalState;
}

void TreeItemForPlot::setStateUnchanged()
{
	if (checkState(1) == Qt::Checked)
		internalState = true;
	else
		internalState = false;
}

const QString & TreeItemForPlot::getFullName()
{
	return fullName;
}

BackpackProblem * TreeItemForPlot::getProblem()
{
	return bpp;
}

void TreeItemForPlot::setProblem(BackpackProblem * bpp)
{
	this->bpp = bpp;
}

QCPGraph * TreeItemForPlot::getGraph()
{
	return graph;
}

void TreeItemForPlot::setGraph(QCPGraph * graph)
{
	this->graph = graph;
}

const QVector<double>& TreeItemForPlot::getData()
{
	return this->data;
}

void TreeItemForPlot::setData(QVector<double>&& data)
{
	this->data = std::move(data);
}

const QVector<bool*>& TreeItemForPlot::getIndividuals()
{
	return individuals;
}

void TreeItemForPlot::setIndividuals(QVector<bool*>&& individuals)
{
	this->individuals = std::move(individuals);
}
