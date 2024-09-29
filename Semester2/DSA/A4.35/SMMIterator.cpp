#include "SMMIterator.h"
#include "SortedMultiMap.h"


void SMMIterator::merge(int left, int middle, int right)
{
	int subArray1 = middle - left + 1;
	int subArray2 = right - middle;

	TElem* leftArray = new TElem[subArray1],
		* rightArray = new TElem[subArray2];

	for (int i = 0; i < subArray1; ++i)
		leftArray[i] = elementsSorted[left + i];

	for (int i = 0; i < subArray2; ++i)
		rightArray[i] = elementsSorted[middle + 1 + i];

	int indexLeft = 0, indexRight = 0, indexMerged = left;

	while (indexLeft < subArray1 && indexRight < subArray2)
	{
		if (map.rel(leftArray[indexLeft].first, rightArray[indexRight].first))
			elementsSorted[indexMerged++] = leftArray[indexLeft++];
		else
			elementsSorted[indexMerged++] = rightArray[indexRight++];
	}

	while (indexLeft < subArray1)
		elementsSorted[indexMerged++] = leftArray[indexLeft++];
	while (indexRight < subArray2)
		elementsSorted[indexMerged++] = rightArray[indexRight++];

	delete[] leftArray;
	delete[] rightArray;
}

void SMMIterator::mergeSort(int left, int right)
{
	if (left >= right)
		return;

	int middle = (left + right) / 2;
	mergeSort(left, middle);
	mergeSort(middle + 1, right);
	merge(left, middle, right);
}

SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d){
	this->current = 0;
	this->elementsSorted = new TElem[d.size()];
	int j = 0;
	for(int i = 0; i < this->map.capacity; i++)
		if (map.elements[i].first != NULL_TKEY)
		{
			this->elementsSorted[j] = map.elements[i];
			j++;
		}

	mergeSort(0, j - 1);
}
//BC=WC=AC = nlog(n) ( from merge sort)


void SMMIterator::first(){
	this->current = 0;
}

void SMMIterator::next(){
	if (this->current >= this->map.size())
		throw exception();
	this->current++;
}

bool SMMIterator::valid() const{
	if (this->current < this->map.size())
		return true;
	return false;
}

TElem SMMIterator::getCurrent() const{
	if (this->current >= this->map.size())
		throw exception();
	return this->elementsSorted[this->current];

}

SMMIterator::~SMMIterator() {
	delete[] this->elementsSorted;
}

void SMMIterator::previous()
{
	if (!this->valid())
		throw exception();
	if (this->current == 0)
	{
		this->current = this->map.size();
		return;
	}
	this->current--;
}

void SMMIterator::jumpForward(int k) {
	if (k <= 0 || this->current + k >= this->map.size())
		throw exception();
	else
		this->current += k;
}