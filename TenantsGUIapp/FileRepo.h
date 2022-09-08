#pragma once
#include <fstream>
#include <sstream>
#include "AbstractRepo.h"
#include "InMemoryRepo.h"

class FileRepo : public InMemoryRepo {
private:
	std::string filename;
	
	// Loads entities from data file
	void load_from_file() {
		std::ifstream fin(filename);
		if (!fin.is_open()) throw LabOOPException("Error opening file for reading.");

		Tenant x;
		while (fin >> x) {
			lst.push_back(x);
		}

		fin.close();
	}

	// Overwrites the entities from data file with current ones
	void write_to_file() {
		std::ofstream fout(filename);
		if (!fout.is_open()) throw LabOOPException("Error opening file for writing.");

		for (auto& it : get_lst()) {
			fout << it << '\n';
		}

		fout.close();
	}

public:

	FileRepo(std::string filename) : InMemoryRepo(), filename{ filename } { load_from_file(); }

	void store(Tenant addedTenant) override {
		InMemoryRepo::store(addedTenant);
		write_to_file();
	}

	void remove(int index) override {
		InMemoryRepo::remove(index);
		write_to_file();
	}

	void update(int index, Tenant updatedTenant) override {
		InMemoryRepo::update(index, updatedTenant);
		write_to_file();
	}
};

