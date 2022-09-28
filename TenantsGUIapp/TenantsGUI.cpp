#include "TenantsGUI.h"


void TenantsGUI::setupGUI() {
	this->resize(890, 500);

	lyMain = new QHBoxLayout();
	setLayout(lyMain);

	this->setWindowIcon(QIcon("icon.png"));
	this->setWindowTitle("Tenants App");

	widLeft = new QWidget();
	widRight = new QWidget();
	widBtns = new QGroupBox();

	lyLeft = new QVBoxLayout();
	lyRight = new QFormLayout();
	lyBtns = new QFormLayout();

	widLeft->setLayout(lyLeft);
	widRight->setLayout(lyRight);
	widBtns->setLayout(lyBtns);

	btnSortByName = new QPushButton("Sort by Name");
	btnSortBySurface = new QPushButton("Sort by Surface");
	btnSortByType = new QPushButton("Sort by Type");
	modelList = new CustomTableModel{ srv.get_all() };
	widList = new QListView();
	lblCount = new QLabel("x tenants");

	widList->setUniformItemSizes(true);
	widList->setModel(modelList);

	lyLeft->addWidget(lblCount);

	lyLeft->addWidget(widList);
	lyLeft->addWidget(btnSortByName);
	lyLeft->addWidget(btnSortBySurface);
	lyLeft->addWidget(btnSortByType);

	btnReset = new QPushButton("Reset List");
	btnFilterByType = new QPushButton("Filter by Type");
	btnFilterBySurface = new QPushButton("Filter by Surface");
	btnSearch = new QPushButton("Search Apartment");
	
	lyRight->addWidget(btnReset);
	lyRight->addWidget(btnFilterByType);
	lyRight->addWidget(btnFilterBySurface);
	lyRight->addWidget(btnSearch);
	txtMulti = new QLineEdit();
	lyRight->addRow(new QLabel(""), txtMulti);

	QGroupBox* groupNotify = new QGroupBox();
	groupNotify->setTitle("Notification List");

	QFormLayout* lyFormNotify = new QFormLayout();
	groupNotify->setLayout(lyFormNotify);
	lyRight->addWidget(groupNotify);

	btnGenerateNotif = new QPushButton("Randomly add n to Notif. list");
	btnOpenExportGUI = new QPushButton("Export Menu");
	modelTable = new CustomTableModel{ srv.get_notif() };
	btnEmptyNotif = new QPushButton("Empty Notif. list");
	btnAddToNotif = new QPushButton("Add to Notif. list");
	widNotifTable = new QTableView();
	widNotifTable->setModel(modelTable);

	lyFormNotify->addWidget(btnGenerateNotif);
	lyFormNotify->addWidget(btnOpenExportGUI);
	lyFormNotify->addWidget(widNotifTable);
	lyFormNotify->addWidget(btnEmptyNotif);
	lyFormNotify->addWidget(btnAddToNotif);

	lyMain->addWidget(widLeft);
	lyMain->addWidget(widRight);	

	QWidget* widDetails = new QWidget();
	lyForm = new QFormLayout();
	widDetails->setLayout(lyForm);
	
	txtApartment = new QLineEdit();
	txtOwner = new QLineEdit();
	txtType = new QLineEdit();
	txtSurface = new QLineEdit();

	lyForm->addRow(new QLabel("Apartment:"), txtApartment);
	lyForm->addRow(new QLabel("Owner:"), txtOwner);
	lyForm->addRow(new QLabel("Type:"), txtType);
	lyForm->addRow(new QLabel("Surface:"), txtSurface);

	btnAdd = new QPushButton("Add Tenant");
	btnDelete = new QPushButton("Delete Selection");
	btnUpdate = new QPushButton("Update Selection");
	btnUndo = new QPushButton("Undo");
	
	lyForm->addWidget(btnAdd);
	lyForm->addWidget(btnDelete);
	lyForm->addWidget(btnUpdate);
	lyForm->addWidget(btnUndo);

	QGroupBox* groupBasket = new QGroupBox();
	groupBasket->setTitle("Basket");

	QFormLayout* lyForm2 = new QFormLayout();
	groupBasket->setLayout(lyForm2);

	btnOpenCRUD = new QPushButton("Open CRUD");
	btnOpenReadOnly = new QPushButton("Open ReadOnly");
	btnEmptyBasket = new QPushButton("Empty Basket");
	btnAddToBasket = new QPushButton("Add selected to Basket");
	btnGenerateBasket = new QPushButton("Add 2 randomly in Basket");
	
	lyForm2->addWidget(btnOpenCRUD);
	lyForm2->addWidget(btnOpenReadOnly);
	lyForm2->addWidget(btnEmptyBasket);
	lyForm2->addWidget(btnAddToBasket);
	lyForm2->addWidget(btnGenerateBasket);

	lyForm->addWidget(groupBasket);
	lyMain->addWidget(widDetails);

	widBtns->setTitle("Count types");
	lyMain->addWidget(widBtns);
}

void TenantsGUI::refreshBtns() {
	auto lst = srv.get_all();
	
	for (auto& btn : btns) {
		lyBtns->removeWidget(btn);
		delete btn;
	}
	btns.clear();
	lyBtns->update();
	types.clear();
	for (const auto& t : lst) {
		bool ok = false; // assume it isn't
		for (auto& type : types)
			if (t.getType() == type.first) {
				type.second++;
				ok = true;
			}
		if (!ok)
			types.push_back({ t.getType(), 1 });
	}
	for (auto& type : types) {
		QPushButton* button = new QPushButton(QString::fromStdString(type.first));
		QObject::connect(button, &QPushButton::clicked, [&]() {
			QMessageBox::information(nullptr, "Apartamente de tip", QString::number(type.second));
		});
		lyBtns->addWidget(button);
		btns.push_back(button);
	}
}

