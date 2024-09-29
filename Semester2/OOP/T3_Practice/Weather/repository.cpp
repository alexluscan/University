//
// Created by Mircea Maierean on 24.05.2023.
//

#include "repository.h"

void Repository::loadFilesIntoRepo()
{
    std::ifstream file("../wths.txt");
    std::string line;
    const char delimiter = ';';
    std::string start, end, prob, name, temp;
    int st, en, pb, tmp;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::getline(iss, start, delimiter);
        std::getline(iss, end, delimiter);
        std::getline(iss, temp, delimiter);
        std::getline(iss, prob, delimiter);
        std::getline(iss, name, delimiter);
        st = stoi(start);
        en = stoi(end);
        pb = stoi(prob);
        tmp = stoi(temp);
        this->items.push_back(Item(st, en, tmp, pb, name));
    }
}

std::vector<Item> Repository::getItems() {
    return this->items;
}
