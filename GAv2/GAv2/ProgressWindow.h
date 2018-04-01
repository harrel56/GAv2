#pragma once
#include <QWidget>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qprogressbar.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qgridlayout.h>

class ProgressWindow : public QWidget
{
	Q_OBJECT

public:
	ProgressWindow(QWidget * parent = Q_NULLPTR);
	~ProgressWindow();

	void succeeded();

	void setRepetition(int r);
	void setGeneration(int g);
	void setMaxValue(int max);
	void setValue(int val);

signals:
	void cancel();

private:
	QGridLayout *layout;

	QLabel *repLabel;
	QLabel *repValLabel;
	QLabel *genLabel;
	QLabel *genValLabel;

	QProgressBar *bar;

	QPushButton *cancelButton;
	
};
