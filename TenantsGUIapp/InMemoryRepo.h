#pragma once
#include <iostream>
#include "LabOOPExceptions.h"
#include "AbstractRepo.h"


class InMemoryRepo : public AbstractRepo {
private:
	std::vector <Tenant> lst;

	int gen_Id() {
		int max=0;
		for (auto& it : lst)
			if (max < it.getId())
				max = it.getId();
		return max + 1;
	}

	friend class FileRepo;
public:
	// Repository constructor
	InMemoryRepo() noexcept {};

	// Repository destructor
	virtual ~InMemoryRepo() {}

	// Repository copy constructor
	InMemoryRepo(const InMemoryRepo& old_repo) {
		lst = std::vector<Tenant>(old_repo.lst);
	}

	void store(Tenant addedTenant) override;

	std::vector <Tenant> get_lst() override { return lst; }

	void remove(int index) override;

	void update(int index, Tenant updatedTenant) override;

	void clear() noexcept override { lst.clear(); }

	std::vector<Tenant> filter(std::function<int(Tenant)> func) override;

	int find(std::string findApartment) override;

	std::vector<Tenant> sort_fn(std::function<int(Tenant, Tenant)> func) override;

	Tenant getElement(int index) override { return lst.at(index); }

	size_t size() noexcept override { return lst.size(); }
};

