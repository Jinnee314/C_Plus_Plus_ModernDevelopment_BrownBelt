#pragma once
#include <chrono>
#include <string>

namespace sch = std::chrono;

class LogDuration
{
private:
	std::string message;
	sch::steady_clock::time_point start;

public:
	explicit LogDuration(const std::string& mes);

	~LogDuration();
};

#define GET_UNIQ_ID(a) varUniqId##a
#define UNIQ_ID(lineno) GET_UNIQ_ID(lineno)

#define LOG_DURATION(message) \
	LogDuration UNIQ_ID(__LINE__)(message);

struct TotalDuration
{
	std::string message;
	sch::steady_clock::duration value;

	explicit TotalDuration(const std::string& message);
	~TotalDuration();
};


class AddDuration
{
private:
	sch::steady_clock::time_point start;
	sch::steady_clock::duration& add_to;

public:

	explicit AddDuration(sch::steady_clock::duration& dur);
	explicit AddDuration(TotalDuration& data);

	~AddDuration();
};

#define ADD_DURATION(value) \
	AddDuration UNIQ_ID(__LINE__){value}