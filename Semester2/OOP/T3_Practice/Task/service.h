#include "repository.h"
#include <algorithm>

class Service {
private:
    Repository repo = Repository();
public:
    vt get_sort();
    pair<int, vt> get_bypr(int pr);
};