#include "SolutionCreator.h"
#include "MainWidget.h"
#include "TournamentSelection.h"

#include <QtWidgets\qmessagebox.h>

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
	nameEdit(new QLineEdit), capacityEdit(new QLineEdit), countEdit(new QLineEdit), valueMaxEdit(new QLineEdit), weightMaxEdit(new QLineEdit), generateButton(new QPushButton("Generate")), nextButton(new QPushButton("Next")),
	mainGridLayout(new QGridLayout(this)), problemVLayout(new QVBoxLayout), generateFormLayout(new QFormLayout), generateVLayout(new QVBoxLayout)
{
	problemVLayout->addWidget(problemList);
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
		solveItem->setText(0, "Solves:");
		solveItem->setText(1, "temp");
		solveItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);
		topItem->addChild(solveItem);
	}
	problemList->resizeColumnToContents(0);
	problemList->resizeColumnToContents(1);
}

void ProblemPage::setMainWidget(MainWidget *parent) { this->mainWidget = parent; }

void ProblemPage::onNextButtonClicked()
{
	if (problemList->currentItem()->isSelected())
	{
		selectedIndex = problemList->currentIndex().row();
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
	mRateLabel(new QLabel("Rate")), cRateLabel(new QLabel("Rate")), pointsLabel(new QLabel("Points")), populationLabel(new QLabel("Population size")), generationLabel(new QLabel("Generations count")), repetitionLabel(new QLabel("Repetitions")),
	mRateEdit(new QLineEdit), cRateEdit(new QLineEdit), pointsEdit(new QLineEdit), populationEdit(new QLineEdit), generationEdit(new QLineEdit), repetitionEdit(new QLineEdit), functionEdit(new QLineEdit),
	validRadio(new QRadioButton("Only valid solutions")), functionRadio(new QRadioButton("Penalty function")), previousButton(new QPushButton("Back")), nextButton(new QPushButton("Next")),
	mutationFormLayout(new QFormLayout), crossoverFormLayout(new QFormLayout), generalFormLayout(new QFormLayout), validationVLayout(new QVBoxLayout), mainGridLayout(new QGridLayout(this))
{
	mutationFormLayout->addRow(mRateLabel, mRateEdit);
	crossoverFormLayout->addRow(cRateLabel, cRateEdit);
	crossoverFormLayout->addRow(pointsLabel, pointsEdit);
	generalFormLayout->addRow(populationLabel, populationEdit);
	generalFormLayout->addRow(generationLabel, generationEdit);
	generalFormLayout->addRow(repetitionLabel, repetitionEdit);

	validationVLayout->addWidget(validRadio);
	validationVLayout->addWidget(functionRadio);
	validationVLayout->addWidget(functionEdit);

	mutationGroup->setLayout(mutationFormLayout);
	crossoverGroup->setLayout(crossoverFormLayout);
	generalGroup->setLayout(generalFormLayout);
	validationGroup->setLayout(validationVLayout);

	mainGridLayout->addWidget(mutationGroup, 0, 0);
	mainGridLayout->addWidget(crossoverGroup, 1, 0);
	mainGridLayout->addWidget(generalGroup, 0, 1);
	mainGridLayout->addWidget(validationGroup, 1, 1);
	mainGridLayout->addWidget(previousButton, 2, 0);
	mainGridLayout->addWidget(nextButton, 2, 1);

	connect(nextButton, &QPushButton::clicked, this, &BasicPage::onNextButtonClicked);
	connect(previousButton, &QPushButton::clicked, [&]() {emit changeIndex(0); });
}

void BasicPage::onNextButtonClicked()
{
	QString errMsg = "";//getValidationError();
	if (errMsg.isEmpty())
	{
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
	double tempDouble = mRateEdit->text().toDouble(&ok);
	if (!ok || tempDouble < 0.0 || tempDouble > 1.0)
		return QString("Mutation rate has to be a real number from the range 0.0-1.0");
	tempDouble = cRateEdit->text().toDouble(&ok);
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
	if(!validRadio->isChecked() && !functionRadio->isChecked())
		return QString("Validation method has to be chosen!");
}

SelectionPage::SelectionPage(QWidget * parent)
	: SolutionPage(parent),
	selectionGroup(new QGroupBox("Selection method")), paramGroup(new QGroupBox("Parameters")),
	methodList(new QListWidget),
	previousButton(new QPushButton("Back")), finishButton(new QPushButton("Solve")),
	stackedWidget(new QStackedWidget),
	formLayout(new QFormLayout), sizeLabel(new QLabel("Size")), sizeEdit(new QLineEdit), probLabel(new QLabel("Pobability")), probEdit(new QLineEdit),
	testLabel(new QLabel("ELOASLODALSODALO")),
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
	stackedWidget->addWidget(wrapper1);
	stackedWidget->addWidget(testLabel);
	stackedWidget->addWidget(new QWidget);
	stackedWidget->setCurrentIndex(stackedWidget->count() - 1);

	QListWidgetItem *tournamentItem = new QListWidgetItem(TournamentSelection::NAME, methodList);
	QListWidgetItem *tournamentItem2 = new QListWidgetItem("TEST", methodList);
	connect(methodList, &QListWidget::itemSelectionChanged, this, &SelectionPage::onListSelectionChange);

	connect(finishButton, &QPushButton::clicked, this, &SelectionPage::onSolveButtonClicked);
	connect(previousButton, &QPushButton::clicked, [&]() {emit changeIndex(1); });
}

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
			errMsg = getTournamentValidationError();
			break;
		case 1:
			errMsg = getTestValidationError();
			break;
		default:
			break;
		}
		if (errMsg.isEmpty())
		{
			emit solveClicked();
		}
		else
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
	return "ELDOKA";
}

QString SelectionPage::getTestValidationError()
{
	return "";
}

SolutionCreator::SolutionCreator(QWidget * parent) : QStackedWidget(parent), page1(new ProblemPage()), page2(new BasicPage()), page3(new SelectionPage())
{
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
	this->setWindowModality(Qt::ApplicationModal);
	this->setMaximumSize(QSize(480, 320));
	this->setMinimumSize(QSize(480, 320));

	this->addWidget(page1);
	this->addWidget(page2);
	this->addWidget(page3);

	connect(page1, SIGNAL(changeIndex(int)), this, SLOT(setCurrentIndex(int)));
	connect(page2, SIGNAL(changeIndex(int)), this, SLOT(setCurrentIndex(int)));
	connect(page3, SIGNAL(changeIndex(int)), this, SLOT(setCurrentIndex(int)));
	connect(page3, SIGNAL(solveClicked()), this, SLOT(close()));
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
