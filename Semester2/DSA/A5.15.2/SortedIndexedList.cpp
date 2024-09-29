#include "ListIterator.h"
#include "SortedIndexedList.h"

using namespace std;

#include <exception>


//Theta(1)
SortedIndexedList::SortedIndexedList(Relation r) {
    this->r = r;
    this->root = nullptr;
    this->size_bst = 0;
}


//O(H)
int SortedIndexedList::lastIndexOf(TComp elem) const {
    Node *current = this->root;
    int position = 0;
    int find = -1;
    while (current != nullptr) {
        if (current->info == elem) {
            find = position + current->nr_left;
        }
        if (this->r(elem, current->info)) {
            current = current->left;
        } else {
            position = position + current->nr_left + 1;
            current = current->right;
        }
    }
    return find;
}

//O(H)
void SortedIndexedList::add(TComp e) {
    Node *newNode = new Node;
    newNode->info = e;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->nr_left = 0;

    if (root == nullptr) {
        root = newNode;
    } else {
        Node *current = this->root;
        while (current) {
            if (this->r(e, current->info)) {
                current->nr_left++;
                if (current->left == nullptr) {
                    current->left = newNode;
                    current = nullptr;
                } else {
                    current = current->left;
                }
            } else {
                if (current->right == nullptr) {
                    current->right = newNode;
                    current = nullptr;
                } else {
                    current = current->right;
                }
            }
        }
    }
    this->size_bst++;
}

//O(H)
TComp SortedIndexedList::remove(int pos) {
    if (pos < 0 || pos >= this->size_bst) {
        throw exception();
    }

    Node* prev = nullptr;
    Node* current = this->root;
    int position = 0;
    while (current) {
        if (pos == (position + current->nr_left)) {
            break;
        }
        if (pos < (position + current->nr_left)) {
            prev = current;
            current = current->left;
        } else {
            prev = current;
            position = position + current->nr_left + 1;
            current = current->right;
        }
    }
    int deleted = current->info;
    if (current->left == nullptr && current->right == nullptr) {
        decrease(pos);
        if (prev == nullptr) {
            this->root = nullptr;
        } else {
            if (this->r(current->info, prev->info)) {
                prev->left = nullptr;
            } else {
                prev->right = nullptr;
            }
            delete[] current;
        }
        this->size_bst--;
    } else {
        if (current->left == nullptr || current->right == nullptr) {
            decrease(pos);

            Node* descendant;
            if (current->left == nullptr) {
                descendant = current->right;
            } else {
                descendant = current->left;
            }

            if (prev == nullptr) {
                this->root = descendant;
            } else {
                if (this->r(current->info, prev->info)) {
                    prev->left = descendant;
                } else {
                    prev->right = descendant;
                }
                delete[] current;

            }
            this->size_bst--;
        }
        else {
            TComp n_successor = this->successor(current->info);
            int index = this->search(n_successor);
            this->remove(index);
            current->info = n_successor;
        }
    }
    return deleted;
}

//Theta(1)
int SortedIndexedList::size() const {
    if (root == nullptr) {
        return 0;
    }
    return this->size_bst;
}


//Theta(1)
bool SortedIndexedList::isEmpty() const {
    return (this->size_bst == 0);
}


//O(H)
void SortedIndexedList::decrease(int pos) const
{
    if (pos >= this->size_bst || pos < 0) {
        throw exception();
    }

    Node* current = this->root;
    int position = 0;
    while (current) {
        if (pos == (position + current->nr_left)) {
            return;
        }
        if (pos < (position + current->nr_left)) {
            current->nr_left--;
            current = current->left;
        } else {
            position += current->nr_left + 1;
            current = current->right;
        }
    }

}

//O(H)
TComp SortedIndexedList::get_parent(TComp e)
{
    if (this->root->info == e) {
        return -1;
    }

    Node* current = this->root;
    while (current && current->left->info != e && current->right->info != e) {
        if (!this->r(current->info, e)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return current->info;

}


//O(H)
TComp SortedIndexedList::successor(TComp e)
{
    Node* current = this->root;
    while (current->info!=e) {
        if (this->r(e, current->info)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (current->right) {
        current = current->right;
        while (current->left) {
            current = current->left;
        }
        return current->info;
    } else {
        int p = this->get_parent(current->info);
        Node* parent = this->root;

        while (parent->info != p) {
            if (this->r(p, parent->info)) {
                parent = parent->left;
            } else {
                parent = parent->right;
            }
        }

        while (parent && parent->left != current) {
            current = parent;
            int n_p = this->get_parent(current->info);
            Node* n_parent = this->root;

            while (n_parent->info != n_p) {
                if (this->r(n_p, n_parent->info)) {
                    n_parent = n_parent->left;
                }
                else {
                    n_parent = n_parent->right;
                }
            }

            parent = n_parent;
        }
        return parent->info;
    }
}


//O(H)
int SortedIndexedList::search(TComp e) const {
    Node *current = this->root;
    int position = 0;
    while (current != nullptr) {
        if (current->info == e) {
            return (position + current->nr_left);
        }
        if (this->r(e, current->info)) {
            current = current->left;
        } else {
            position = position + current->nr_left + 1;
            current = current->right;
        }
    }
    return -1;
}

ListIterator SortedIndexedList::iterator() {
    return ListIterator(*this);
}


//Theta(n)
SortedIndexedList::~SortedIndexedList() {
    while (!this->isEmpty()) {
        this->remove(0);
    }
}


//O(H)
TComp SortedIndexedList::getElement(int pos) const {
    if (pos < 0 || pos >= this->size_bst) {
        throw exception();
    }

    Node *current = this->root;
    int position = 0;
    while (current != nullptr) {
        if (pos == (position + current->nr_left)) {
            return current->info;
        }
        if (pos < position + current->nr_left) {
            current = current->left;
        } else {
            position = position + current->nr_left + 1;
            current = current->right;
        }
    }
    return NULL_TCOMP;
}

