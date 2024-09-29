#pragma once
#include "Observer.h"
#include <vector>


class Subject{
private:
	std::vector<Observer*> observers;
public:
	Subject() {};

	void registerObserver(Observer* observer);
	void unregisterObserver(Observer* observer);
	void notify();

	virtual ~Subject() {};
};