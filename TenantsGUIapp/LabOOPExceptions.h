#pragma once
#include <string>
#include <exception>
#include <map>

class LabOOPException : public std::exception {
	std::string msg;

public:
	LabOOPException(std::string msg) : msg(msg) {}

	std::string what() { return msg; }
};