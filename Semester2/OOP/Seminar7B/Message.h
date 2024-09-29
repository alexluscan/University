#pragma once
#include <string>

class Message {
private:
	int timestamp;
	std::string message;
public:
	Message(std::string& message): message(message) {};
	Message(int timestamp, const std::string& message) : timestamp(timestamp), message(message) {};

	std::string getMessage() { return this->message; };

	~Message() {};
};