#include <iostream>

#include "Item.h"

using namespace std;

Item::Item() {}

Item::Item(int w, int v) : weight(w), value(v) {}

int Item::getWeight() const { return weight; }

int Item::getValue() const { return value; }
