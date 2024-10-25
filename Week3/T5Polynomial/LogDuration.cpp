#include "LogDuration.hpp"

#include <iostream>

using namespace std::chrono;
LogDuration::LogDuration(const std::string& mes) : message(mes + ": "), start(steady_clock::now()) {}

LogDuration::~LogDuration()
{
	auto finish = steady_clock::now();
	auto dur = finish - start;
	std::cerr << message << duration_cast<milliseconds>(dur).count() << " ms" << std::endl;
}

TotalDuration::TotalDuration(const std::string& message) : message(message), value(0) {}

TotalDuration::~TotalDuration()
{
	std::cerr << message << duration_cast<milliseconds>(value).count() << " ms" << std::endl;
}

AddDuration::AddDuration(sch::steady_clock::duration& dur) 
	: add_to(dur)
	, start(steady_clock::now())
{
}

AddDuration::AddDuration(TotalDuration& data) : AddDuration(data.value) {}

AddDuration::~AddDuration()
{
	add_to += steady_clock::now() - start;
}
