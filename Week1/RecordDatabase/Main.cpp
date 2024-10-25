#include "MyUnitTestFreimwork.hpp"

#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

using namespace std;

struct Record {
	string id;
	string title;
	string user;
	int timestamp;
	int karma;
};


class Database {
private:
	unordered_map<string, Record> kernelDB;
	multimap<int, const Record&> timestampDB;
	multimap<int, const Record&> karmaDB;
	unordered_multimap<string_view, const Record&> usersDB;

	template <typename MultiConteiner, typename Key, typename ID>
	void eraseElemInMultiConteiner(MultiConteiner& mc, Key key, ID id)
	{
		auto [begin, end] = mc.equal_range(key);
		for (begin; begin != end; ++begin)
		{
			if (begin->second.id == id)
			{
				mc.erase(begin);
				break;
			}
		}
	}

	template <typename Iterator, typename Callback>
	void rangeByMultiMap(Iterator begin, Iterator end, Callback callback) const
	{
		for (begin; begin != end; ++begin)
		{
			if (!callback(begin->second))
			{
				break;
			}
		}
	}

public:
	bool Put(Record record)
	{
		auto [iter, isEmpl] = kernelDB.emplace(record.id, move(record));
		if (!isEmpl)
		{
			return false;
		}
		timestampDB.emplace(iter->second.timestamp, iter->second);
		karmaDB.emplace(iter->second.karma, iter->second);
		usersDB.emplace(string_view(iter->second.user), iter->second);

		return true;
	}

	bool Erase(const string& id)
	{
		auto iterKernel = kernelDB.find(id);

		if (iterKernel == kernelDB.end())
		{
			return false;
		}

		eraseElemInMultiConteiner(timestampDB, iterKernel->second.timestamp, iterKernel->second.id);
		eraseElemInMultiConteiner(karmaDB, iterKernel->second.karma, iterKernel->second.id);
		eraseElemInMultiConteiner(usersDB, string_view(iterKernel->second.user), iterKernel->second.id);

		kernelDB.erase(iterKernel);
	}

	const Record* GetById(const string& id) const
	{
		auto res = kernelDB.find(id);

		if (res == end(kernelDB))
		{
			return nullptr;
		}

		return &(res->second);
	}

	template <typename Callback>
	void RangeByTimestamp(int low, int high, Callback callback) const
	{
		auto begin = timestampDB.lower_bound(low);
		auto end = timestampDB.upper_bound(high);

		rangeByMultiMap(begin, end, callback);
	}

	template <typename Callback>
	void RangeByKarma(int low, int high, Callback callback) const
	{
		auto begin = karmaDB.lower_bound(low);
		auto end = karmaDB.upper_bound(high);

		rangeByMultiMap(begin, end, callback);
	}

	template <typename Callback>
	void AllByUser(const string& user, Callback callback) const
	{
		auto [begin, end] = usersDB.equal_range(user);

		rangeByMultiMap(begin, end, callback);
	}
};

void TestRangeBoundaries() {
	const int good_karma = 1000;
	const int bad_karma = -10;

	Database db;
	db.Put({ "id1", "Hello there", "master", 1536107260, good_karma });
	db.Put({ "id2", "O>>-<", "general2", 1536107260, bad_karma });

	int count = 0;
	db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
		++count;
		return true;
		});

	ASSERT_EQUAL(2, count);
}

void TestSameUser() {
	Database db;
	db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
	db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });

	int count = 0;
	db.AllByUser("master", [&count](const Record&) {
		++count;
		return true;
		});

	ASSERT_EQUAL(2, count);
}

void TestReplacement() {
	const string final_body = "Feeling sad";

	Database db;
	db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
	db.Erase("id");
	db.Put({ "id", final_body, "not-master", 1536107260, -10 });

	auto record = db.GetById("id");
	ASSERT(record != nullptr);
	ASSERT_EQUAL(final_body, record->title);
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestRangeBoundaries);
	RUN_TEST(tr, TestSameUser);
	RUN_TEST(tr, TestReplacement);
	return 0;
}
