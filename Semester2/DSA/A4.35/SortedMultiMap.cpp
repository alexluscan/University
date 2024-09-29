#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include <exception>
using namespace std;

SortedMultiMap::SortedMultiMap(Relation r) {
	this->rel = r;
	this-> capacity = 2;
	this->SMMsize = 0;
	this->next = new int[capacity];
	this->elements = new TElem[capacity];
	this->firstEmpty = 0;
	for (int i = 0; i < this->capacity; i++)
	{
		this->next[i] = -1;
		this->elements[i] = NULL_TELEM;
	}
}
//BC=WC=TC= Theta(n)


void SortedMultiMap::add(TKey c, TValue v) {
	
	if (this->SMMsize == this->capacity)
		resize();

	int pos = this->hash(c);

	if(this->elements[pos].first == NULL_TKEY)
	{
		//position is empty, there's no collision
		this->elements[pos].first = c;
		this->elements[pos].second = v;
		this->next[pos] = -1;
		if (pos == this->firstEmpty)
		{
			//update first empty of it was occupied
			while (this->firstEmpty < this->capacity && this->elements[firstEmpty].first != NULL_TKEY)
				this->firstEmpty++;
		}

	}
	else
	{
		//find where to insert 
		int current = pos;
		int prev = -1;
		while (current != -1 && this->rel(this->elements[current].first, c))
		{
			prev = current;
			current = this->next[current];
		}

		if (current == pos)
		{
			//if it should be the first on that position, move the old one to the first empty and put the new one on the pos
			int newEmpty = this->firstEmpty;
			this->elements[newEmpty] = this->elements[pos];
			this->elements[pos].first = c;
			this->elements[pos].second = v;
			this->next[pos] = newEmpty;
		}
		else
		{
			int newEmpty = this->firstEmpty;
			this->elements[newEmpty].first = c;
			this->elements[newEmpty].second = v;
			if (prev != -1)
			{
				this->next[prev] = newEmpty;
			}
			this->next[newEmpty] = current;
		}
		while (this->firstEmpty < this->capacity && this->elements[firstEmpty].first != NULL_TKEY)
			this->firstEmpty++;


	}
	this->SMMsize++;
}
//BC = Theta(1)
//WC = Theta(n)
//TC =  O(n)

vector<TValue> SortedMultiMap::search(TKey c) const {
	
	vector<TValue> values;
	int pos = this->hash(c);
	while (pos != -1)
	{
		if(this->elements[pos].first == c)
			values.push_back(this->elements[pos].second);
		pos = this->next[pos];
	}
	return values;
}
//BC = Theta(1)
//WC = Theta(n)
//TC = O(n)


bool SortedMultiMap::remove(TKey c, TValue v) {
	int pos = this->hash(c);
	int prev = -1;
	while (pos != -1)
	{
		if (this->elements[pos].first == c && this->elements[pos].second == v)
		{
			if (prev == -1)
			{
				//if it is the head
				if (this->next[pos] == -1)
				{
					//the only element 
					this->elements[pos] = NULL_TELEM;
					this->next[pos] = -1;
				}
				else 
				{//move the next elem in the current position
					int nextPos = this->next[pos];
					this->elements[pos] = this->elements[nextPos];
					this->next[pos] = this->next[nextPos];
					this->elements[nextPos] = NULL_TELEM;
					this->next[nextPos] = -1;
				}
			}
			else
			{
				//if it's not the head, skip over the current element, and set it to null
				this->next[prev] = this->next[pos];
				this->elements[pos] = NULL_TELEM;
				this->next[pos] = -1;
			}
			if (pos < this->firstEmpty)
			{
				this->firstEmpty = pos;
			}
			this->SMMsize--;
			return true;
		}
		prev = pos;
		pos = this->next[pos];

	}
    return false;
}
//WC = theta(n)
//TC=O(n)
//BC=Theta(n)
//

int SortedMultiMap::size() const {
	return this->SMMsize;
}

bool SortedMultiMap::isEmpty() const {

	return this->SMMsize == 0;
}

SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator(*this);
}

SortedMultiMap::~SortedMultiMap() {
	delete[]this->elements;
	delete[]this->next;
}



void SortedMultiMap::resize() {
	int oldCap = this->capacity;
	this->capacity *= 2;
	TElem* newElements = new TElem[this->capacity];
	int* newNext = new int[this->capacity];


	for (int i = 0; i < this->capacity; i++)
	{
		newElements[i] = NULL_TELEM;
		newNext[i] = -1;
	}

	this->firstEmpty = 0;

	// Rehash elements
	for (int i = 0; i < oldCap; i++)
	{
		if (this->elements[i].first != NULL_TKEY) 
		{
			int newPos = this->hash(this->elements[i].first);
			if (newElements[newPos].first == NULL_TKEY) 
			{
				newElements[newPos] = this->elements[i];
				if (newPos == this->firstEmpty) 
				{
					while (this->firstEmpty < this->capacity && newElements[this->firstEmpty].first != NULL_TKEY)
					{
						this->firstEmpty++;
					}
				}
			}
			else {
				int currentPos = newPos;
				while (newNext[currentPos] != -1)
					currentPos = newNext[currentPos];

				int newFirstEmpty = this->firstEmpty;
				newElements[newFirstEmpty] = this->elements[i];
				newNext[currentPos] = newFirstEmpty;

				while (this->firstEmpty < this->capacity && newElements[this->firstEmpty].first != NULL_TKEY)
				{
					this->firstEmpty++;
				}
			}
		}
	}

	delete[] this->elements;
	delete[] this->next;
	this->elements = newElements;
	this->next = newNext;
}
//BC=WC=TC = theta(new_capacity)

