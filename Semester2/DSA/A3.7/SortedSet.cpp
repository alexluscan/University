#include "SortedSet.h"
#include "SortedSetIterator.h"
#include <iostream>


//Time Complexity
//Best Case: theta(n)
//Worst Case: theta(n)
//Total Complexity: theta(n), n is the capacity of the slla
SortedSet::SortedSet(Relation r) {
	rel = r;
	this->_size=0;
    this->list.capacity = 10;
    this->list.elems = new TComp [this->list.capacity];
    this->list.next = new int [this->list.capacity];
    this->list.head = -1;

    for (int i=0;i<this->list.capacity-1;i++){
        this->list.next[i] = i+1;
    }
    this->list.next[this->list.capacity-1] = -1;
    this->list.firstEmpty = 0;
}

//Time Complexity
//Best Case: theta(1)
//Worst Case: theta(n)
//Total Complexity: O(n)
bool SortedSet::add(TComp elem) {

    TComp prev_elem;
    int position=0;
    int currentNode = this->list.head;

    if (this->search(elem))
        return false;

    while(currentNode!=-1 && !this->rel(elem, this->list.elems[currentNode]))
    {
        position++;
        prev_elem = this->list.elems[currentNode];
        currentNode = this->list.next[currentNode];
    }

    if(this->_size == 0 || position == 0)
        this->insertFirst(elem);
    else {
            this->insertAtPosition(searchPosition(prev_elem), elem);}
    return true;
}

//Time Complexity
//Best Case: theta(1)
//Worst Case: theta(n)
//Total Complexity: O(n)
bool SortedSet::remove(TComp elem) {
	int currentNode = this->list.head;
	int prevNode = -1;

	while (currentNode != -1 && this->list.elems[currentNode]!=elem){
	    prevNode = currentNode;
	    currentNode = this->list.next[currentNode];
	}
	if (currentNode!=-1){
	    if (currentNode == this->list.head)
	        this->list.head = this->list.next[this->list.head];
	    else
	        this->list.next[prevNode] = this->list.next[currentNode];
	    this->list.next[currentNode] = this->list.firstEmpty;
	    this->list.firstEmpty = currentNode;
	    this->_size--;
	    return true;
	}

	return false;
}


//Time Complexity
//Best Case: theta(1)
//Worst Case: theta(n)
//Total Complexity: O(n)
bool SortedSet::search(TComp elem) const {
	int current = this->list.head;

	while (current != -1 && this->list.elems[current] != elem)
	    current = this->list.next[current];
	if (current!=-1)
	    return true;
	else
	    return false;
}


//Time Complexity
//Best Case: theta(1)
//Worst Case: theta(1)
//Total Complexity: theta(1)
int SortedSet::size() const {
	return this->_size;
}

//Time Complexity
//Best Case: theta(1)
//Worst Case: theta(1)
//Total Complexity: theta(1)
bool SortedSet::isEmpty() const {
	if ( this->_size == 0)
	    return true;
	else
	    return false;
}

//Time Complexity
//Best Case: theta(1)
//Worst Case: theta(1)
//Total Complexity: theta(1)
SortedSetIterator SortedSet::iterator() const {
	return SortedSetIterator(*this);
}

//Time Complexity
//Best Case: theta(1)
//Worst Case: theta(n)
//Total Complexity: O(n)
void SortedSet::insertFirst(TComp e) {
    if (this->list.firstEmpty == -1){
        TComp* newElems;
        int* newNext;

        newElems = new TComp[this->list.capacity*2];
        newNext = new int[this->list.capacity*2];

        for(int i=0;i<this->list.capacity;i++){
            newElems[i] = this->list.elems[i];
            newNext[i] = this->list.next[i];
        }
        for (int i=this->list.capacity;i<this->list.capacity*2-1;i++)
            newNext[i] = i+1;
        newNext[this->list.capacity*2-1] = -1;

        delete this->list.elems;
        delete this->list.next;
        this->list.elems = newElems;
        this->list.next = newNext;
        this->list.firstEmpty = this->list.capacity;
        this->list.capacity*=2;
    }
    int newPosition;
    newPosition = this->list.firstEmpty;
    this->list.elems[newPosition] = e;
    this->list.firstEmpty = this->list.next[this->list.firstEmpty];
    this->list.next[newPosition] = this->list.head;
    this->list.head = newPosition;
    this->_size++;
}

//Time Complexity
//Best Case: theta(1)
//Worst Case: theta(n)
//Total Complexity: O(n)
int SortedSet::searchPosition(TComp elem) {
    if(!this->search(elem))
        return -1;
    else{
    int current = this->list.head;
    int pos = 0;

    while (current != -1 && this->list.elems[current] != elem)
    {current = this->list.next[current];
     pos+=1;}

    return current;}
}

//Time Complexity
//Best Case: theta(1)
//Worst Case: theta(n)
//Total Complexity: O(n)
void SortedSet::insertAtPosition(int pos, TComp elem) {
    if (this->list.firstEmpty == -1){
        TComp* newElems;
        int* newNext;

        newElems = new TComp[this->list.capacity*2];
        newNext = new int[this->list.capacity*2];

        for(int i=0;i<this->list.capacity;i++){
            newElems[i] = this->list.elems[i];
            newNext[i] = this->list.next[i];
        }
        for (int i=this->list.capacity;i<this->list.capacity*2-1;i++)
            newNext[i] = i+1;
        newNext[this->list.capacity*2-1] = -1;

        delete this->list.elems;
        delete this->list.next;
        this->list.elems = newElems;
        this->list.next = newNext;
        this->list.firstEmpty = this->list.capacity;
        this->list.capacity*=2;
    }

        int newPosition;
        newPosition = this->list.firstEmpty;
        this->list.elems[newPosition] = elem;
        this->list.firstEmpty = this->list.next[this->list.firstEmpty];
        this->list.next[newPosition] = this->list.next[pos];
        this->list.next[pos] = newPosition;
        this->_size++;

}

/*
 n = size(this)
 m = size(s)
 Time complexity:
 Best Case: theta(n)
 Worst Case: theta(n * m)
 Total Complexity: O(n * m)
 */

void SortedSet::intersection(const SortedSet& s) {
    int currentNode = this->list.head;
    int nextNode = s.list.head;

    while (currentNode!=-1 && nextNode!=-1) {
        int node = currentNode;
        if (currentNode == nextNode)
        {
            currentNode = this->list.next[currentNode];
            nextNode = s.list.next[nextNode];
            this->remove(node);
        }
        else {
            while(!this->rel(nextNode, currentNode) && currentNode != -1)
                currentNode = this->list.next[currentNode];

            while(!s.rel(currentNode, nextNode) && nextNode != -1)
                nextNode = s.list.next[nextNode];
        }
    }

}

SortedSet::~SortedSet() {
    delete[] this->list.elems;
    delete[] this->list.next;
}

