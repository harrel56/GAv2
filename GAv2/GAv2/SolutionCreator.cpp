#include "SolutionCreator.h"
#include "MainWidget.h"
#include "ProgressWindow.h"
#include "TournamentSelection.h"
#include "RouletteWheelSelection.h"
#include "RankSelection.h"
#include "StochasticUniformSelection.h"
#include "GASolver.h"

#include <QtWidgets\qmessagebox.h>
#include <QtConcurrent\qtconcurrentrun.h>
#include <QtCore\qstringlist.h>
#include <iostream>

SolutionPage::SolutionPage(QWidget * parent) : QWidget(parent) {}

SolutionPage::~SolutionPage()
{
	this->disconnect();
}

void SolutionPage::refreshData(QVector<BackpackProblem*> *data) { }

void SolutionPage::setMainWidget(MainWidget *parent) { }

ProblemPage::ProblemPage(QWidget * parent)
	: SolutionPage(parent),
	problemBox(new QGroupBox("Problem")), generateBox(new QGroupBox("Generate")),
	problemList(new QTreeWidget),
	nameLabel(new QLabel("Name")), capacityLabel(new QLabel("Capacity")), countLabel(new QLabel("Item count")), valueMaxLabel(new QLabel("Max. value")), weightMaxLabel(new QLabel("Max. weight")),
	nameEdit(new QLineEdit), capacityEdit(new QLineEdit), countEdit(new QLineEdit), valueMaxEdit(new QLineEdit), weightMaxEdit(new QLineEdit),
	loadButton(new QPushButton("Load from CSV")), generateButton(new QPushButton("Generate")), nextButton(new QPushButton("Next")),
	mainGridLayout(new QGridLayout(this)), problemVLayout(new QVBoxLayout), generateFormLayout(new QFormLayout), generateVLayout(new QVBoxLayout)
{
	problemVLayout->addWidget(problemList);
	problemVLayout->addWidget(loadButton);
	problemBox->setLayout(problemVLayout);

	generateFormLayout->addRow(nameLabel, nameEdit);
	generateFormLayout->addRow(capacityLabel, capacityEdit);
	generateFormLayout->addRow(countLabel, countEdit);
	generateFormLayout->addRow(valueMaxLabel, valueMaxEdit);
	generateFormLayout->addRow(weightMaxLabel, weightMaxEdit);

	generateVLayout->addLayout(generateFormLayout);
	generateVLayout->addWidget(generateButton);

	generateBox->setLayout(generateVLayout);

	mainGridLayout->addWidget(problemBox, 0, 0);
	mainGridLayout->addWidget(generateBox, 0, 1);
	mainGridLayout->addWidget(nextButton, 1, 1);

	mainGridLayout->setColumnStretch(0, 1);
	mainGridLayout->setColumnStretch(1, 1);

	connect(nextButton, &QPushButton::clicked, this, &ProblemPage::onNextButtonClicked);
	connect(generateButton, &QPushButton::clicked, this, &ProblemPage::onGenerateButtonClicked);
	connect(loadButton, &QPushButton::clicked, this, &ProblemPage::onLoadButtonClicked);

	problemList->setHeaderLabels({ "Name", "Value" });
	problemList->setColumnCount(2);
	connect(problemList, &QTreeWidget::itemExpanded, [&]() {problemList->resizeColumnToContents(0);
															problemList->resizeColumnToContents(1); });
	connect(problemList, &QTreeWidget::itemCollapsed, [&]() {problemList->resizeColumnToContents(0);
															problemList->resizeColumnToContents(1); });

}

