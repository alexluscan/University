#include "domain.h"
#include <vector>

using vt = vector<Task>;

class Repository {
private:
    vt tasks;
public:
    Repository(){load_data();}
    ~Repository(){};
    void load_data();
    vt get_all();
};