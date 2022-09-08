#pragma once
#include <stdexcept>
#include <fstream>
#include <random>    // std::default_random_engine
#include <chrono>    // std::chrono::system_clock
#include "AbstractRepo.h"
#include "UndoAction.h"
#include "Observer.h"

class BasketTenants : public Observable {
	std::vector <Tenant> inBasket;
	std::shared_ptr <AbstractRepo> repo;

public:
	BasketTenants(std::shared_ptr<AbstractRepo> repository) : repo{ repository } {}

	bool add(const Tenant& t) {
		const auto& it = std::find_if(inBasket.begin(), inBasket.end(), [t](auto L) noexcept {return L.getId() == t.getId(); });
		if (it != inBasket.end()) return false;
		inBasket.push_back(t);
		notify();
		return true;
	}

	void clear() {
		inBasket.clear();
		notify();
	}

	std::vector<Tenant> list() { return inBasket; }

	void load_list(std::vector<Tenant> lst) {
		inBasket = lst;
		notify();
	}

	void fill(int count) {
		int seed = std::chrono::system_clock::now().time_since_epoch().count();
		auto all = repo->get_lst();
		std::shuffle(all.begin(), all.end(), std::default_random_engine(seed)); // shuffle vector
		while (inBasket.size() < count && all.size() > 0) {
			inBasket.push_back(all.back());
			all.pop_back();
		}

		// notify when something changes in basket
		notify();
	}

	void generate(int nr) {
		auto List = repo->get_lst();
		if (nr < 1) {
			return;
		}
		if (List.size() < nr || nr < 1 || List.size() - inBasket.size() < nr) {
			return;
		}
		std::mt19937 mt{ std::random_device{}() };
		const std::uniform_int_distribution<> dist(0, static_cast<int>(List.size() - 1));

		while (nr--) {
			const int rndNr = dist(mt);
			const auto ok = add(List.at(rndNr));
			if (!ok) nr++;
		}
	}
};

class Controller {
private:
	std::shared_ptr<AbstractRepo> repo;
	std::vector <Tenant> notif;
	std::vector <UndoAction*> undoActions;
	BasketTenants basket;

public:
	// Service constructor
	Controller(std::shared_ptr<AbstractRepo> repository) : repo{ repository }, basket{ repository } {}

	// ~Controller() { for (auto& it : undoActions) if(it) delete it; }
	
	// Adds Tenant with given parameters
	// apartment, owner, apartmentType - string
	// surface - int
	// throws exception if duplicate found
	void addTenant(std::string apartment, std::string owner, std::string apartmentType, int surface);

	// Removes Tenant with given apartment name
	// returns true if it was found and removed
	void removeTenant(std::string apartment);

	// Updates Tenant with given apartment name
	// returns true if it was found and updated
	void updateTenant(std::string apartment, std::string newOwner, std::string newType, int newSurface);

	// Checks if apartment exists
	// returns true if it was found
	bool existsTenant(std::string apartment) const { return repo->find(apartment) != -1; }


	// returns the Tenant with given apartment.
	Tenant getTenant(std::string apartment) { return repo->getElement(repo->find(apartment)); }

	// Filters the repository by surface size
	// returns a repository with apartments of size at least min_surface
	std::vector<Tenant> filter_surface(int min_surface) const;

	// Filters the repository by type
	// returns a repository with apartments of given type
	std::vector<Tenant> filter_type(std::string filter_type) const;

	// returns the number of apartments stored.
	size_t size() { return repo->size(); }

	// sorts the repository by given settings.
	// if sort_type is
	// - "nume", sorts the repo by the owner's names
	// - "suprafata", sorts the repo by apartments surface
	// - "tip", sorts the repo by type of apartment, if equal, compares the surfaces
	std::vector<Tenant> sort_types(std::string sort_type);

	void add_to_notif(int nrAp);
	bool store_notif(const Tenant& addedTenant);

	void clear_notif() noexcept { notif.clear(); }

	void gen_notif(int nr);
	
	size_t notif_size() noexcept { return notif.size(); }

	std::vector <Tenant> get_notif() { return notif; }
	std::vector<Tenant> get_all() const { return repo->get_lst(); }

	void export_notif_csv(std::string filename);

	void export_notif_html(std::string filename);

	void undo();

	BasketTenants& getBasket() { return basket; }
};