void ProblemPage::refreshData(QVector<BackpackProblem*> *data)
{
	this->data = data;
	problemList->clear();
	for (const BackpackProblem* bpp : *data)
	{
		QTreeWidgetItem *topItem = new QTreeWidgetItem;
		topItem->setText(0, bpp->getName());
		problemList->addTopLevelItem(topItem);

		QTreeWidgetItem *capItem = new QTreeWidgetItem;
		capItem->setText(0, "Capacity:");
		capItem->setText(1, QString::number(bpp->getBackpackCapacity()));
		capItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
		topItem->addChild(capItem);

		QTreeWidgetItem *countItem = new QTreeWidgetItem;
		countItem->setText(0, "Items:");
		countItem->setText(1, QString::number(bpp->getItems().size()));
		countItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
		topItem->addChild(countItem);

		QTreeWidgetItem *solveItem = new QTreeWidgetItem;
		solveItem->setText(0, "Solutions:");
		solveItem->setText(1, QString::number(bpp->getSolutions().size()));
		solveItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
		topItem->addChild(solveItem);
	}
	problemList->resizeColumnToContents(0);
	problemList->resizeColumnToContents(1);
}

BackpackProblem *ProblemPage::getSelectedProblem() { return selectedProblem; }

void ProblemPage::setMainWidget(MainWidget *parent) { this->mainWidget = parent; }

void ProblemPage::onNextButtonClicked()
{
	if (problemList->currentItem() != nullptr && problemList->currentItem()->isSelected())
	{
		selectedProblem = data->at(problemList->currentIndex().row());
		emit changeIndex(1);
	}
	else
		QMessageBox::warning(this, "Warning", "You have to select a problem from the list!");
}

void ProblemPage::onGenerateButtonClicked()
{
	QString errMsg = getValidationError();
	if (errMsg.isEmpty())
	{
		data->append(BackpackProblem::generateNew(nameEdit->text(),
			capacityEdit->text().toInt(), countEdit->text().toInt(),
			valueMaxEdit->text().toInt(), weightMaxEdit->text().toInt()));
		mainWidget->getDBManager().insertProblem(data->back());
		qSort(data->begin(), data->end(), [](BackpackProblem *bpp1, BackpackProblem *bpp2) { return bpp1->getName() < bpp2->getName(); });
		nameEdit->clear();
		capacityEdit->clear();
		countEdit->clear();
		valueMaxEdit->clear();
		weightMaxEdit->clear();
		QMessageBox::information(this, "Problem generation", "Problem has been succesfully generated!");
		refreshData(data);
		mainWidget->refreshTreeWidget();
	}
	else
	{
		QMessageBox::warning(this, "Validation error!", errMsg);
	}
}

void ProblemPage::onLoadButtonClicked()
{
	QFile file(QFileDialog::getOpenFileName(this, "Open file", QString(), "CSV files (*.csv)"));
	QString errMsg;
	if (file.open(QIODevice::ReadOnly))
	{
		bool ok;
		QString name = file.readLine().split('\n').first();
		if (name.size() < 3 || name.size() > 20)
		{
			errMsg = "First row of file should contain name of the problem! (3-20 letters)";
		}
		int capacity = QString(file.readLine().split('\n').first()).toInt(&ok);
		if (!ok || capacity <= 1)
		{
			errMsg = "Second row of file should contain the backpack capacity!";
		}

		QVector<Item> items;
		QString line = file.readLine();
		while (errMsg.isEmpty() && !line.isEmpty()) 
		{
			QStringList temp = line.split('\n').first().split(',');

			int value = temp.first().toInt(&ok);
			if(!ok || value <= 0)
				errMsg = "Incorrect items format!";
			int weight = temp.last().toInt(&ok);
			if (!ok || weight <= 0)
				errMsg = "Incorrect items format!";

			if (errMsg.isEmpty())
			{
				items.append(Item(weight, value));
				line = file.readLine();
			}
		}

		if (errMsg.isEmpty())
		{
			data->append(new BackpackProblem(-1, name, capacity, std::move(items), QVector<SolutionData*>()));
			mainWidget->getDBManager().insertProblem(data->back());
			qSort(data->begin(), data->end(), [](BackpackProblem *bpp1, BackpackProblem *bpp2) { return bpp1->getName() < bpp2->getName(); });
			QMessageBox::information(this, "Problem loaded", "Problem loaded from file successfully!");
			refreshData(data);
			mainWidget->refreshTreeWidget();
		}
		else
		{
			QMessageBox::warning(this, "File parsing error", errMsg);
		}
	}
	else
	{
		QMessageBox::warning(this, "File opening error", "Selected file could not be opened!");
	}
}

