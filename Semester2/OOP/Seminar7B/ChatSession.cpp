#include "ChatSession.h"

ChatSession::ChatSession() {}

void ChatSession::addMessage(std::string& user, std::string& message)
{
	this->messages.push_back(std::make_pair(message, user));
	notify();
}

std::vector<userMessage> ChatSession::getMessages()
{
	return this->messages;
}
