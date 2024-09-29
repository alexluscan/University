#include <bits/stdc++.h>
using namespace std;

using VS = vector<string>;


bool cmp(string a, string b) {
    return a > b;
}

int main()
{
    VS v{"rain", "in", "spain", "falls", "mainly", "pain"};
    sort(v.begin(), v.end, cmp);
    VS:: iterator it = v.begin() + 4;
    *it = "brain";
    it = v.begin();
    while(it != v.end())
    {
        string aux = *it;
        char c1 = aux[aux.size() - 3];
        char c2 = aux[aux.size() - 2];
        char c3 = aux[aux.size() - 1];
        if (c1 == 'a' && c2 == 'i' && c3 == 'n')
            cout << *it << '\n';
        it++;

    }
}