QString ProblemPage::getValidationError()
{
	if (nameEdit->text().size() > 20 || nameEdit->text().size() < 3)
		return QString("Problem name has to be 3-20 letter string!");
	if (capacityEdit->text().toInt() <= 0)
		return QString("Backpack capacity has to be an integer from the range 1-2147483647");
	int count = countEdit->text().toInt();
	if (count <= 0 || count > 20000)
		return QString("Item count has to be an integer from the range 1-20000");
	if (valueMaxEdit->text().toInt() <= 0)
		return QString("Maximum value has to be an integer from the range 1-2147483647");
	if (weightMaxEdit->text().toInt() <= 0)
		return QString("Maximum weight has to be an integer from the range 1-2147483647");

	return QString();
}

BasicPage::BasicPage(QWidget * parent)
	: SolutionPage(parent),
	mutationGroup(new QGroupBox("Mutation")), crossoverGroup(new QGroupBox("Crossover")), generalGroup(new QGroupBox("General")), validationGroup(new QGroupBox("Solution validation")),
	mRateLabel(new QLabel("Probability")), cRateLabel(new QLabel("Probability")), pointsLabel(new QLabel("Points")), populationLabel(new QLabel("Population size")), generationLabel(new QLabel("Generations count")), repetitionLabel(new QLabel("Repetitions")),
	mRateEdit(new QLineEdit), cRateEdit(new QLineEdit), pointsEdit(new QLineEdit), populationEdit(new QLineEdit), generationEdit(new QLineEdit), repetitionEdit(new QLineEdit),
	fixedLabel1(new QLabel("Fixed penalty")), fixedLabel2(new QLabel("% of total value")), progressLabel1(new QLabel("Progressive penalty")), progressLabel2(new QLabel(" * (weight-capacity)^")),
	fixedEdit(new QLineEdit), progressEdit1(new QLineEdit), progressEdit2(new QLineEdit),
	previousButton(new QPushButton("Back")), nextButton(new QPushButton("Next")),
	fixedPenaltyLayout(new QHBoxLayout), progressPenaltyLayout(new QHBoxLayout),
	mutationFormLayout(new QFormLayout), crossoverFormLayout(new QFormLayout), generalFormLayout(new QFormLayout), validationFormLayout(new QFormLayout), mainGridLayout(new QGridLayout(this))
{
	//int defaultWidth = mRateEdit->sizeHint().width();
	//mRateEdit->setFixedWidth(defaultWidth / 2);
	//cRateEdit->setFixedWidth(defaultWidth / 2);
	//pointsEdit->setFixedWidth(defaultWidth / 2);
	//populationEdit->setFixedWidth(defaultWidth / 2);
	//generationEdit->setFixedWidth(defaultWidth / 2);
	//repetitionEdit->setFixedWidth(defaultWidth / 2);
	//fixedEdit->setFixedWidth(defaultWidth / 2);
	//progressEdit1->setFixedWidth(defaultWidth / 2);
	//progressEdit2->setFixedWidth(defaultWidth / 2);

	//mutationFormLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	//generalFormLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);

	mutationFormLayout->addRow(mRateLabel, mRateEdit);
	crossoverFormLayout->addRow(cRateLabel, cRateEdit);
	crossoverFormLayout->addRow(pointsLabel, pointsEdit);
	generalFormLayout->addRow(populationLabel, populationEdit);
	generalFormLayout->addRow(generationLabel, generationEdit);
	generalFormLayout->addRow(repetitionLabel, repetitionEdit);

	fixedPenaltyLayout->addWidget(fixedEdit);
	fixedPenaltyLayout->addWidget(fixedLabel2);

	progressPenaltyLayout->addWidget(progressEdit1);
	progressPenaltyLayout->addWidget(progressLabel2);
	progressPenaltyLayout->addWidget(progressEdit2);

	validationFormLayout->addRow(fixedLabel1, fixedPenaltyLayout);
	validationFormLayout->addRow(progressLabel1, progressPenaltyLayout);

	mutationGroup->setLayout(mutationFormLayout);
	crossoverGroup->setLayout(crossoverFormLayout);
	generalGroup->setLayout(generalFormLayout);
	validationGroup->setLayout(validationFormLayout);

	mainGridLayout->addWidget(mutationGroup, 0, 0);
	mainGridLayout->addWidget(crossoverGroup, 1, 0);
	mainGridLayout->addWidget(generalGroup, 0, 1, 2, 1);
	mainGridLayout->addWidget(validationGroup, 2, 0, 1, 2);
	mainGridLayout->addWidget(previousButton, 3, 0);
	mainGridLayout->addWidget(nextButton, 3, 1);

	connect(nextButton, &QPushButton::clicked, this, &BasicPage::onNextButtonClicked);
	connect(previousButton, &QPushButton::clicked, [&]() {emit changeIndex(0); });

	//init default values for tests
	mRateEdit->setText("0.01");
	cRateEdit->setText("0.9");
	pointsEdit->setText("1");
	populationEdit->setText("200");
	generationEdit->setText("100");
	repetitionEdit->setText("3");
	fixedEdit->setText("10.0");
	progressEdit1->setText("10.0");
	progressEdit2->setText("1");
}

