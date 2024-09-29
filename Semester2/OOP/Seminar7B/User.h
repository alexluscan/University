#pragma once
#include <string>

class User {
private:
	std::string username;
	std::string name;
public:
	User(const std::string username, const std::string name) : username(username), name(name) {};
	std::string getName() { return this->name; };

	~User() {};
};