void TenantsGUI::connectSignalsToSlots() {
	QObject::connect(btnAdd, &QPushButton::clicked, [&]() {
		auto apartment = txtApartment->text().toStdString();
		auto owner = txtOwner->text().toStdString();
		auto type = txtType->text().toStdString();
		auto surface = txtSurface->text().toInt();
		try {
			srv.addTenant(apartment, owner, type, surface);
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
		reloadList();
		refreshBtns();
	});

	QObject::connect(widList->selectionModel(), &QItemSelectionModel::selectionChanged, [&]() {
		auto selected = widList->selectionModel()->selectedIndexes();
		if (selected.isEmpty()) {
			txtApartment->setText("");
			txtOwner->setText("");
			txtType->setText("");
			txtSurface->setText("");
		}
		else {
			auto elem = getSelectedElement();
			txtApartment->setText(QString::fromStdString(elem.getApartment()));
			txtOwner->setText(QString::fromStdString(elem.getOwner()));
			txtType->setText(QString::fromStdString(elem.getType()));
			txtSurface->setText(QString::number(elem.getSurface()));
		}
	});

	QObject::connect(btnDelete, &QPushButton::clicked, [&]() {
		auto selected = widList->selectionModel()->selectedIndexes();
		if (!selected.isEmpty()) {
			auto elem = getSelectedElement();
			srv.removeTenant(elem.getApartment());
		}
		refreshBtns();
		reloadList();
	});

	QObject::connect(btnUpdate, &QPushButton::clicked, [&]() {
		auto selected = widList->selectionModel()->selectedIndexes();
		if (!selected.isEmpty()) {
			auto elem = getSelectedElement();
			auto owner = txtOwner->text().toStdString();
			auto type = txtType->text().toStdString();
			auto surface = txtSurface->text().toInt();
			try {
				srv.updateTenant(elem.getApartment(), owner, type, surface);
			}
			catch (LabOOPException& e) {
				QErrorMessage* message = new QErrorMessage();
				message->showMessage(QString::fromStdString(e.what()));
			}
		}
		refreshBtns();
		reloadList();
	});
	QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
		try {
			srv.undo();
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
		refreshBtns();
		reloadList();
	});
	QObject::connect(btnSortByName, &QPushButton::clicked, [&]() {
		try {
			populateList(srv.sort_types("nume"));
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
	QObject::connect(btnSortBySurface, &QPushButton::clicked, [&]() {
		try {
			populateList(srv.sort_types("suprafata"));
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
	QObject::connect(btnSortByType, &QPushButton::clicked, [&]() {
		try {
			populateList(srv.sort_types("tip"));
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
	QObject::connect(btnReset, &QPushButton::clicked, [&]() {
		try {
			reloadList();
			refreshBtns();
			reloadNotif();
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
	QObject::connect(btnFilterByType, &QPushButton::clicked, [&]() {
		try {
			auto arg = txtMulti->text().toStdString();
			populateList(srv.filter_type(arg));
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
	QObject::connect(btnFilterBySurface, &QPushButton::clicked, [&]() {
		try {
			auto arg = txtMulti->text().toInt();
			populateList(srv.filter_surface(arg));
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
	QObject::connect(btnOpenCRUD, &QPushButton::clicked, [&]() {
		try {
			BasketCRUDGUI* cosGUI = new BasketCRUDGUI{ srv.getBasket() };
			cosGUI->show();
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
	QObject::connect(btnOpenReadOnly, &QPushButton::clicked, [&]() {
		try {
			BasketReadOnlyGUI* cosGUI = new BasketReadOnlyGUI{ srv.getBasket() };
			cosGUI->show();
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
	QObject::connect(btnEmptyBasket, &QPushButton::clicked, [&]() {
		try {
			auto& cos = srv.getBasket();
			cos.clear();
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
	QObject::connect(btnAddToBasket, &QPushButton::clicked, [&]() {
		auto selected = widList->selectionModel()->selectedIndexes();
		if (!selected.isEmpty()) {
			auto elem = getSelectedElement();
			auto& cos = srv.getBasket();
			cos.add(elem);
		}
	});
	QObject::connect(btnGenerateBasket, &QPushButton::clicked, [&]() {
		auto& cos = srv.getBasket();
		cos.generate(2);
	});
	QObject::connect(btnSearch, &QPushButton::clicked, [&]() {
		auto arg = txtMulti->text().toStdString();
		if (srv.existsTenant(arg)) {
			auto ten = srv.getTenant(arg);
			populateList({ ten });
		}
		else populateList({});
	});
	QObject::connect(btnOpenExportGUI, &QPushButton::clicked, [&]() {
		export_gui->show();
	});
	QObject::connect(btnAddToNotif, &QPushButton::clicked, [&]() {
		auto selected = widList->selectionModel()->selectedIndexes();
		if (!selected.isEmpty()) {
			auto elem = getSelectedElement();
			srv.add_to_notif(elem.getId());
		}
		reloadNotif();
	});
	QObject::connect(btnEmptyNotif, &QPushButton::clicked, [&]() {
		try {
			srv.clear_notif();
			reloadNotif();
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
	QObject::connect(btnGenerateNotif, &QPushButton::clicked, [&]() {
		try {
			auto arg = txtMulti->text().toInt();
			srv.gen_notif(arg);
			reloadNotif();
		}
		catch (LabOOPException& e) {
			QErrorMessage* message = new QErrorMessage();
			message->showMessage(QString::fromStdString(e.what()));
		}
	});
}