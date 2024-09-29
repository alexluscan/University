#include <assert.h>
#include <iostream>

#include "ListIterator.h"
#include "SortedIndexedList.h"

using namespace std;

bool relation1(TComp e1, TComp e2) {
	if (e1 <= e2) {
		return true;
	}
	else {
		return false;
	}
}

bool relation2(TComp e1, TComp e2) {
	if (e1 < e2) {
		return true;
	}
	else {
		return false;
	}
}

void testAll(){
	SortedIndexedList list = SortedIndexedList(relation1);
	assert(list.size() == 0);
	assert(list.isEmpty());
    list.add(1);
    assert(list.size() == 1);
    assert(!list.isEmpty());
    ListIterator iterator = list.iterator();
    assert(iterator.valid());
    iterator.first();
    assert(iterator.getCurrent() == 1);
    iterator.next();
    assert(!iterator.valid());
    iterator.first();
    assert(iterator.valid());
    assert(list.search(1) == 0);
    assert(list.remove(0) == 1);
    assert(list.size() == 0);
    assert(list.isEmpty());
}

void testExtra() {
	SortedIndexedList list = SortedIndexedList(relation2);
	std::cout << "EXTRA\n";
	list.add(1);
	list.add(2);
	list.add(3);
	list.add(5);
	list.add(6);
	assert(list.lastIndexOf(6) == 4);
	list.add(4);
	assert(list.lastIndexOf(6) == 5);
	assert(list.lastIndexOf(7) == -1);
	std::cout << "\n";
}