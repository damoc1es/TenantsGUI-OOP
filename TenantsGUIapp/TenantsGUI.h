#pragma once
#include <QWidget>
#include <QLabel>
#include <QHboxLayout>
#include <QListWidget>
#include <QTableWidget>
#include "Controller.h"
#include "AbstractRepo.h"
#include <QPushButton>
#include <QFormLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QErrorMessage>
#include <QPainter>
#include <QGroupBox>
#include "CustomModelView.h"

class BasketCRUDGUI : public QWidget, public Observer {
private:
	BasketTenants& basket;

	CustomTableModel* modelTable;
	QTableView* widNotifTable;
	QPushButton* btnEmpty;
	QPushButton* btnRandom;

	void populateTable(const std::vector <Tenant>& lst) {
		modelTable->setTenants(lst);
		widNotifTable->update();
		widNotifTable->viewport()->repaint();
	}

	void setupGUI() {
		QHBoxLayout* ly = new QHBoxLayout;
		modelTable = new CustomTableModel{basket.list()};
		widNotifTable = new QTableView();
		widNotifTable->setModel(modelTable);
		ly->addWidget(widNotifTable);
		btnEmpty = new QPushButton("Clear basket");
		ly->addWidget(btnEmpty);

		btnRandom = new QPushButton("Add 2 randomly");
		ly->addWidget(btnRandom);
		setLayout(ly);
	}

	void connectSignals() {
		basket.addObserver(this);
		QObject::connect(btnEmpty, &QPushButton::clicked, [&]() {
			basket.clear();
			populateTable(basket.list());
			widNotifTable->update();
		});
		QObject::connect(btnRandom, &QPushButton::clicked, [&]() {
			basket.generate(2);
			populateTable(basket.list());
		});
		QObject::connect(modelTable, &QAbstractItemModel::dataChanged, [&]() {
			widNotifTable->viewport()->repaint();
			widNotifTable->repaint();
			widNotifTable->update();
		});
	}

public:

	BasketCRUDGUI(BasketTenants& basket) : basket{ basket } {
		basket.addObserver(this);
		setupGUI();
		connectSignals();
		populateTable(basket.list());
	}

	void update() override {
		populateTable(basket.list());
	}

	~BasketCRUDGUI() {
		basket.removeObserver(this);
	}
};

class BasketReadOnlyGUI : public QWidget, public Observer {
	BasketTenants& basket;
public:
	void paintEvent(QPaintEvent* ev) override {
		QPainter p{ this };

		int w = width() < 25 ? 0 : width();
		int h = height() < 25 ? 0 : height();

		std::mt19937 mt1{ std::random_device{}() };
		const std::uniform_int_distribution<> dist2(0, static_cast<int>(w));
		std::mt19937 mt2{ std::random_device{}() };
		const std::uniform_int_distribution<> dist1(0, static_cast<int>(h));

		auto lst = basket.list();
		for (int i = 0; i < lst.size(); i++) {
			p.fillRect(dist2(mt2), dist1(mt1), 50, 50, Qt::BrushStyle::Dense1Pattern);
		}
	}

	void update() override {
		repaint();
	}

	BasketReadOnlyGUI(BasketTenants& basket) : basket{ basket } {
		resize(300, 300);
		basket.addObserver(this);
		repaint();
	}
	~BasketReadOnlyGUI() {
		basket.removeObserver(this);
	}
};


class TenantsGUI : public QWidget {
private:
	Controller srv;

	QListView* widList;
	CustomTableModel* modelList;

	QWidget* widLeft;
	QWidget* widRight;
	QGroupBox* widBtns;

	QHBoxLayout* lyMain;
	QVBoxLayout* lyLeft;
	QFormLayout* lyRight;
	QFormLayout* lyBtns;

	QPushButton* btnSortByName;
	QPushButton* btnSortBySurface;
	QPushButton* btnSortByType;

