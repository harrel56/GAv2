#include "MainWidget.h"

#include <QtWidgets\qmessagebox.h>
#include <QtWidgets\qheaderview.h>

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent), treeWidget(new QTreeWidget()), backpackView(new QGraphicsView()), plotView(new QGraphicsView()),
	setButton(new QPushButton("Set active")), removeButton(new QPushButton("Remove")), saveButton(new QPushButton("Save to file")), loadButton(new QPushButton("Load from file")), createButton(new QPushButton("Create new GA solution")),
	buttonsVLayout(new QVBoxLayout()), leftVLayout(new QVBoxLayout()), mainHLayout(new QHBoxLayout(this)),
	solutionCreator(nullptr),
	dbManager("QSQLITE")
{
	this->setMinimumSize(1280, 720);
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(QColor(255, 255, 255)));

	//plotView->setStyleSheet("border:none;");
	//backpackView->setStyleSheet("border:none;");

	connect(createButton, &QPushButton::clicked, [&]()
	{
		solutionCreator = new SolutionCreator;
		connect(solutionCreator, &SolutionCreator::onClose, [&]() { delete solutionCreator; solutionCreator = nullptr; });
		solutionCreator->setMainWidget(this);
		solutionCreator->refreshData();
		solutionCreator->show();
	});

	buttonsVLayout->addWidget(setButton);
	buttonsVLayout->addWidget(removeButton);
	buttonsVLayout->addWidget(saveButton);
	buttonsVLayout->addWidget(loadButton);
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
	
	//treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	treeWidget->setHeaderLabels({ "Name", "Value" });
	treeWidget->setColumnCount(2);
	connect(treeWidget, &QTreeWidget::itemExpanded, [&]() {treeWidget->resizeColumnToContents(0);
															treeWidget->resizeColumnToContents(1); });
	connect(treeWidget, &QTreeWidget::itemCollapsed, [&]() {treeWidget->resizeColumnToContents(0);
															treeWidget->resizeColumnToContents(1); });
	data = dbManager.fetchProblems();
	refreshTreeWidget();
}

MainWidget::~MainWidget()
{

}

const DBManager& MainWidget::getDBManager() const { return this->dbManager; }

QVector<BackpackProblem*> *MainWidget::getData() const { return this->data; }

void MainWidget::refreshTreeWidget()
{
	treeWidget->clear();

	for (BackpackProblem *bpp : *data)
	{
		QTreeWidgetItem *topItem = new QTreeWidgetItem;
		topItem->setText(0, bpp->getName());
		treeWidget->addTopLevelItem(topItem);

		QTreeWidgetItem *capItem = new QTreeWidgetItem;
		capItem->setText(0, "Capacity:");
		capItem->setText(1, QString::number(bpp->getBackpackCapacity()));
		capItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
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
		}
	}
	
	treeWidget->resizeColumnToContents(0);
	treeWidget->resizeColumnToContents(1);
}
