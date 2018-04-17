#include "MainWidget.h"
#include "TreeItemForPlot.h"

#include <iostream>
#include <QtWidgets\qmessagebox.h>
#include <QtWidgets\qheaderview.h>
#include <QtWidgets\qcolordialog.h>

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent), treeWidget(new QTreeWidget()), backpackView(new BackpackView), plotView(new QCustomPlot),
	clearButton(new QPushButton("Clear plot")), removeButton(new QPushButton("Remove")), createButton(new QPushButton("Create new GA solution")),
	buttonsVLayout(new QVBoxLayout()), leftVLayout(new QVBoxLayout()), mainHLayout(new QHBoxLayout(this)),
	solutionCreator(nullptr),
	dbManager("QSQLITE")
{
	this->setMinimumSize(1280, 720);
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(QColor(255, 255, 255)));

	//plotView->setStyleSheet("border:none;");
	//backpackView->setStyleSheet("border:none;");

	connect(clearButton, &QPushButton::clicked, [&]()
	{
		refreshTreeWidget();
		backpackView->clear();
	});

	connect(createButton, &QPushButton::clicked, [&]()
	{
		solutionCreator = new SolutionCreator;
		connect(solutionCreator, &SolutionCreator::onClose, [&]() { delete solutionCreator; solutionCreator = nullptr; });
		solutionCreator->setMainWidget(this);
		solutionCreator->refreshData();
		solutionCreator->show();
	});

	buttonsVLayout->addWidget(clearButton);
	buttonsVLayout->addWidget(removeButton);
	buttonsVLayout->addWidget(createButton);

	leftVLayout->addWidget(treeWidget);
	leftVLayout->addLayout(buttonsVLayout);

	mainHLayout->addLayout(leftVLayout);
	mainHLayout->addWidget(backpackView);
	mainHLayout->addWidget(plotView);

	mainHLayout->setStretch(0, 1);
	mainHLayout->setStretch(1, 1);
	mainHLayout->setStretch(2, 4);

	if (!dbManager.initializeDatabase())
	{
		QMessageBox::critical(this, "Error", "Database could not be initialized!");
	}
	
	treeWidget->setHeaderLabels({ "Name", "Value" });
	treeWidget->setColumnCount(2);
	treeWidget->setIndentation(15);
	connect(treeWidget, &QTreeWidget::itemExpanded, [&]() {treeWidget->resizeColumnToContents(0);
															treeWidget->resizeColumnToContents(1); });
	connect(treeWidget, &QTreeWidget::itemCollapsed, [&]() {treeWidget->resizeColumnToContents(0);
															treeWidget->resizeColumnToContents(1); });
	connect(treeWidget, &QTreeWidget::itemChanged, this, &MainWidget::onTreeItemChanged);
	data = dbManager.fetchProblems();
	refreshTreeWidget();

	plotView->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	plotView->yAxis->setLabel("Fitness");
	plotView->xAxis->setLabel("Generation");

}

MainWidget::~MainWidget()
{

}

const DBManager& MainWidget::getDBManager() const { return this->dbManager; }

QVector<BackpackProblem*> *MainWidget::getData() const { return this->data; }

