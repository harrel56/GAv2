#pragma once

#include "qcustomplot.h"
#include "BackpackProblem.h"

#include <QtWidgets\qtreewidget.h>
#include <vector>


class TreeItemForPlot : public QTreeWidgetItem
{

public:
	TreeItemForPlot(const QString& fullName);
	~TreeItemForPlot();

	bool hasCheckStateChanged();
	void setStateUnchanged();
	const QString& getFullName();
	BackpackProblem *getProblem();
	void setProblem(BackpackProblem *bpp);
	QCPGraph *getGraph();
	void setGraph(QCPGraph *graph);
	const QVector<double>& getData();
	void setData(QVector<double>&& data);
	const QVector<bool*>& getIndividuals();
	void setIndividuals(QVector<bool*>&& individuals);

private:

	bool internalState;
	QString fullName;
	BackpackProblem *bpp;
	QCPGraph *graph;
	QVector<double> data;
	QVector<bool*> individuals;
};

