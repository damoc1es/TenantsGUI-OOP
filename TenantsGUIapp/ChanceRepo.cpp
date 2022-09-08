#include "ChanceRepo.h"

int ChanceRepo::gen_Id() {
	int max = 0;
	for (auto& it : dict)
		if (max < it.first)
			max = it.first;
	return max + 1;
}

void ChanceRepo::feeling_lucky() {
	std::random_device dev;
	std::uniform_int_distribution<int> distribution(1, 100);
	std::mt19937 engine(dev()); // Mersenne twister MT19937

	if (distribution(engine) < chance*100)
		throw LabOOPException("Unlucky.");
}


void ChanceRepo::store(Tenant x) {
	feeling_lucky();
	
	if (x.getId() == -1)
		x.id = gen_Id();
	
	if (find(x.getApartment()) != -1)
		throw LabOOPException{ "Apartment already exists." };

	dict.insert({ x.getId(), x });
}

std::vector<Tenant> ChanceRepo::get_lst() {
	feeling_lucky();

	std::vector <Tenant> List;

	for (auto& it : dict)
		List.push_back(it.second);

	return List;
}

void ChanceRepo::remove(int index) {
	feeling_lucky();
	auto it = dict.find(index);

	if (it != dict.end())
		dict.erase(index);
}

void ChanceRepo::update(int index, Tenant x) {
	feeling_lucky();

	auto it = dict.find(index);

	if (it != dict.end()) {
		dict[index].setOwner(x.getOwner());
		dict[index].setType(x.getType());
		dict[index].setSurface(x.getSurface());
	}
}

void ChanceRepo::clear() {
	feeling_lucky();

	dict.clear();
}

std::vector<Tenant> ChanceRepo::filter(std::function<int(Tenant)> func) {
	feeling_lucky();

	auto List = get_lst();
	std::vector<Tenant> filtered;
	std::copy_if(List.begin(), List.end(), std::back_inserter(filtered), func);
	return filtered;
}

int ChanceRepo::find(std::string findApartment) {
	for (auto& it : dict) {
		if (it.second.getApartment() == findApartment)
			return it.first;
	}
	return -1;
}

std::vector<Tenant> ChanceRepo::sort_fn(std::function<int(Tenant, Tenant)> func) {
	feeling_lucky();

	auto List = get_lst();
	sort(List.begin(), List.end(), func);
	return List;
}

Tenant ChanceRepo::getElement(int index) {
	feeling_lucky();

	return dict[index];
}

size_t ChanceRepo::size() {
	feeling_lucky();

	return dict.size();
}

