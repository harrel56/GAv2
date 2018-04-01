#include "TreeItemForPlot.h"

TreeItemForPlot::TreeItemForPlot()
{

}

TreeItemForPlot::~TreeItemForPlot()
{

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
