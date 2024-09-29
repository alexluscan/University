#include "service.h"

vt Service::get_sort() {
    vt v = repo.get_all();
    sort(v.begin(), v.end(), [](Task t1, Task t2) {
        if (t1.getPriority() == t2.getPriority())
            return t1.getDuration() < t2.getDuration();
        return t1.getPriority() < t2.getPriority();
    });
    return v;
}

pair<int, vt> Service::get_bypr(int pr) {
    int total = 0;
    vt v = repo.get_all();
    vt vpr;
    for (auto el : v) {
        if (el.getPriority() == pr) {
            total += el.getPriority();
            vpr.push_back(el);
        }
    }
    return {total, vpr};
}
