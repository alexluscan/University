#include <iostream>
#include <string>
using namespace std;

class Item
{
private:
    string name;
    int start, end, prob, temp;
public:
    Item(int start, int end, int temp, int prob, string name) : start(start), end(end), temp(temp), prob(prob), name(name){};
    ~Item(){};
    int getStart(){return start;}
    int getEnd(){return end;}
    int getProb(){return prob;}
    string getName(){return name;}
    std::string toString() const {
        return to_string(this->start) + " " + to_string(this->end) + " " + to_string(this->temp) + " " + std::to_string(this->prob) + " " + name;
    }
};
