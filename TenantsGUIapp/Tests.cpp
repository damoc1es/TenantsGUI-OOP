#include "Tests.h"
#include <iostream>

void test_add() {
	InMemoryRepo repo;
	assert(repo.size() == 0);
	repo.clear();
	assert(repo.size() == 0);
	Tenant x{ "asd1", "asd2", "asd3", 53 };
	repo.store(x);
	assert(repo.size() == 1);
	if (repo.get_lst().size() > 0) {
		Tenant y = repo.get_lst().at(0);
		assert(y.getApartment() == x.getApartment() && y.getApartment() == "asd1");
		assert(y.getOwner() == x.getOwner() && y.getOwner() == "asd2");
		assert(y.getType() == x.getType() && y.getType() == "asd3");
		assert(y.getSurface() == x.getSurface() && y.getSurface() == 53);
	}

	Tenant x2{ "asd1", "asd2", "asd3", 53 };
	try {
		repo.store(x2);
		assert(false);
	}
	catch (LabOOPException) {
		assert(true);
	}
	assert(repo.size() == 1);

	std::shared_ptr<InMemoryRepo> repo2{ new InMemoryRepo() };
	Controller srv{ repo2 };
	auto lst = srv.get_all();
	assert(lst.size() == srv.size() && srv.size() == 0);
	srv.addTenant("asd1", "asd2", "asd3", 53);
	assert(srv.size() == 1);
	Tenant y2 = repo.get_lst().at(0);
	assert(y2.getApartment() == "asd1");
	assert(y2.getOwner() == "asd2");
	assert(y2.getType() == "asd3");
	assert(y2.getSurface() == 53);

	try {
		srv.addTenant("asd1", "asd2", "asd3", 53);
		assert(false);
	}
	catch (LabOOPException) {
		assert(true);
	}
	assert(srv.size() == 1);
	srv.addTenant("asd4", "asd2", "asd3", 53);
	assert(srv.size() == 2);
	srv.addTenant("asd5", "asd2", "asd3", 53);
	assert(srv.size() == 3);
	try {
		srv.addTenant("asd4", "asd2", "asd3", 53);
		assert(false);
	}
	catch (LabOOPException& exc) {
		assert(exc.what() != "");
		assert(true);
	}
	assert(srv.size() == 3);
}

void test_delete() {
	InMemoryRepo repo;
	repo.store({ "asd", "xyz", "dshgws", 51 });
	repo.store({ "a", "b", "c", 4 });
	repo.store({ "ast", "byw", "cyw", 51 });
	repo.store({ "a32", "nhdb", "hec", 61 });
	assert(repo.size() == 4);
	repo.remove(4);
	assert(repo.size() == 4);
	auto repo3 = repo;
	assert(repo3.size() == 4);
	assert(repo.find("ast") != -1);
	repo.remove(2);
	assert(repo.find("ast") == -1);
	assert(repo.size() == 3);

	assert(repo.find("asd") != -1);
	repo.remove(0);
	assert(repo.find("asd") == -1);
	assert(repo.size() == 2);
	auto y = Tenant();
	assert(repo.find("a32") != -1);
	repo.remove(1);
	assert(repo.find("a32") == -1);
	assert(repo.size() == 1);

	std::shared_ptr<InMemoryRepo> repo2{ new InMemoryRepo() };
	Controller srv{ repo2 };
	srv.addTenant("asd", "xyz", "dshgws", 51);
	srv.addTenant("a", "b", "c", 4);
	srv.addTenant("ast", "byw", "cyw", 51);
	srv.addTenant("a32", "nhdb", "hec", 61);
	assert(srv.size() == 4);

	try {
		srv.removeTenant("NUEXISTA");
		assert(false);
	} catch(LabOOPException) {
		assert(true);
	}
	assert(srv.size() == 4);

	assert(srv.existsTenant("asd"));
	srv.removeTenant("asd");
	assert(srv.size() == 3);

	try {
		srv.removeTenant("asd");
		assert(false);
	} catch (LabOOPException) {
		assert(true);
	}
	assert(srv.size() == 3);

	assert(srv.existsTenant("a32"));
	srv.removeTenant("a32");
	assert(srv.size() == 2);
}

