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

		QTreeWidgetItem *solveItem = new QTreeWidgetItem;
		solveItem->setText(0, "Solves");
		topItem->addChild(solveItem);

		QTreeWidgetItem *s1Item = new QTreeWidgetItem;
		s1Item->setText(0, "solve1");
		solveItem->addChild(s1Item);

		QTreeWidgetItem *s2Item = new QTreeWidgetItem;
		s2Item->setText(0, "solve2");
		solveItem->addChild(s2Item);
	}
	
	treeWidget->resizeColumnToContents(0);
	treeWidget->resizeColumnToContents(1);
}
