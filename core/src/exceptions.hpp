#pragma once

#include <exception>
#include <stdexcept>

class bridge_error : public std::runtime_error
{
	bridge_error(const char* str) : std::runtime_error(str) { }

	const char* what() { return what(); }
};

