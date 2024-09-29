#include "service.h"

vi Service::get_sorted() {
    vi v = this->repository.getItems();
    sort(v.begin(), v.end(), [](Item i1, Item i2) {
        if (i1.getStart() == i2.getStart())
            return i1.getEnd() < i2.getEnd();
        return i1.getStart() < i2.getStart();
    });
    return v;
}

vi Service::get_prob(int prob) {
    vi v = get_sorted();
    vi r;
    for (auto el : v) {
        if (el.getProb() <= prob)
            r.push_back(el);
    }
    return r;
}

pair<vi, int> Service::get_f3(string name, int start) {
    vi v = get_sorted();
    vi r;
    int total = 0;
    int lstart = start;
    for (auto el : v) {
        if (el.getName() == name) {
            if (el.getEnd() > lstart) {
                total += el.getEnd() - lstart;
                lstart = el.getEnd();
                r.push_back(el);
            }
        }
    }
    return {r, total};
}
