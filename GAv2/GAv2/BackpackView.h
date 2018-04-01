#pragma once

#include "BackpackProblem.h"

#include <QtWidgets\qgraphicsview>

class BackpackView : public QGraphicsView {
	Q_OBJECT

public:
	BackpackView();
	~BackpackView();

	void draw(BackpackProblem *bpp, bool *individual);

private:
	void prepareScene();
	void calculateRatios();
	int drawItem(const Item& item, int pos);
	int toSceneHeight(int weight);

	QGraphicsScene *scene;
	BackpackProblem *bpp;
	double minRatio;
	double maxRatio;
};
