
#include "BackpackView.h"

#include <QtWidgets\qmessagebox.h>
#include <iostream>

BackpackView::BackpackView() : QGraphicsView(), scene(new QGraphicsScene), bpp(nullptr), individual(nullptr), OFFSET(1), CAP_HEIGHT(10)
{
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setScene(scene);
}

BackpackView::~BackpackView() {
	
}

void BackpackView::draw(BackpackProblem *bpp, bool *individual)
{
	this->bpp = bpp;
	this->individual = individual;
	calculateRatios();
	prepareScene();
	int pos = 0;
	itemsCount = 0;
	totalWeight = 0;
	leftoverPixels = 0;
	for (int i = 0; i < bpp->getItems().size(); ++i)
	{
		if (individual[i])
		{
			totalWeight += bpp->getItems().at(i).getWeight();
			++itemsCount;
		}
	}
	std::cout << "cap: " << bpp->getBackpackCapacity() << "  <-->  weight: " << totalWeight << "  <-->  items: " << itemsCount << std::endl;

	totalWeight = totalWeight > bpp->getBackpackCapacity() ? totalWeight : bpp->getBackpackCapacity();

	for (int i = 0; i < bpp->getItems().size(); ++i)
		if (individual[i])
			pos += drawItem(bpp->getItems()[i], pos);

	drawBackpackCap();
	scene->update(scene->sceneRect());
}

void BackpackView::clear()
{
	scene->clear();
}

void BackpackView::resizeEvent(QResizeEvent * e)
{
	if(bpp != nullptr)
		draw(bpp, individual);
}

void BackpackView::prepareScene()
{
	scene->clear();
	sceneWidth = this->geometry().width();
	sceneHeight = this->geometry().height() - 5;
	scene->setSceneRect(0, 0, sceneWidth, sceneHeight);
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

int BackpackView::drawItem(const Item& item, int pos)
{
	int height = toSceneHeight(item.getWeight());
	int additionalPixels = (int)leftoverPixels;
	leftoverPixels = leftoverPixels - (int)leftoverPixels;
	scene->addRect(QRect(0, sceneHeight - pos - height - additionalPixels, sceneWidth, height + additionalPixels), Qt::NoPen, QBrush(ratioToRGB(item.getValue() / (double)item.getWeight())));
	return height + additionalPixels + OFFSET;
}

void BackpackView::drawBackpackCap()
{
	int height = sceneHeight - sceneHeight * (bpp->getBackpackCapacity() / (double)totalWeight);
	scene->addRect(QRect(0, height, sceneWidth, CAP_HEIGHT), Qt::NoPen, QBrush(Qt::black));
}

int BackpackView::toSceneHeight(int weight)
{
	double height = (sceneHeight - (itemsCount - 1) * OFFSET) * (weight / (double)totalWeight);
	leftoverPixels += height - (int)height;
	return height;
}

QColor BackpackView::ratioToRGB(double ratio)
{
	if (minRatio == maxRatio)
	{
		return QColor(0, 255, 0);
	}
	else
	{
		int value = (ratio - minRatio) / (maxRatio - minRatio) * 510;
		return QColor(value <= 255 ? 255 : 510 - value, value >= 255 ? 255 : value, 0);
	}
}