void test_update() {
	std::shared_ptr<InMemoryRepo> repo2{ new InMemoryRepo() };
	Controller srv{ repo2 };
	srv.addTenant("asd", "xyz", "dshgws", 51);
	srv.addTenant("a", "b", "c", 4);
	srv.addTenant("ast", "byw", "cyw", 51);
	srv.addTenant("a32", "nhdb", "hec", 61);

	try {
		srv.updateTenant("NUEXIST", "a1", "a2", 5);
		assert(false);
	} catch (LabOOPException) {
		assert(true);
	}

	auto x = srv.getTenant("asd");
	assert(x.getOwner() == "xyz");
	assert(x.getType() == "dshgws");
	assert(x.getSurface() == 51);
	srv.updateTenant("asd", "was", "whgfds", 7);
	auto x2 = srv.getTenant("asd");
	assert(x2.getOwner() == "was");
	assert(x2.getType() == "whgfds");
	assert(x2.getSurface() == 7);
	
	auto y = srv.getTenant("a32");
	assert(y.getOwner() == "nhdb");
	assert(y.getType() == "hec");
	assert(y.getSurface() == 61);
	srv.updateTenant("a32", "agsdarwq", "hwrbfdfvas", 134);
	auto y2 = srv.getTenant("a32");
	assert(y2.getOwner() == "agsdarwq");
	assert(y2.getType() == "hwrbfdfvas");
	assert(y2.getSurface() == 134);
}

void test_filter() {
	std::shared_ptr<InMemoryRepo> repo2{ new InMemoryRepo() };
	Controller srv{ repo2 };
	srv.addTenant("Strada 1", "Paul", "garsoniera", 51);
	srv.addTenant("Strada 2", "Alex", "mansarda", 40);
	srv.addTenant("Strada 3", "Diana", "garsoniera", 101);
	srv.addTenant("Strada 4", "Andrei", "garsoniera", 35);
	srv.addTenant("Strada 5", "Constantin", "mansarda", 36);

	auto filtrat_type = srv.filter_type("garsoniera");
	assert(std::find_if(filtrat_type.begin(), filtrat_type.end(), [](auto L) {return L.getApartment() == "Strada 1";}) != filtrat_type.end());
	assert(std::find_if(filtrat_type.begin(), filtrat_type.end(), [](auto L) {return L.getApartment() == "Strada 2";}) == filtrat_type.end());
	assert(std::find_if(filtrat_type.begin(), filtrat_type.end(), [](auto L) {return L.getApartment() == "Strada 3";}) != filtrat_type.end());
	assert(std::find_if(filtrat_type.begin(), filtrat_type.end(), [](auto L) {return L.getApartment() == "Strada 4";}) != filtrat_type.end());
	assert(std::find_if(filtrat_type.begin(), filtrat_type.end(), [](auto L) {return L.getApartment() == "Strada 5";}) == filtrat_type.end());

	auto filtrat_type2 = srv.filter_type("altele");
	assert(filtrat_type2.size() == 0);

	auto filtrat_surface = srv.filter_surface(40);
	assert(std::find_if(filtrat_surface.begin(), filtrat_surface.end(), [](auto L) {return L.getApartment() == "Strada 1";}) != filtrat_surface.end());
	assert(std::find_if(filtrat_surface.begin(), filtrat_surface.end(), [](auto L) {return L.getApartment() == "Strada 2";}) != filtrat_surface.end());
	assert(std::find_if(filtrat_surface.begin(), filtrat_surface.end(), [](auto L) {return L.getApartment() == "Strada 3";}) != filtrat_surface.end());
	assert(std::find_if(filtrat_surface.begin(), filtrat_surface.end(), [](auto L) {return L.getApartment() == "Strada 4";}) == filtrat_surface.end());
	assert(std::find_if(filtrat_surface.begin(), filtrat_surface.end(), [](auto L) {return L.getApartment() == "Strada 5";}) == filtrat_surface.end());

	auto filtrat_surface2 = srv.filter_surface(150);
	assert(filtrat_surface2.size() == 0);
}