GASolver *BasicPage::getSelectedSolver() { return selectedSolver; }

void BasicPage::onNextButtonClicked()
{
	QString errMsg = getValidationError();
	if (errMsg.isEmpty())
	{
		selectedSolver = new GASolver(populationEdit->text().toInt(), generationEdit->text().toInt(), mRateEdit->text().replace(',', '.').toDouble(),
										cRateEdit->text().replace(',', '.').toDouble(), pointsEdit->text().toInt(), repetitionEdit->text().toInt(),
										fixedEdit->text().toDouble(), progressEdit1->text().toDouble(), progressEdit2->text().toDouble());
		emit changeIndex(2);
	}
	else
	{
		QMessageBox::warning(this, "Validation error!", errMsg);
	}
}

QString BasicPage::getValidationError()
{
	bool ok;
	double tempDouble = mRateEdit->text().replace(',', '.').toDouble(&ok);
	if (!ok || tempDouble < 0.0 || tempDouble > 1.0)
		return QString("Mutation rate has to be a real number from the range 0.0-1.0");
	tempDouble = cRateEdit->text().replace(',', '.').toDouble(&ok);
	if (!ok || tempDouble < 0.0 || tempDouble > 1.0)
		return QString("Crossover probability has to be a real number from the range 0.0-1.0");
	int tempInt = pointsEdit->text().toInt();
	if (tempInt < 1 || tempInt > 1000)
		return QString("Crossover points has to be an integer from the range 1-1000");
	tempInt = populationEdit->text().toInt();
	if (tempInt < 1 || tempInt > 100000)
		return QString("Population size has to be an integer from the range 1-100000");
	tempInt = generationEdit->text().toInt();
	if (tempInt < 1 || tempInt > 100000)
		return QString("Generation count has to be an integer from the range 1-100000");
	tempInt = repetitionEdit->text().toInt();
	if (tempInt < 1 || tempInt > 1000)
		return QString("Repetitions count has to be an integer from the range 1-1000");
	tempDouble = fixedEdit->text().replace(',', '.').toDouble(&ok);
	if (!ok || tempDouble < 0.0 || tempDouble > 100.0)
			return QString("Fixed penalty has to be a real number from the range 0.0-100.0");
	tempDouble = progressEdit1->text().replace(',', '.').toDouble(&ok);
	if (!ok || tempDouble < 0.0 || tempDouble > 10000.0)
		return QString("Progressive penalty coefficient has to be a real number from the range 0.0-10000.0");
	tempDouble = progressEdit2->text().replace(',', '.').toDouble(&ok);
	if (!ok || tempDouble < 0.0 || tempDouble > 100.0)
		return QString("Progressive penalty power has to be a real number from the range 0.0-100.0");

	return QString();
}

