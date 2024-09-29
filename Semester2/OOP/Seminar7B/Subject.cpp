#include "Subject.h"
#include <algorithm>

void Subject::registerObserver(Observer* observer)
{
	this->observers.push_back(observer);
}

void Subject::unregisterObserver(Observer* observer)
{
	auto it = std::find(observers.begin(), observers.end(), observer);
	this->observers.erase(it);
}

void Subject::notify()
{
	for (auto& o : this->observers)
		o->update();
}