void test_sort() {
	std::shared_ptr<InMemoryRepo> repo2{ new InMemoryRepo() };
	Controller srv{ repo2 };
	srv.addTenant("Strada 1", "Paul", "garsoniera", 51);
	srv.addTenant("Strada 2", "Alex", "mansarda", 40);
	srv.addTenant("Strada 3", "Diana", "garsoniera", 101);
	srv.addTenant("Strada 4", "Andrei", "garsoniera", 35);
	srv.addTenant("Strada 5", "Constantin", "mansarda", 36);
	
	auto sorted1 = srv.sort_types("nume");
	assert(sorted1.at(0).getOwner() == "Alex");
	assert(sorted1.at(1).getOwner() == "Andrei");
	assert(sorted1.at(2).getOwner() == "Constantin");
	assert(sorted1.at(3).getOwner() == "Diana");
	assert(sorted1.at(4).getOwner() == "Paul");

	auto sorted2 = srv.sort_types("suprafata");
	assert(sorted2.at(0).getSurface() == 101);
	assert(sorted2.at(1).getSurface() == 51);
	assert(sorted2.at(2).getSurface() == 40);
	assert(sorted2.at(3).getSurface() == 36);
	assert(sorted2.at(4).getSurface() == 35);

	auto sorted3 = srv.sort_types("tip");
	assert(sorted3.at(0).getType() == "garsoniera" && sorted3.at(0).getSurface() == 101);
	assert(sorted3.at(1).getType() == "garsoniera" && sorted3.at(1).getSurface() == 51);
	assert(sorted3.at(2).getType() == "garsoniera" && sorted3.at(2).getSurface() == 35);
	assert(sorted3.at(3).getType() == "mansarda" && sorted3.at(3).getSurface() == 40);
	assert(sorted3.at(4).getType() == "mansarda" && sorted3.at(4).getSurface() == 36);

	auto sorted_wrong = srv.sort_types("asgfadg");
	assert(sorted_wrong.size() == 0);
}

void test_notif() {
	std::shared_ptr<InMemoryRepo> repo2{ new InMemoryRepo() };
	Controller srv{ repo2 };
	srv.addTenant("Strada 1", "Paul", "garsoniera", 51); // 1
	srv.addTenant("Strada 2", "Alex", "mansarda", 40); // 2
	srv.addTenant("Strada 3", "Diana", "garsoniera", 101); // 3
	srv.addTenant("Strada 4", "Andrei", "garsoniera", 35); // 4
	srv.addTenant("Strada 5", "Constantin", "mansarda", 36); // 5
	const auto& lst = srv.get_all();
	assert(srv.notif_size() == 0);
	int k = 0;
	for (const auto& it : lst) {
		srv.add_to_notif(it.getId());
		assert(srv.notif_size() == ++k);
	}
	std::string line;
	srv.export_notif_csv("test.csv");
	std::ifstream testin("test.csv");
	std::getline(testin, line);
	assert(line == "1,Strada 1,Paul,51,garsoniera");
	std::getline(testin, line);
	assert(line == "2,Strada 2,Alex,40,mansarda");
	std::getline(testin, line);
	assert(line == "3,Strada 3,Diana,101,garsoniera");
	std::getline(testin, line);
	assert(line == "4,Strada 4,Andrei,35,garsoniera");
	std::getline(testin, line);
	assert(line == "5,Strada 5,Constantin,36,mansarda");
	std::getline(testin, line);
	assert(line == "");
	testin.close();


	srv.export_notif_html("test.html");
	std::ifstream testin2("test.html");
	std::getline(testin2, line);
	assert(line == "<DOCTYPE html>");
	std::getline(testin2, line);
	assert(line == "<html><head><title>Exported HTML</title></head>");
	std::getline(testin2, line);
	assert(line == "<body>");
	std::getline(testin2, line);
	assert(line == "<li>1, Strada 1, Paul, 51, garsoniera</li>");
	std::getline(testin2, line);
	assert(line == "<li>2, Strada 2, Alex, 40, mansarda</li>");
	std::getline(testin2, line);
	assert(line == "<li>3, Strada 3, Diana, 101, garsoniera</li>");
	std::getline(testin2, line);
	assert(line == "<li>4, Strada 4, Andrei, 35, garsoniera</li>");
	std::getline(testin2, line);
	assert(line == "<li>5, Strada 5, Constantin, 36, mansarda</li>");
	std::getline(testin2, line);
	assert(line == "</body></html>");
	std::getline(testin2, line);
	assert(line == "");
	testin2.close();

	assert(srv.notif_size() == 5 && srv.notif_size() == srv.get_notif().size());
	srv.clear_notif();
	assert(srv.notif_size() == 0);
	try {
		srv.add_to_notif(1351351);
		assert(false);
	}
	catch (LabOOPException) {
		assert(true);
	}
	try {
		srv.gen_notif(0);
		assert(false);
	}
	catch (LabOOPException) {
		assert(true);
	}
	try {
		srv.gen_notif(6);
		assert(false);
	}
	catch (LabOOPException) {
		assert(true);
	}
	srv.gen_notif(5);
}

