#pragma once

using namespace std;

class Item
{
public:
	Item();
	Item(int w, int v);

	int getWeight() const;
	int getValue() const;

private:
	int weight;
	int value;

};
