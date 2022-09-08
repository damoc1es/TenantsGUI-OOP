#pragma once
#include "AbstractRepo.h"
#include <random>

class ChanceRepo : public AbstractRepo {
private:
	std::map<int, Tenant> dict;
	float chance;

	// Generates currently unused id
	int gen_Id();

	// Are you feeling lucky?
	// Throws exception randomly, based on chance
	void feeling_lucky();

public:
	ChanceRepo(float chance) noexcept : chance{chance} {}

	virtual ~ChanceRepo() {}

	void store(Tenant addedTenant) override;

	std::vector <Tenant> get_lst() override;

	void remove(int index) override;

	void update(int index, Tenant x) override;

	void clear() override;

	std::vector<Tenant> filter(std::function<int(Tenant)> func) override;

	int find(std::string findApartment) override;

	std::vector<Tenant> sort_fn(std::function<int(Tenant, Tenant)> func) override;

	Tenant getElement(int index) override;

	size_t size() override;
};