void test_file() {
	FileRepo repo{ "test_file_repo.TXT" };
	repo.clear();
	repo.store({ "asd", "xyz", "dshgws", 51 });
	try {
		repo.store({ "asd", "xyz", "dshgws", 51 });
		assert(false);
	}
	catch (...) {
		assert(true);
	}
	repo.store({ "a", "b", "c", 4 });
	repo.store({ "ast", "byw", "cyw", 51 });
	repo.store({ "a32", "nhdb", "hec", 61 });
	assert(repo.size() == 4);
	repo.remove(4);
	assert(repo.size() == 4);

	assert(repo.find("ast") != -1);
	repo.remove(2);
	assert(repo.find("ast") == -1);
	assert(repo.size() == 3);


	repo.update(1, { "a", "x", "y", 55 });
	auto List = repo.get_lst();
	assert(List.at(1).getApartment() == "a");
	assert(List.at(1).getOwner() == "x");
	assert(List.at(1).getType() == "y");
	assert(List.at(1).getSurface() == 55);
}

void test_undo() {
	std::shared_ptr<InMemoryRepo> repo2{ new InMemoryRepo() };
	Controller srv{ repo2 };
	srv.addTenant("asd1", "asd2", "asd3", 53);
	assert(srv.size() == 1);
	srv.undo();
	assert(srv.size() == 0);
	try {
		srv.undo();
		assert(false);
	}
	catch (...) {
		assert(true);
	}

	srv.addTenant("loc1", "oldProp", "oldType", 362);
	srv.updateTenant("loc1", "updProp", "updType", 355);
	
	auto x = srv.getTenant("loc1");
	assert(x.getOwner() == "updProp" && x.getType() == "updType" && x.getSurface() == 355);
	srv.undo();
	auto x2 = srv.getTenant("loc1");
	
	assert(x2.getOwner() == "oldProp" && x2.getType() == "oldType" && x2.getSurface() == 362);

	assert(srv.size() == 1);
	srv.removeTenant("loc1");
	assert(srv.size() == 0);
	srv.undo();
	assert(srv.size() == 1);
}

void test_unlucky_repo() {
	ChanceRepo repo{ 2 };
	Tenant x{"asd", "ahfsb", "hfshdag", 136};
	try {
		repo.store(x);
		assert(false);
	} catch (...) {
		assert(true);
	}

	try {
		auto List = repo.get_lst();
		assert(false);
	}
	catch (...) {
		assert(true);
	}

	try {
		repo.remove(1);
		assert(false);
	}
	catch (...) {
		assert(true);
	}

	try {
		repo.update(1, x);
		assert(false);
	}
	catch (...) {
		assert(true);
	}

	try {
		repo.clear();
		assert(false);
	}
	catch (...) {
		assert(true);
	}

	try {
		repo.size();
		assert(false);
	}
	catch (...) {
		assert(true);
	}
}

