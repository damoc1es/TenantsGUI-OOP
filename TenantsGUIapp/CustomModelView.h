#pragma once
#include "Tenant.h"
#include <QAbstractTableModel>
#include <QAbstractListModel>
#include <vector>

class CustomTableModel : public QAbstractTableModel {
	std::vector<Tenant> tenants;

public:
	CustomTableModel(const std::vector<Tenant>& tenants) : tenants{ tenants } {}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return tenants.size();
	}

	int columnCount(const QModelIndex& parent = QModelIndex()) const override {
		return 4;
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
		// to see exactly when any row/col loads
		// showing that it loads only needed data
		qDebug() << "row:" << index.row() << " col:" << index.column() << " role" << role;

		if (role == Qt::DisplayRole) {

			auto p = tenants[index.row()];
			switch (index.column()) {
				case 0: return QString::fromStdString(p.getApartment());
				case 1: return QString::fromStdString(p.getOwner());
				case 2: return QString::fromStdString(p.getType());
				case 3: return QString::number(p.getSurface());
				default: return QString::fromStdString(p.getApartment());
			}
		}

		return QVariant{};
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
		if (role == Qt::DisplayRole) {
			if (orientation == Qt::Horizontal) {
				switch (section) {
					case 0: return QString("Apartment");
					case 1: return QString("Owner");
					case 2: return QString("Type");
					case 3: return QString("Surface");
				}	
			}
		}
		return QVariant();
	}

	void setTenants(std::vector<Tenant> newTenants) {
		this->tenants = newTenants;
		auto topLeft = createIndex(0, 0);
		auto bottomRight = createIndex(rowCount(), columnCount());
		emit dataChanged(topLeft, bottomRight);
		emit layoutChanged();
	}
};