SelectionPage::SelectionPage(QWidget * parent)
	: SolutionPage(parent),
	selectionGroup(new QGroupBox("Selection method")), paramGroup(new QGroupBox("Parameters")),
	methodList(new QListWidget),
	previousButton(new QPushButton("Back")), finishButton(new QPushButton("Solve")),
	stackedWidget(new QStackedWidget),
	formLayout(new QFormLayout), sizeLabel(new QLabel("Size")), sizeEdit(new QLineEdit), probLabel(new QLabel("Pobability")), probEdit(new QLineEdit),
	paramFormLayout(new QFormLayout), innerHLayout(new QHBoxLayout), buttonHLayout(new QHBoxLayout), mainGridLayout(new QGridLayout(this))
{

	paramFormLayout->addWidget(stackedWidget);

	paramGroup->setLayout(paramFormLayout);
	selectionGroup->setLayout(innerHLayout);

	innerHLayout->addWidget(methodList, 1);
	innerHLayout->addWidget(paramGroup, 1);

	buttonHLayout->addWidget(previousButton);
	buttonHLayout->addWidget(finishButton);

	mainGridLayout->addWidget(selectionGroup, 0, 0);
	mainGridLayout->addLayout(buttonHLayout, 1, 0);

	formLayout->addRow(sizeLabel, sizeEdit);
	formLayout->addRow(probLabel, probEdit);
	QWidget *wrapper1 = new QWidget;
	wrapper1->setLayout(formLayout);
	stackedWidget->addWidget(new QWidget);
	stackedWidget->addWidget(new QWidget);
	stackedWidget->addWidget(new QWidget);
	stackedWidget->addWidget(wrapper1);
	stackedWidget->addWidget(new QWidget);
	stackedWidget->setCurrentIndex(stackedWidget->count() - 1);

	QListWidgetItem *rankItem = new QListWidgetItem(RankSelection::NAME, methodList);
	QListWidgetItem *RWItem = new QListWidgetItem(RouletteWheelSelection::NAME, methodList);
	QListWidgetItem *stochasticItem = new QListWidgetItem(StochasticUniformSelection::NAME, methodList);
	QListWidgetItem *tournamentItem = new QListWidgetItem(TournamentSelection::NAME, methodList);
	connect(methodList, &QListWidget::itemSelectionChanged, this, &SelectionPage::onListSelectionChange);

	connect(finishButton, &QPushButton::clicked, this, &SelectionPage::onSolveButtonClicked);
	connect(previousButton, &QPushButton::clicked, [&]() {emit changeIndex(1); });

	//init default values for tests
	sizeEdit->setText("5");
	probEdit->setText("1.0");
}

ISelection *SelectionPage::getSelectedSelection() { return selectedSelection; }

void SelectionPage::onListSelectionChange()
{
	if (!methodList->selectedItems().isEmpty())
		stackedWidget->setCurrentIndex(methodList->currentRow());
	else
		stackedWidget->setCurrentIndex(stackedWidget->count() - 1);
}

