#include <string>
using namespace std;

class Task {
private:
    string name;
    int duration, priority;
public:
    Task(string name, int duration ,int priority) : name{name}, duration{duration}, priority{priority}{};
    ~Task(){};

    string getDesc() const {return name;}
    int getDuration() const {return duration;}
    int getPriority() const {return priority;}
    string toString() {
        return name + " | " + to_string(duration) + " | " + to_string(priority);
    }
};