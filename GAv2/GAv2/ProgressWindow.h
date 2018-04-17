#pragma once
#include <QDialog>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qprogressbar.h>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qgridlayout.h>

class ProgressWindow : public QDialog
{
	Q_OBJECT

public:
	ProgressWindow(QWidget * parent = Q_NULLPTR);
	~ProgressWindow();

	void setRepetition(int r);
	void setGeneration(int g);
	void setValue(int val);

	bool isCancelled();

public slots:
	void setMaxValue(int max);
	void update(int rep, int gen, int val);
	void succeeded();
	void failed();

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

	bool cancelled;
	
};