void SelectionPage::onSolveButtonClicked()
{
	if (!methodList->selectedItems().isEmpty())
	{
		QString errMsg;
		switch (methodList->currentRow())
		{
		case 0:
			selectedSelection = new RankSelection;
			emit solveClicked();
			break;
		case 1:
			selectedSelection = new RouletteWheelSelection;
			emit solveClicked();
			break;
		case 2:
			selectedSelection = new StochasticUniformSelection;
			emit solveClicked();
			break;
		case 3:
			errMsg = getTournamentValidationError();
			if (errMsg.isEmpty())
			{
				selectedSelection = new TournamentSelection(sizeEdit->text().toInt(), probEdit->text().replace(',', '.').toDouble());
				emit solveClicked();
			}
			break;
		default:
			break;
		}

		if (!errMsg.isEmpty())
		{
			QMessageBox::warning(this, "Validation error", errMsg);
		}
	}
	else
	{
		QMessageBox::warning(this, "Validation error", "Selection method needs to be chosen!");
	}
}

QString SelectionPage::getTournamentValidationError()
{
	int tempInt = sizeEdit->text().toInt();
	if (tempInt <= 0 || tempInt > 100)
		return QString("Size has to be an integer from the range 1-100");
	bool ok;
	double tempDouble = probEdit->text().replace(',', '.').toDouble(&ok);
	if (!ok || tempDouble < 0.0 || tempDouble > 1.0)
		return QString("Probability has to be a real number from the range 0.0-1.0");

	return QString();
}

QString SelectionPage::getTestValidationError()
{
	return "";
}

SolutionCreator::SolutionCreator(QWidget * parent) : QStackedWidget(parent), page1(new ProblemPage()), page2(new BasicPage()), page3(new SelectionPage())
{
	this->setWindowFlags(Qt::WindowCloseButtonHint);
	this->setWindowModality(Qt::ApplicationModal);
	this->setMaximumSize(QSize(480, 320));
	this->setMinimumSize(QSize(480, 320));

	this->addWidget(page1);
	this->addWidget(page2);
	this->addWidget(page3);

	connect(page1, SIGNAL(changeIndex(int)), this, SLOT(setCurrentIndex(int)));
	connect(page2, SIGNAL(changeIndex(int)), this, SLOT(setCurrentIndex(int)));
	connect(page3, SIGNAL(changeIndex(int)), this, SLOT(setCurrentIndex(int)));
	connect(page3, &SelectionPage::solveClicked, this, &SolutionCreator::onSolveClicked);
	connect(&futureWatcher, &QFutureWatcher<SolutionData*>::finished, [&]() { onSolutionCreated(futureWatcher.result()); });
}

SolutionCreator::~SolutionCreator() {
	this->disconnect();
	delete page1;
	delete page2;
	delete page3;
}

void SolutionCreator::setMainWidget(MainWidget *parent) 
{
	this->mainWidget = parent;
	page1->setMainWidget(parent);
}

void SolutionCreator::refreshData()
{
	page1->refreshData(this->mainWidget->getData());
}

void SolutionCreator::closeEvent(QCloseEvent *event)
{
	emit onClose();
}

void SolutionCreator::onSolveClicked()
{
	GASolver *solver = page2->getSelectedSolver();
	solver->setSelection(page3->getSelectedSelection());
	BackpackProblem *problem = page1->getSelectedProblem();
	this->setWindowModality(Qt::NonModal);

	//Need to be created on GUI thread
	ProgressWindow *bar = new ProgressWindow();
	bar->show();
	
	futureResult = QtConcurrent::run([=]() {return solver->solve(problem, bar); });
	futureWatcher.setFuture(futureResult);
}

void SolutionCreator::onSolutionCreated(SolutionData *data)
{
	this->setWindowModality(Qt::ApplicationModal);
	if (data != nullptr)
	{
		data->setName("Solution " + QString::number(page1->getSelectedProblem()->getSolutions().size() + 1));
		page1->getSelectedProblem()->addSolution(data);
		mainWidget->getDBManager().insertSolution(page1->getSelectedProblem()->getId(), page1->getSelectedProblem()->getItems().size(), data);
		mainWidget->refreshTreeWidget();
		page1->refreshData(mainWidget->getData());
	}
}
