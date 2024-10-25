#pragma once
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

// INTERFACE

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec);

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::deque<T>& deq);

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& l);

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::forward_list<T>& fl);

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& s);

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::multiset<T>& s);

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::unordered_set<T>& s);

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::unordered_multiset<T>& s);

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const std::map<K, V>& m);

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const std::multimap<K, V>& m);

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const std::unordered_map<K, V>& m);

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const std::unordered_multimap<K, V>& m);


namespace MyUnitTest
{
	template<typename T, typename U>
	void AssertEqual(const T& t, const U& u, const std::string& hint = {});

	void Assert(bool b, const std::string& hint = {});

	class TestRunner
	{
	private:
		uint32_t failCount = 0;
	public:

		~TestRunner();

		template<class TestFunc>
		void RunTest(TestFunc func, const std::string& test_name);
	};
}

//IMPLEMENTATION

template<typename LineConteiner>
void outLineConteiner(std::ostream& out, const LineConteiner& con)
{
	out << "[";

	bool first = true;
	for (const auto& elem : con)
	{
		if (!first)
		{
			out << ", ";
		}
		first = false;
		out << elem;
	}
	out << "]";
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec)
{
	outLineConteiner(out, vec);

	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::deque<T>& deq)
{
	outLineConteiner(out, deq);

	return out;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& out, const std::list<T>& l)
{
	outLineConteiner(out, l);

	return out;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& out, const std::forward_list<T>& fl)
{
	outLineConteiner(out, fl);

	return out;
}

template<typename SomeSet>
void outSomeSet(std::ostream& out, const SomeSet& s)
{
	out << "{";

	bool first = true;
	for (const auto& elem : s)
	{
		if (!first)
		{
			out << ", ";
		}
		first = false;
		out << elem;
	}
	out << "}";
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& s)
{
	outSomeSet(out, s);

	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::multiset<T>& s)
{
	outSomeSet(out, s);

	return out;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& out, const std::unordered_set<T>& s)
{
	outSomeSet(out, s);

	return out;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& out, const std::unordered_multiset<T>& s)
{
	outSomeSet(out, s);

	return out;
}

template<typename SomeMap>
void outSomeMap(std::ostream& out, const SomeMap& m)
{
	out << "{";

	bool first = true;
	for (const auto& [key, value] : m)
	{
		if (!first)
		{
			out << ", ";
		}
		first = false;
		out << key << ": " << value;
	}
	out << "}";
}

template<typename K, typename V>
std::ostream& operator <<(std::ostream& out, const std::map<K, V>& m)
{
	outSomeMap(out, m);

	return out;
}

template<typename K, typename V>
std::ostream& operator <<(std::ostream& out, const std::multimap<K, V>& m)
{
	outSomeMap(out, m);

	return out;
}

template<typename K, typename V>
inline std::ostream& operator<<(std::ostream& out, const std::unordered_map<K, V>& m)
{
	outSomeMap(out, m);

	return out;
}

template<typename K, typename V>
inline std::ostream& operator<<(std::ostream& out, const std::unordered_multimap<K, V>& m)
{
	outSomeMap(out, m);

	return out;
}


template<typename T, typename U>
void MyUnitTest::AssertEqual(const T& t, const U& u, const std::string& hint)
{
	if (t != u)
	{
		std::ostringstream os;
		os << "Assertion failed:\n" << t << " != " << u << ";\nHint " << hint << "\n";
		throw std::runtime_error(os.str());
	}
}

template<class TestFunc>
void MyUnitTest::TestRunner::RunTest(TestFunc func, const std::string& test_name) {
	try {
		func();
		std::cerr << test_name << " OK" << std::endl;
	}
	catch (std::exception& e) {
		++failCount;
		std::cerr << test_name << " fail: " << e.what() << std::endl;
	}
	catch (...) {
		++failCount;
		std::cerr << "Unknown exception caught" << std::endl;
	}
}

#define ASSERT_EQUAL(x, y)						\
	{											\
		std::ostringstream testOut;					\
		testOut << #x << " != " << #y				\
		<< ", " << __FILE__ << ":"				\
		<< __LINE__;							\
		MyUnitTest::AssertEqual(x,y, testOut.str());	\
	}

#define ASSERT(x)								\
	{											\
		std::ostringstream testOut;					\
		testOut << #x << " is false"				\
		<< ", " << __FILE__ << ":"				\
		<< __LINE__;							\
		MyUnitTest::Assert(x, testOut.str());		\
	}

#define RUN_TEST(tr, func) tr.RunTest(func, #func);