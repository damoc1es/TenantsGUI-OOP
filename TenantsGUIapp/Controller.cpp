#include "Controller.h"

void Controller::addTenant(std::string apartament, std::string proprietar, std::string tip_apartament, int suprafata) {
	const Tenant x{ apartament, proprietar, tip_apartament, suprafata };
	if (apartament == "" || proprietar == "")
		throw LabOOPException{ "Apartment and owner can't be null." };
	if (suprafata <= 0)
		throw LabOOPException{ "Surface must be a positive integer." };
	repo->store(x);
	undoActions.push_back(new UndoAdd{ *repo, x });
}

void Controller::removeTenant(std::string apartament) {
	auto const k = repo->find(apartament);
	if (k == -1)
		throw LabOOPException{ "Apartment doesn't exist." };
	
	const auto Id = repo->getElement(k).getId();
	const auto& it = std::find_if(notif.begin(), notif.end(), [Id](auto L) noexcept {return L.getId() == Id; });
	if (it != notif.end()) notif.erase(notif.begin() + (it-notif.begin()));

	undoActions.push_back(new UndoDelete{ *repo, repo->getElement(k) });
	repo->remove(k);
}

void Controller::updateTenant(std::string apartament, std::string newProprietar, std::string newTip, int newSuprafata) {
	auto const k = repo->find(apartament);
	if (k == -1)
		throw LabOOPException{ "Apartment doesn't exist." };
	if (newProprietar == "")
		throw LabOOPException{ "Apartment and owner can't be null." };
	if (newSuprafata <= 0)
		throw LabOOPException{ "Surface must be a positive integer." };

	auto oldX = repo->getElement(k);
	Tenant x{oldX.getApartment(), newProprietar, newTip, newSuprafata};
	repo->update(k, x);

	undoActions.push_back(new UndoUpdate{ *repo, oldX, x });
}

std::vector<Tenant> Controller::filter_surface(int min_surface) const {
	return repo->filter([min_surface](auto L) noexcept {return L.getSurface() >= min_surface; });
}

std::vector<Tenant> Controller::filter_type(std::string filter_type) const {
	return repo->filter([filter_type](auto L) {return L.getType() == filter_type; });
}

std::vector<Tenant> Controller::sort_types(std::string sort_type) {
	if (sort_type == "nume")
		return repo->sort_fn([](Tenant x1, Tenant x2) -> int {return x1.getOwner() < x2.getOwner(); });
	else if (sort_type == "suprafata")
		return repo->sort_fn([](Tenant x1, Tenant x2) noexcept -> int {return x1.getSurface() > x2.getSurface(); });
	else if (sort_type == "tip")
		return repo->sort_fn([](Tenant x1, Tenant x2) -> int {return (x1.getType() < x2.getType()) || (x1.getType() == x2.getType() && x1.getSurface() > x2.getSurface()); });
	
	return {};
}

void Controller::add_to_notif(int nrAp) {
	const auto& lst = repo->get_lst();
	const auto& it = std::find_if(lst.begin(), lst.end(), [nrAp](auto L) noexcept { return L.getId() == nrAp; });

	if(it == lst.end())
		throw LabOOPException{ "Apartment doesn't exist." };
	store_notif(*it);
}

void Controller::export_notif_csv(std::string filename) {
	const auto& lst = get_notif();
	std::ofstream exportout(filename);
	if (!exportout.is_open() || exportout.bad() || exportout.fail()) {
		throw LabOOPException{ "Error opening export file." };
	}

	std::for_each(lst.begin(), lst.end(), [&](auto& it) {
		exportout << it.getId() << "," << it.getApartment() << "," << it.getOwner() << "," << it.getSurface() << "," << it.getType() << '\n';
	});

	exportout.close();
}

void Controller::export_notif_html(std::string filename) {
	const auto& lst = notif;
	std::ofstream exportout(filename);
	if (!exportout.is_open() || exportout.bad() || exportout.fail()) {
		throw LabOOPException{ "Error opening export file." };
	}
	exportout << "<DOCTYPE html>\n<html><head><title>Exported HTML</title></head>\n<body>\n";
	std::for_each(lst.begin(), lst.end(), [&](auto& it) {
		exportout << "<li>" << it.getId() << ", " << it.getApartment() << ", " << it.getOwner() << ", " << it.getSurface() << ", " << it.getType() << "</li>\n";
	});
	
	exportout << "</body></html>\n";
	exportout.close();
}

void Controller::undo() {
	if (undoActions.empty())
		throw LabOOPException("No operation to undo.");

	UndoAction* act = undoActions.back();
	undoActions.pop_back();
	if (act) {
		act->doUndo();
		delete act;
	}
}

void Controller::gen_notif(int nr) {
	auto List = repo->get_lst();
	if (nr < 1) {
		throw LabOOPException{ "Number must be positive." };
	}
	if (List.size() < nr || nr < 1 || List.size() - notif.size() < nr) {
		throw LabOOPException{ "Not enough apartments." };
	}
	std::mt19937 mt{ std::random_device{}() };
	const std::uniform_int_distribution<> dist(0, static_cast<int>(List.size() - 1));

	while (nr--) {
		const int rndNr = dist(mt);
		const auto ok = store_notif(List.at(rndNr));
		if (!ok) nr++;
	}
}

bool Controller::store_notif(const Tenant& x) {
	const auto& it = std::find_if(notif.begin(), notif.end(), [x](auto L) noexcept {return L.getId() == x.getId(); });
	if (it != notif.end()) return false;
	notif.push_back(x);
	return true;
}