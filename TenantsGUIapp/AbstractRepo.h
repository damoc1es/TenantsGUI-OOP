#pragma once
#include "LabOOPExceptions.h"
#include "Tenant.h"
#include <algorithm>
#include <vector>
#include <functional>
#include <random>
#include <stdexcept>
#include <memory>


class AbstractRepo {
	friend class Tenant;
public:
	AbstractRepo() = default;

	// Stores given Tenant in repository
	// throws LabOOPException if the apartment already exists
	virtual void store(Tenant x) = 0;

	// Returns a vector with all entities stored
	virtual std::vector <Tenant> get_lst() = 0;

	// Removes the entity from given index
	virtual void remove(int index) = 0;

	// Updates the entity from position with the given one
	virtual void update(int index, Tenant x) = 0;

	// Clears the repository
	virtual void clear() = 0;

	// Filters the repository with given function, returns vector with those respecting function
	virtual std::vector<Tenant> filter(std::function<int(Tenant)> func) = 0;

	// Finds the index of apartment by name
	virtual int find(std::string findApartment) = 0;
	
	// Returns sorted vector of entities by using a custom compare function
	virtual std::vector<Tenant> sort_fn(std::function<int(Tenant, Tenant)> func) = 0;
	
	// Returns the entity from given position
	virtual Tenant getElement(int index) = 0;
	
	// Returns the size of the repository
	virtual size_t size() = 0;
};