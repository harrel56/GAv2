
#include "BackpackView.h"

#include <iostream>

BackpackView::BackpackView() : QGraphicsView(), scene(new QGraphicsScene)
{
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setStyleSheet("border:none;");
	this->setScene(scene);
}

BackpackView::~BackpackView() {
	
}

void BackpackView::draw(BackpackProblem *bpp, bool *individual)
{
	this->bpp = bpp;
	calculateRatios();
	prepareScene();
	int pos = 0;
	for (int i = 0; i < bpp->getItems().size(); ++i)
	{
		if (individual[i])
		{
			pos += drawItem(bpp->getItems()[i], pos);
		}
	}

	scene->update(scene->sceneRect());
}

void BackpackView::prepareScene()
{
	scene->clear();
	scene->setSceneRect(this->geometry());
	//scene->setBackgroundBrush(QBrush(QColor(230, 230, 230)));
}

void BackpackView::calculateRatios()
{
	minRatio = std::numeric_limits<double>::max();
	maxRatio = std::numeric_limits<double>::min();

	for (const Item& item : bpp->getItems())
	{
		double ratio = item.getValue() / (double)item.getWeight();
		if (maxRatio < ratio)
			maxRatio = ratio;
		if (minRatio > ratio)
			minRatio = ratio;
	}
}

int BackpackView::drawItem(const Item & item, int pos)
{
	scene->addRect(QRect(scene->width(), pos + scene->height(), scene->width(), toSceneHeight(item.getWeight())), QPen(), QBrush(QColor(155, 0, 0)));
	std::cout << item.getWeight() << " : " << toSceneHeight(item.getWeight()) << std::endl;
	return toSceneHeight(item.getWeight()) + 5;
}

int BackpackView::toSceneHeight(int weight)
{
	return scene->height() * (weight / (double) bpp->getBackpackCapacity());
}
