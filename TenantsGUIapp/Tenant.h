#pragma once
#include <string>
#include <iostream>

using std::ostream;
using std::istream;

class Tenant {

	friend class AbstractRepo;
	friend class InMemoryRepo;
	friend class FileRepo;
	friend class ChanceRepo;

private:
	std::string apartment, owner, type;
	int surface, id = -1;

	// Locatar owner setter
	void setOwner(std::string newOwner) { this->owner = newOwner; };

	// Locatar apartment type setter
	void setType(std::string newType) { this->type = newType; };

	// Locatar surface setter
	void setSurface(int newSurface) noexcept { this->surface = newSurface; };


public:
	// Locatar constructor
	Tenant() noexcept { surface = 0; }
	Tenant(std::string apartment, std::string owner, std::string apartmentType, int surface) : apartment{ apartment }, owner{ owner }, type{ apartmentType }, surface{ surface } {}

	// Locatar apartment getter
	std::string getApartment() const { return this->apartment; }

	// Locatar owner getter
	std::string getOwner() const { return this->owner; }

	// Locatar apartment type getter
	std::string getType() const { return this->type; }

	// Locatar surface getter
	int getSurface() const noexcept { return this->surface; };

	int getId() const noexcept { return this->id; }

	friend ostream& operator<< (ostream& stream, const Tenant& tenant) {
		stream << tenant.id << ";" << tenant.apartment << ";" << tenant.owner << ";" << tenant.type << ";" << tenant.surface;
		return stream;
	}

	friend istream& operator>> (istream& stream, Tenant& tenant) {
		std::string id, surface;
		std::getline(stream, id, ';');
		try {
			tenant.id = stoi(id);
		}
		catch (...) {
			tenant.id = -1;
		}
		std::getline(stream, tenant.apartment, ';');
		std::getline(stream, tenant.owner, ';');
		std::getline(stream, tenant.type, ';');
		stream >> tenant.surface;
		return stream;
	}
};

