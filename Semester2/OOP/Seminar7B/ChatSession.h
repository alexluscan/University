#pragma once
#include <utility>
#include <vector>
#include "User.h"
#include "Message.h"
#include "Subject.h"

typedef std::pair<std::string, std::string> userMessage;

class ChatSession : public Subject{
private:
	std::vector<userMessage> messages;
public:
	ChatSession();

	void addMessage(std::string& user, std::string& message);
	std::vector<userMessage> getMessages();

	~ChatSession() {};
};