	QPushButton* btnReset;
	QPushButton* btnFilterByType;
	QPushButton* btnFilterBySurface;
	QPushButton* btnSearch;
	QLineEdit* txtMulti;

	QPushButton* btnAdd;
	QPushButton* btnUpdate;
	QPushButton* btnDelete;
	QPushButton* btnUndo;
	
	QPushButton* btnOpenCRUD;
	QPushButton* btnOpenReadOnly;
	QPushButton* btnAddToBasket;
	QPushButton* btnEmptyBasket;
	QPushButton* btnGenerateBasket;

	QFormLayout* lyForm;

	QLineEdit* txtApartment;
	QLineEdit* txtOwner;
	QLineEdit* txtType;
	QLineEdit* txtSurface;

	std::vector<QPushButton*> btns;
	std::vector <std::pair<std::string, int>> types;

	QPushButton* btnOpenExportGUI;
	QWidget* export_gui;
	QPushButton* btnAddToNotif;
	QPushButton* btnEmptyNotif;
	QPushButton* btnGenerateNotif;
	
	CustomTableModel* modelTable;
	QTableView* widNotifTable;


	QLabel* lbl = new QLabel("File will be suffixed with corresponding file extension.");
	QHBoxLayout* lyMainEx;
	QPushButton* btnHTML;
	QPushButton* btnCSV;
	QFormLayout* lyFormEx;
	QLineEdit* txtFilename;

	QLabel* lblCount;

	void setupGUI();
	
	void reloadList() {
		populateList(srv.get_all());
	}

	void populateList(std::vector <Tenant> lst) {
		lblCount->setText(QString::number(lst.size()) + QString{ " tenants" });
		modelList->setTenants(lst);
	}

	void reloadNotif() {
		populateNotif(srv.get_notif());
	}

	void populateNotif(std::vector <Tenant> lst) {
		modelTable->setTenants(lst);
	}

	void connectSignalsToSlots();
	Tenant getSelectedElement() {
		auto selected = widList->selectionModel()->selectedIndexes();
		auto selItem = selected.at(0);
		auto apartment = widList->model()->data(selItem, Qt::DisplayRole).toString();
		return srv.getTenant(apartment.toStdString());
	}

	void refreshBtns();
	void setupExport() {
		export_gui = new QWidget();
		export_gui->resize(300, 150);
		lyMainEx = new QHBoxLayout();
		lyFormEx = new QFormLayout();
		export_gui->setLayout(lyFormEx);
		lyFormEx->addWidget(lbl);
		txtFilename = new QLineEdit();
		lyFormEx->addRow(new QLabel("Filename:"), txtFilename);


		btnCSV = new QPushButton("CSV (.csv)");
		lyFormEx->addRow(btnCSV);
		btnHTML = new QPushButton("HTML (.html)");
		lyFormEx->addRow(btnHTML);

		QObject::connect(btnHTML, &QPushButton::clicked, [&]() {
			try {
				auto arg = txtFilename->text().toStdString();
				srv.export_notif_html(arg + ".html");
				QMessageBox::information(nullptr, "Success!", QString::fromStdString("Successfully exported to " + arg + ".html"));
			}
			catch (LabOOPException& e) {
				QErrorMessage* message = new QErrorMessage();
				message->showMessage(QString::fromStdString(e.what()));
			}
		});

		QObject::connect(btnCSV, &QPushButton::clicked, [&]() {
			try {
				auto arg = txtFilename->text().toStdString();
				srv.export_notif_csv(arg + ".csv");
				QMessageBox::information(nullptr, "Success!", QString::fromStdString("Successfully exported to " + arg + ".csv"));
			}
			catch (LabOOPException& e) {
				QErrorMessage* message = new QErrorMessage();
				message->showMessage(QString::fromStdString(e.what()));
			}
		});
	}
public:
	TenantsGUI(Controller& srv) : srv{ srv } {
		setupGUI();
		connectSignalsToSlots();
		reloadList();
		reloadNotif();
		refreshBtns();
		setupExport();
	}
};