void test_lucky_repo_store() {
	ChanceRepo repo{ 0 };

	assert(repo.size() == 0);
	repo.clear();
	assert(repo.size() == 0);
	Tenant x{ "asd1", "asd2", "asd3", 53 };
	repo.store(x);
	assert(repo.size() == 1);
	if (repo.get_lst().size() > 0) {
		Tenant y = repo.get_lst().at(0);
		assert(y.getApartment() == x.getApartment() && y.getApartment() == "asd1");
		assert(y.getOwner() == x.getOwner() && y.getOwner() == "asd2");
		assert(y.getType() == x.getType() && y.getType() == "asd3");
		assert(y.getSurface() == x.getSurface() && y.getSurface() == 53);
	}

	Tenant x2{ "asd1", "asd2", "asd3", 53 };
	try {
		repo.store(x2);
		assert(false);
	}
	catch (LabOOPException) {
		assert(true);
	}
	assert(repo.size() == 1);
}

void test_lucky_repo_remove() {
	ChanceRepo repo{ 0 };

	repo.store({ "asd", "xyz", "dshgws", 51 });
	repo.store({ "a", "b", "c", 4 });
	repo.store({ "ast", "byw", "cyw", 51 });
	repo.store({ "a32", "nhdb", "hec", 61 });
	assert(repo.size() == 4);
	repo.remove(5);
	assert(repo.size() == 4);
	auto repo3 = repo;
	assert(repo3.size() == 4);
	assert(repo.find("ast") != -1);
	repo.remove(3);
	assert(repo.find("ast") == -1);
	assert(repo.size() == 3);

	assert(repo.find("asd") != -1);
	repo.remove(1);
	assert(repo.find("asd") == -1);
	assert(repo.size() == 2);
	auto y = Tenant();
	assert(repo.find("a32") != -1);
	repo.remove(repo.find("a32"));
	assert(repo.find("a32") == -1);
	assert(repo.size() == 1);
}

void test_lucky_repo_update() {
	ChanceRepo repo2{ 0 };
	repo2.store({ "asd", "xyz", "dshgws", 51 });
	repo2.store({ "a", "b", "c", 4 });
	repo2.store({ "ast", "byw", "cyw", 51 });
	repo2.store({ "a32", "nhdb", "hec", 61 });

	auto x = repo2.getElement(repo2.find("asd"));
	assert(x.getOwner() == "xyz");
	assert(x.getType() == "dshgws");
	assert(x.getSurface() == 51);
	repo2.update(repo2.find("asd"), { "asd", "was", "whgfds", 7 });
	auto x2 = repo2.getElement(repo2.find("asd"));
	assert(x2.getOwner() == "was");
	assert(x2.getType() == "whgfds");
	assert(x2.getSurface() == 7);

	auto y = repo2.getElement(repo2.find("a32"));
	assert(y.getOwner() == "nhdb");
	assert(y.getType() == "hec");
	assert(y.getSurface() == 61);
	repo2.update(repo2.find("a32"), { "a32", "agsdarwq", "hwrbfdfvas", 134 });
	auto y2 = repo2.getElement(repo2.find("a32"));
	assert(y2.getOwner() == "agsdarwq");
	assert(y2.getType() == "hwrbfdfvas");
	assert(y2.getSurface() == 134);
}

