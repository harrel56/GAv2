#pragma once

#include "SolutionCreator.h"
#include "DBManager.h"
#include "BackpackView.h"
#include "qcustomplot.h"

#include <QtCore\qvector.h>
#include <QtWidgets\qwidget.h>
#include <QtWidgets\qtreewidget.h>
#include <QtWidgets\qgraphicsview.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlayout.h>

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);
	~MainWidget();

	const DBManager& getDBManager() const;
	QVector<BackpackProblem*> *getData() const;

	void refreshTreeWidget();
	void onTreeItemChanged(QTreeWidgetItem *item, int column);

private:

	//DB
	DBManager dbManager;
	QVector<BackpackProblem*> *data;

	//Widgets
	QTreeWidget *treeWidget;

	BackpackView *backpackView;
	QCustomPlot *plotView;

	QPushButton *setButton;
	QPushButton *removeButton;
	QPushButton *saveButton;
	QPushButton *loadButton;
	QPushButton *createButton;

	//Layouts
	QVBoxLayout *buttonsVLayout;
	QVBoxLayout *leftVLayout;
	QHBoxLayout *mainHLayout;

	//Pop-out window
	SolutionCreator *solutionCreator;
};

