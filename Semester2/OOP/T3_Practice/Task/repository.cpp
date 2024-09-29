#include "repository.h"

#include <fstream>
#include <sstream>

void Repository::load_data() {
    const std::string filename = "../cmake-build-debug/tasks.txt";
    std::ifstream fin(filename);

    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        std::string desc, duration, priority;
        std::getline(iss, desc, '|');
        std::getline(iss, duration, '|');
        std::getline(iss, priority, '|');
        tasks.push_back(Task(desc, std::stoi(duration), std::stoi(priority)));
    }
}

vt Repository::get_all(){
        return tasks;
}
