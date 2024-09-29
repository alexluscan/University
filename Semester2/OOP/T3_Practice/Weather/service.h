#include "repository.h"
#include <algorithm>

using vi = vector<Item>;

class Service{
private:
    Repository repository;
public:
    vi get_sorted();
    vi get_prob(int prob);
    pair<vi, int> get_f3(string name, int start);
};