void MainWidget::refreshTreeWidget()
{
	plotView->clearPlottables();
	plotView->legend->setVisible(false);
	plotView->replot();
	backpackView->clear();
	treeWidget->clear();

	for (BackpackProblem *bpp : *data)
	{
		QTreeWidgetItem *topItem = new QTreeWidgetItem;
		topItem->setText(0, bpp->getName());
		treeWidget->addTopLevelItem(topItem);

		QTreeWidgetItem *capItem = new QTreeWidgetItem;
		capItem->setText(0, "Capacity:");
		capItem->setText(1, QString::number(bpp->getBackpackCapacity()));
		topItem->addChild(capItem);

		QTreeWidgetItem *countItem = new QTreeWidgetItem;
		countItem->setText(0, "Items:");
		countItem->setText(1, QString::number(bpp->getItems().size()));
		topItem->addChild(countItem);

		QTreeWidgetItem *solutionItem = new QTreeWidgetItem;
		solutionItem->setText(0, "Solutions");
		topItem->addChild(solutionItem);

		for (SolutionData *solution : bpp->getSolutions())
		{
			QTreeWidgetItem *s1Item = new QTreeWidgetItem;
			s1Item->setText(0, solution->getName());
			solutionItem->addChild(s1Item);

			QTreeWidgetItem *plotItem = new QTreeWidgetItem;
			plotItem->setText(0, "Plotting");
			s1Item->addChild(plotItem);

			TreeItemForPlot *bestItem = new TreeItemForPlot(bpp->getName() + ":" + solution->getName().split(' ')[1] + ":Best");
			bestItem->setText(0, "Best:");
			bestItem->setFlags(plotItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
			bestItem->setCheckState(1, Qt::Unchecked);
			bestItem->setData(solution->getBestData());
			bestItem->setProblem(bpp);
			bestItem->setIndividuals(solution->getBestIndividuals());
			plotItem->addChild(bestItem);

			TreeItemForPlot *avgItem = new TreeItemForPlot(bpp->getName() + ":" + solution->getName().split(' ')[1] + ":Average");
			avgItem->setText(0, "Average:");
			avgItem->setFlags(plotItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
			avgItem->setCheckState(1, Qt::Unchecked);
			avgItem->setData(solution->getAverageData());
			plotItem->addChild(avgItem);

			TreeItemForPlot *worstItem = new TreeItemForPlot(bpp->getName() + ":" + solution->getName().split(' ')[1] + ":Worst");
			worstItem->setText(0, "Worst:");
			worstItem->setFlags(plotItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
			worstItem->setCheckState(1, Qt::Unchecked);
			worstItem->setData(solution->getWorstData());
			plotItem->addChild(worstItem);

			TreeItemForPlot *devItem = new TreeItemForPlot(bpp->getName() + ":" + solution->getName().split(' ')[1] + ":Deviation");
			devItem->setText(0, "Deviation:");
			devItem->setFlags(plotItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
			devItem->setCheckState(1, Qt::Unchecked);
			devItem->setData(solution->getDeviationData());
			plotItem->addChild(devItem);

			QTreeWidgetItem *popItem = new QTreeWidgetItem;
			popItem->setText(0, "Population:");
			popItem->setText(1, QString::number(solution->getPopSize()));
			s1Item->addChild(popItem);

			QTreeWidgetItem *genItem = new QTreeWidgetItem;
			genItem->setText(0, "Generations:");
			genItem->setText(1, QString::number(solution->getGenerationCount()));
			s1Item->addChild(genItem);

			QTreeWidgetItem *mutItem = new QTreeWidgetItem;
			mutItem->setText(0, "Mutation:");
			mutItem->setText(1, QString::number(solution->getMutationChance()));
			s1Item->addChild(mutItem);

			QTreeWidgetItem *crossItem = new QTreeWidgetItem;
			crossItem->setText(0, "Crossover:");
			crossItem->setText(1, QString::number(solution->getCrossChance()));
			s1Item->addChild(crossItem);

			QTreeWidgetItem *pointsItem = new QTreeWidgetItem;
			pointsItem->setText(0, "Points:");
			pointsItem->setText(1, QString::number(solution->getCrossPoints()));
			s1Item->addChild(pointsItem);

			QTreeWidgetItem *repItem = new QTreeWidgetItem;
			repItem->setText(0, "Repetitions:");
			repItem->setText(1, QString::number(solution->getRepetitions()));
			s1Item->addChild(repItem);

			QTreeWidgetItem *selectionItem = new QTreeWidgetItem;
			selectionItem->setText(0, solution->getSelectionName());
			s1Item->addChild(selectionItem);

			for (auto param : solution->getSelectionParams())
			{
				QTreeWidgetItem *paramItem = new QTreeWidgetItem;
				paramItem->setText(0, param.first);
				paramItem->setText(1, param.second);
				selectionItem->addChild(paramItem);
			}

			QTreeWidgetItem *functionItem = new QTreeWidgetItem;
			functionItem->setText(0, "Penalty function:");
			functionItem->setText(1, "totalValue * " + QString::number(solution->getPenaltyFixed()) + " + " + QString::number(solution->getPenaltyParam()) + " * (totalWeight - capacity)^" + QString::number(solution->getPenaltyPower()));
			s1Item->addChild(functionItem);

		}
	}
	
	treeWidget->resizeColumnToContents(0);
	treeWidget->resizeColumnToContents(1);
}

void MainWidget::onTreeItemChanged(QTreeWidgetItem *item, int column)
{
	auto plotItem = dynamic_cast<TreeItemForPlot*>(item);
	if (plotItem->hasCheckStateChanged())
	{
		plotItem->setStateUnchanged();
		if (plotItem->checkState(column) == Qt::Checked)
		{
			QColor color = QColorDialog::getColor(Qt::blue, this);
			if (color.isValid())
			{
				QVector<double> keys(plotItem->getData().size());
				for (int i = 0; i < keys.size(); ++i)
				{
					keys[i] = i;
				}
				plotItem->setGraph(plotView->addGraph());
				plotItem->getGraph()->setData(keys, plotItem->getData(), true);
				plotItem->getGraph()->setName(plotItem->getFullName());
				plotItem->getGraph()->setPen(QPen(color));
				plotItem->setBackgroundColor(1, color);

				if (plotItem->text(0) == "Best:")
				{
					plotItem->getGraph()->setSelectable(QCP::stSingleData);
					plotItem->getGraph()->setScatterStyle(QCPScatterStyle::ssCircle);
					std::cout << plotItem->getIndividuals().size() << std::endl;
					connect(plotItem->getGraph(), static_cast<void (QCPAbstractPlottable::*)(const QCPDataSelection&)>(&QCPAbstractPlottable::selectionChanged), [=](const QCPDataSelection& data)
					{
						if (data.dataRangeCount() > 0)
						{
							bool *individual = plotItem->getIndividuals().at(data.dataRange().begin());
							backpackView->draw(plotItem->getProblem(), individual);
						}
						else
						{
							backpackView->clear();
						}
					});
				}
			}
			else
			{
				plotItem->setCheckState(1, Qt::Unchecked);
				plotItem->setStateUnchanged();
				return;
			}
		}
		else if (plotItem->getGraph() != nullptr)
		{
			plotView->removeGraph(plotItem->getGraph());
			plotItem->setGraph(nullptr);
			plotItem->setBackgroundColor(1, Qt::white);
		}
		else
		{
			return;
		}


		if (plotView->graphCount() == 0)
			plotView->legend->setVisible(false);
		else
			plotView->legend->setVisible(true);

		plotView->rescaleAxes();
		plotView->replot();
	}
}
