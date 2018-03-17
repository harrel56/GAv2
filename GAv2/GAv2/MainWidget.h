#pragma once

#include "SolutionCreator.h"
#include "DBManager.h"

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

private:

	//DB
	DBManager dbManager;
	QVector<BackpackProblem*> *data;

	//Widgets
	QTreeWidget *treeWidget;

	QGraphicsView *backpackView;
	QGraphicsView *plotView;

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

