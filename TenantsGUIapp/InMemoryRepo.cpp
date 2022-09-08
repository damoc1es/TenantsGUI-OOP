#include "InMemoryRepo.h"

void InMemoryRepo::store(Tenant x) {
	if (find(x.getApartment()) != -1) throw LabOOPException{ "Apartment already exists" };

	if (x.id == -1)
		x.id = gen_Id();

	lst.push_back(x);
}

void InMemoryRepo::remove(int index) {
	if (index < lst.size()) {
		lst.erase(lst.begin() + index);
	}
}

void InMemoryRepo::update(int index, Tenant x) {
	lst.at(index).setOwner(x.getOwner());
	lst.at(index).setType(x.getType());
	lst.at(index).setSurface(x.getSurface());
}

std::vector<Tenant> InMemoryRepo::filter(std::function<int(Tenant)> func) {
	auto List = get_lst();
	std::vector<Tenant> filtered;
	std::copy_if(List.begin(), List.end(), std::back_inserter(filtered), func);
	return filtered;
}

int InMemoryRepo::find(std::string findapartament) {
	auto List = get_lst();
	const auto& it = std::find_if(List.begin(), List.end(), [findapartament](auto L) {return L.getApartment() == findapartament; });
	return it == List.end() ? -1 : int(it - List.begin());
}

std::vector<Tenant> InMemoryRepo::sort_fn(std::function<int(Tenant, Tenant)> func) {
	auto List = get_lst();
	sort(List.begin(), List.end(), func);
	return List;
}