void test_lucky_repo_filter() {
	std::shared_ptr<ChanceRepo> repo2{ new ChanceRepo(0) };
	Controller srv{ repo2 };
	srv.addTenant("Strada 1", "Paul", "garsoniera", 51);
	srv.addTenant("Strada 2", "Alex", "mansarda", 40);
	srv.addTenant("Strada 3", "Diana", "garsoniera", 101);
	srv.addTenant("Strada 4", "Andrei", "garsoniera", 35);
	srv.addTenant("Strada 5", "Constantin", "mansarda", 36);

	auto filtrat_type = srv.filter_type("garsoniera");
	assert(std::find_if(filtrat_type.begin(), filtrat_type.end(), [](auto L) {return L.getApartment() == "Strada 1"; }) != filtrat_type.end());
	assert(std::find_if(filtrat_type.begin(), filtrat_type.end(), [](auto L) {return L.getApartment() == "Strada 2"; }) == filtrat_type.end());
	assert(std::find_if(filtrat_type.begin(), filtrat_type.end(), [](auto L) {return L.getApartment() == "Strada 3"; }) != filtrat_type.end());
	assert(std::find_if(filtrat_type.begin(), filtrat_type.end(), [](auto L) {return L.getApartment() == "Strada 4"; }) != filtrat_type.end());
	assert(std::find_if(filtrat_type.begin(), filtrat_type.end(), [](auto L) {return L.getApartment() == "Strada 5"; }) == filtrat_type.end());

	auto filtrat_type2 = srv.filter_type("altele");
	assert(filtrat_type2.size() == 0);

	auto filtrat_surface = srv.filter_surface(40);
	assert(std::find_if(filtrat_surface.begin(), filtrat_surface.end(), [](auto L) {return L.getApartment() == "Strada 1"; }) != filtrat_surface.end());
	assert(std::find_if(filtrat_surface.begin(), filtrat_surface.end(), [](auto L) {return L.getApartment() == "Strada 2"; }) != filtrat_surface.end());
	assert(std::find_if(filtrat_surface.begin(), filtrat_surface.end(), [](auto L) {return L.getApartment() == "Strada 3"; }) != filtrat_surface.end());
	assert(std::find_if(filtrat_surface.begin(), filtrat_surface.end(), [](auto L) {return L.getApartment() == "Strada 4"; }) == filtrat_surface.end());
	assert(std::find_if(filtrat_surface.begin(), filtrat_surface.end(), [](auto L) {return L.getApartment() == "Strada 5"; }) == filtrat_surface.end());

	auto filtrat_surface2 = srv.filter_surface(150);
	assert(filtrat_surface2.size() == 0);
}

void test_lucky_repo_sort() {
	std::shared_ptr<ChanceRepo> repo2{ new ChanceRepo(0) };
	Controller srv{ repo2 };
	srv.addTenant("Strada 1", "Paul", "garsoniera", 51);
	srv.addTenant("Strada 2", "Alex", "mansarda", 40);
	srv.addTenant("Strada 3", "Diana", "garsoniera", 101);
	srv.addTenant("Strada 4", "Andrei", "garsoniera", 35);
	srv.addTenant("Strada 5", "Constantin", "mansarda", 36);

	auto sorted1 = srv.sort_types("nume");
	assert(sorted1.at(0).getOwner() == "Alex");
	assert(sorted1.at(1).getOwner() == "Andrei");
	assert(sorted1.at(2).getOwner() == "Constantin");
	assert(sorted1.at(3).getOwner() == "Diana");
	assert(sorted1.at(4).getOwner() == "Paul");

	auto sorted2 = srv.sort_types("suprafata");
	assert(sorted2.at(0).getSurface() == 101);
	assert(sorted2.at(1).getSurface() == 51);
	assert(sorted2.at(2).getSurface() == 40);
	assert(sorted2.at(3).getSurface() == 36);
	assert(sorted2.at(4).getSurface() == 35);

	auto sorted3 = srv.sort_types("tip");
	assert(sorted3.at(0).getType() == "garsoniera" && sorted3.at(0).getSurface() == 101);
	assert(sorted3.at(1).getType() == "garsoniera" && sorted3.at(1).getSurface() == 51);
	assert(sorted3.at(2).getType() == "garsoniera" && sorted3.at(2).getSurface() == 35);
	assert(sorted3.at(3).getType() == "mansarda" && sorted3.at(3).getSurface() == 40);
	assert(sorted3.at(4).getType() == "mansarda" && sorted3.at(4).getSurface() == 36);

	auto sorted_wrong = srv.sort_types("asgfadg");
	assert(sorted_wrong.size() == 0);
}

void test_everything() {
	test_add();
	test_delete();
	test_update();
	test_filter();
	test_sort();
	test_notif();
	test_file();
	test_undo();
	test_unlucky_repo();
	test_lucky_repo_store();
	test_lucky_repo_remove();
	test_lucky_repo_update();
	test_lucky_repo_filter();
	test_lucky_repo_sort();
}