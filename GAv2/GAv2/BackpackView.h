#pragma once

#include "BackpackProblem.h"

#include <QtWidgets\qgraphicsview>

class BackpackView : public QGraphicsView {
	Q_OBJECT

public:
	BackpackView();
	~BackpackView();

	void draw(BackpackProblem *bpp, bool *individual);
	void clear();

	const int OFFSET;
	const int CAP_HEIGHT;

protected:
	void resizeEvent(QResizeEvent *e);

private:
	void prepareScene();
	void calculateRatios();
	int drawItem(const Item& item, int pos);
	void drawBackpackCap();
	int toSceneHeight(int weight);
	QColor ratioToRGB(double ratio);

	QGraphicsScene *scene;
	BackpackProblem *bpp;
	bool *individual;
	double minRatio;
	double maxRatio;
	int sceneWidth;
	int sceneHeight;
	int itemsCount;
	int totalWeight;
	double leftoverPixels;
};
