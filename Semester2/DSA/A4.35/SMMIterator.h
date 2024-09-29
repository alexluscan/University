#pragma once

#include "SortedMultiMap.h"


class SMMIterator{
	friend class SortedMultiMap;
private:
	//DO NOT CHANGE THIS PART
	const SortedMultiMap& map;
	SMMIterator(const SortedMultiMap& map);

	TElem* elementsSorted;
	int current;

	void merge(int left, int middle, int right);
	void mergeSort(int left, int right);

public:
	void previous();
	void first();
	void next();
	bool valid() const;
   	TElem getCurrent() const;
	void jumpForward(int k);
	~SMMIterator();
};

