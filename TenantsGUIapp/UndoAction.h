#pragma once
#include "AbstractRepo.h"

class UndoAction {
public:
	virtual void doUndo() = 0;
	virtual ~UndoAction() {};
};

class UndoAdd : public UndoAction {
	Tenant addedTenant;
	AbstractRepo& repo;

public:
	UndoAdd(AbstractRepo& repo, const Tenant& addedTenant) : repo{ repo }, addedTenant{ addedTenant } {}
	void doUndo() override {
		repo.remove(repo.find(addedTenant.getApartment()));
	}
};

class UndoDelete : public UndoAction {
	Tenant deletedTenant;
	AbstractRepo& repo;

public:
	UndoDelete(AbstractRepo& repo, const Tenant& deletedTenant) : repo{ repo }, deletedTenant{ deletedTenant } {}
	void doUndo() override {
		repo.store(deletedTenant);
	}
};

class UndoUpdate : public UndoAction {
	Tenant oldTenant, updatedTenant;
	AbstractRepo& repo;

public:
	UndoUpdate(AbstractRepo& repo, const Tenant& oldTenant, const Tenant& updatedTenant) : repo{ repo }, oldTenant{ oldTenant }, updatedTenant{ updatedTenant } {}
	void doUndo() override {
		repo.remove(repo.find(updatedTenant.getApartment()));
		repo.store(oldTenant);
	}
};