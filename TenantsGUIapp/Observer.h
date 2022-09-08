#pragma once
#include <vector>
#include <algorithm>

// Abstract class
// update method need to be implemented by observers
class Observer {
public:
	virtual void update() = 0;
};


// Observable - class that notify changes in it's state
// Derive from this class if you want to provide notifications
class Observable {
private:
	// Non owning pointers to observer instances
	std::vector<Observer*> observers;
public:
	void addObserver(Observer* obs) {
		observers.push_back(obs);
	}
	void removeObserver(Observer* obs) {
		observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
	}
	void notify() {
		for (auto obs : observers) {
			obs->update();
		}
	}
};