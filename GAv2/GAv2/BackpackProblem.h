#pragma once
#include "Item.h"

#include <QtCore\qvector.h>
#include <QtCore\qstring.h>

class BackpackProblem
{
public:
	static BackpackProblem *generateNew(const QString& name, int cap, int itemCount, int maxWeight, int maxValue);

	BackpackProblem() = delete;
	BackpackProblem(int id, const QString& name, int cap);
	BackpackProblem(int id, const QString& name, int cap, QVector<Item>&& ritem);

	void setId(int id);
	int getId() const;
	const QString& getName() const;
	int getBackpackCapacity() const;
	const QVector<Item>& getItems() const;

	//~BackpackProblem();

private:
	int id;
	QString name;
	int backpackCapacity;
	QVector<Item> items;

};
