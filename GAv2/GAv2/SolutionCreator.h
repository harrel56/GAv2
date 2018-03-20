#pragma once
#include "BackpackProblem.h"
#include "GASolver.h"

#include <QStackedWidget>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlistwidget.h>
#include <QtWidgets\qtreewidget.h>
#include <QtWidgets\qgroupbox.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qlineedit.h>
#include <QtWidgets\qformlayout.h>
#include <QtWidgets\qradiobutton.h>

class MainWidget;

class SolutionPage : public QWidget
{
	Q_OBJECT

public:
	SolutionPage(QWidget *parent = Q_NULLPTR);
	~SolutionPage();

	virtual void refreshData(QVector<BackpackProblem*> *data);
	virtual void setMainWidget(MainWidget *parent);

signals:
	void changeIndex(int);
	void solveClicked(/*Solution sol*/);
};

class ProblemPage : public SolutionPage
{
	Q_OBJECT


public:
	ProblemPage(QWidget *parent = Q_NULLPTR);

	void refreshData(QVector<BackpackProblem*> *data);
	BackpackProblem *getSelectedProblem();
	void setMainWidget(MainWidget *parent);

private:
	void onNextButtonClicked();
	void onGenerateButtonClicked();
	QString getValidationError();

	MainWidget *mainWidget;
	QVector<BackpackProblem*> *data;
	BackpackProblem *selectedProblem;

	QGroupBox *problemBox;
	QGroupBox *generateBox;

	QVBoxLayout *problemVLayout;
	QFormLayout *generateFormLayout;
	QVBoxLayout *generateVLayout;
	QGridLayout *mainGridLayout;

	QTreeWidget *problemList;

	QLabel *nameLabel;
	QLabel *capacityLabel;
	QLabel *countLabel;
	QLabel *valueMaxLabel;
	QLabel *weightMaxLabel;
	QLineEdit *nameEdit;
	QLineEdit *capacityEdit;
	QLineEdit *countEdit;
	QLineEdit *valueMaxEdit;
	QLineEdit *weightMaxEdit;
	QPushButton *generateButton;

	QPushButton *nextButton;

};

class BasicPage : public SolutionPage
{
	Q_OBJECT


public:
	BasicPage(QWidget *parent = Q_NULLPTR);

	GASolver *getSelectedSolver();

private:
	void onNextButtonClicked();
	QString getValidationError();

	GASolver *selectedSolver;

	QGroupBox *mutationGroup;
	QGroupBox *crossoverGroup;
	QGroupBox *generalGroup;
	QGroupBox *validationGroup;

	QFormLayout *mutationFormLayout;
	QFormLayout *crossoverFormLayout;
	QFormLayout *generalFormLayout;
	QVBoxLayout *validationVLayout;
	QGridLayout *mainGridLayout;

	QLabel *mRateLabel;
	QLabel *cRateLabel;
	QLabel *pointsLabel;
	QLabel *populationLabel;
	QLabel *generationLabel;
	QLabel *repetitionLabel;

	QLineEdit *mRateEdit;
	QLineEdit *cRateEdit;
	QLineEdit *pointsEdit;
	QLineEdit *populationEdit;
	QLineEdit *generationEdit;
	QLineEdit *repetitionEdit;
	QLineEdit *functionEdit;

	QRadioButton *validRadio;
	QRadioButton *functionRadio;

	QPushButton *previousButton;
	QPushButton *nextButton;

};

class SelectionPage : public SolutionPage
{
	Q_OBJECT

public:
	SelectionPage(QWidget *parent = Q_NULLPTR);

	ISelection *getSelectedSelection();

private:
	void onListSelectionChange();
	void onSolveButtonClicked();

	QString getTournamentValidationError();
	QString getTestValidationError();

	ISelection *selectedSelection;

	QGroupBox *selectionGroup;
	QGroupBox *paramGroup;

	QFormLayout *paramFormLayout;
	QHBoxLayout *innerHLayout;
	QHBoxLayout *buttonHLayout;
	QGridLayout *mainGridLayout;

	QListWidget *methodList;

	QPushButton *previousButton;
	QPushButton *finishButton;

	QStackedWidget *stackedWidget;

	//Tournament
	QFormLayout *formLayout;
	QLabel *sizeLabel;
	QLineEdit *sizeEdit;
	QLabel *probLabel;
	QLineEdit *probEdit;

	//Test
	QLabel *testLabel;

};

class SolutionCreator : public QStackedWidget
{
	Q_OBJECT

public:
	SolutionCreator(QWidget * parent = Q_NULLPTR);
	~SolutionCreator();

	void setMainWidget(MainWidget *parent);
	void refreshData();

protected:
	void closeEvent(QCloseEvent *event);

private:
	void onSolveClicked();

	MainWidget *mainWidget;

	ProblemPage *page1;
	BasicPage *page2;
	SelectionPage *page3;

signals:
	void onClose();
	
};
