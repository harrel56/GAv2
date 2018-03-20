#include "ProgressWindow.h"

ProgressWindow::ProgressWindow(QWidget * parent) : QWidget(parent),
	layout(new QGridLayout(this)),
	repLabel(new QLabel("Repetition: ")), repValLabel(new QLabel), genLabel(new QLabel("Generation: ")), genValLabel(new QLabel),
	bar(new QProgressBar), cancelButton(new QPushButton("Cancel"))
{
	this->setWindowTitle("Genetic algorithm progress");
	this->setWindowFlags(Qt::WindowTitleHint | Qt::WindowStaysOnTopHint | Qt::MSWindowsFixedSizeDialogHint);
	this->setWindowModality(Qt::ApplicationModal);

	bar->setMinimumWidth(300);
	bar->setTextVisible(false);

	layout->addWidget(repLabel, 0, 0);
	layout->addWidget(repValLabel, 0, 1);
	layout->addWidget(genLabel, 0, 2);
	layout->addWidget(genValLabel, 0, 3);
	layout->addWidget(bar, 1, 0, 1, 4);
	layout->addWidget(cancelButton, 2, 2, 1, 2);

	connect(cancelButton, &QPushButton::clicked, [&]() {emit cancel(); });
}

ProgressWindow::~ProgressWindow() 
{
	
}

void ProgressWindow::setRepetition(int r)
{
	repValLabel->setText(QString::number(r));

}
void ProgressWindow::setGeneration(int g)
{
	genValLabel->setText(QString::number(g));
}

void ProgressWindow::setMaxValue(int max)
{
	bar->setMaximum(max);
}

void ProgressWindow::setValue(int val)
{
	bar->setValue(val);
}
