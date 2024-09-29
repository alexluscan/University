#include "ListIterator.h"
#include "SortedIndexedList.h"
using namespace std;
#include <exception>

SortedIndexedList::SortedIndexedList(Relation r) {
    this->rel = r;
    this->root = -1;
    this->numberOfValues = 0;
    this->firstEmpty = 0;
    this->capacity = 8;
    this->tree = new Node[8];
    for (int i = 0; i < capacity - 1; i++)
        this->tree[i].left = i + 1;
    this->tree[capacity - 1].left = -1;
}

int SortedIndexedList::size() const {
    return this->numberOfValues;
}

bool SortedIndexedList::isEmpty() const {
    return this->numberOfValues == 0;
}

TComp SortedIndexedList::getElement(int i) const{
    if (i < 0 || i >= this->numberOfValues)
        throw exception();
    int current = this->root;
    while (this->tree[current].leftElements != i)
    {
        if (i < this->tree[current].leftElements)
            current = this->tree[current].left;
        else
        {
            i -= (this->tree[current].leftElements + 1);
            current = this->tree[current].right;
        }
    }
    return this->tree[current].element;
}

TComp SortedIndexedList::remove(int i) {
    if (i < 0 || i >= this->numberOfValues)
        throw exception();
    int current = this->root, parent = -1;
    while (this->tree[current].leftElements != i)
    {
        parent = current;
        if (i < this->tree[current].leftElements)
        {
            this->tree[current].leftElements--;
            current = this->tree[current].left;
        }
        else
        {
            i -= (this->tree[current].leftElements + 1);
            current = this->tree[current].right;
        }
    }
    TComp removedElement = this->tree[current].element;

    if (this->tree[current].left == -1 && this->tree[current].right == -1)
    {
        this->removeNode(current, parent, -1);
    }
    else if (this->tree[current].right == -1)
    {
        this->removeNode(current, parent, this->tree[current].left);
    }
    else if (this->tree[current].left == -1)
    {
        this->removeNode(current, parent, this->tree[current].right);
    }
    else
    {
        int minPos = this->tree[current].right, minParent = current;
        this->tree[minPos].leftElements--;
        while (this->tree[minPos].left != -1)
        {
            minParent = minPos;
            minPos = this->tree[minPos].left;
            this->tree[minPos].leftElements--;
        }
        this->tree[current].element = this->tree[minPos].element;
        this->removeNode(minPos, minParent, this->tree[minPos].right);
    }
    this->numberOfValues--;
    return removedElement;
}
// O(n)

void SortedIndexedList::removeNode(int nodePos, int parentPos, int nodeChildPos)
{
    if (nodePos == this->root)
        this->root = nodeChildPos;
    else if (nodePos == this->tree[parentPos].left)
        this->tree[parentPos].left = nodeChildPos;
    else
        this->tree[parentPos].right = nodeChildPos;
    this->tree[nodePos].left = this->firstEmpty;
    this->firstEmpty = nodePos;
}
// Theta(1)

int SortedIndexedList::search(TComp e) const {
    if (this->root == -1) return -1;
    int foundPosition = -1, listPosition = 0;
    int current = this->root;
    while (current != -1)
    {
        if (this->tree[current].element == e)
            foundPosition = listPosition + this->tree[current].leftElements;
        if (this->rel(e, tree[current].element))
            current = this->tree[current].left;
        else
        {
            listPosition += this->tree[current].leftElements + 1;
            current = this->tree[current].right;
        }
    }
    return foundPosition;
}
// O(n)

void SortedIndexedList::add(TComp e) {
    if (this->firstEmpty == -1)
        this->resizeTreeArray();

    int current = this->root, parent = -1;
    while (current != -1)
    {
        parent = current;
        if (this->rel(e, tree[current].element))
        {
            this->tree[current].leftElements++;
            current = this->tree[current].left;
        }
        else
            current = this->tree[current].right;
    }

    int newPosition = this->firstEmpty;
    if (current == this->root)
        this->root = newPosition;
    else if (rel(e, tree[parent].element))
        this->tree[parent].left = newPosition;
    else
        this->tree[parent].right = newPosition;

    this->firstEmpty = this->tree[firstEmpty].left;

    this->tree[newPosition].element = e;
    this->tree[newPosition].leftElements = 0;
    this->tree[newPosition].left = -1;
    this->tree[newPosition].right = -1;
    this->numberOfValues++;
}
// O(n)

ListIterator SortedIndexedList::iterator(){
    return ListIterator(*this);
}
// Theta(1)

SortedIndexedList::~SortedIndexedList() {
    delete[] this->tree;
}
// Theta(1)

void SortedIndexedList::resizeTreeArray()
{

    Node* newTree = new Node[capacity * 2];
    for (int i = 0; i < capacity; i++)
        newTree[i] = this->tree[i];

    for (int i = capacity; i < capacity * 2 - 1; i++)
        newTree[i].left = i + 1;
    newTree[capacity * 2 - 1].left = -1;
    delete this->tree;
    this->tree = newTree;
    this->firstEmpty = capacity;
    this->capacity *= 2;
}

void SortedIndexedList::removeBetween(int start, int end) {
    if(start < 0 || end < 0 || start > this->numberOfValues || end > this->numberOfValues)
        throw exception();

    for(int i = start; i < end; i++)
        this->remove(i);
}
// O(n)