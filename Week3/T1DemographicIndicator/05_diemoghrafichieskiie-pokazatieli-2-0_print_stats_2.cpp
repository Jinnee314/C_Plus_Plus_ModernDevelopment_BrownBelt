#include "MyUnitTestFreimwork.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <set>
#include <map>
#include <optional>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
	IteratorRange(Iterator begin, Iterator end)
		: first(begin)
		, last(end)
	{
	}

	Iterator begin() const {
		return first;
	}

	Iterator end() const {
		return last;
	}

private:
	Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
	return IteratorRange{ v.begin(), next(v.begin(), min(top, v.size())) };
}

struct Person {
	string name;
	int age, income;
	bool is_male;
};

struct InformationAboutPeople
{
	vector<Person> peopleSortedByAge;
	vector<int>	cumulativeWealth;
	string maleMostPopularName, womanMostPopularName;
};

string getMostPopularName(
	map<string, int> names
)
{
	if (names.empty())
	{
		return {};
	}

	auto iter = names.begin();

	string_view mostPopularName = iter->first;

	int maxRepetitionName = iter->second;

	for (iter; iter != names.end(); ++iter)
	{
		if (iter->second > maxRepetitionName)
		{
			mostPopularName = iter->first;
			maxRepetitionName = iter->second;
		}
	}

	return string{ mostPopularName };
}

vector<int> getAllSumIncomes(multiset<int> incomes)
{
	if (incomes.empty())
	{
		return {};
	}
	vector<int> res(incomes.size());
	auto iter = incomes.rbegin();
	res[0] = *iter;
	++iter;
	for (size_t i = 1; i < incomes.size(); ++i, ++iter)
	{
		res[i] = res[i - 1] + *iter;
	}

	return res;
}

InformationAboutPeople ReadPeople(istream& input) {
	int count;
	input >> count;

	vector<Person> people(count);
	multiset<int> sortedIncomes;
	map<string, int> maleNames, womanNames;

	for (Person& p : people) {
		char gender;
		input >> p.name >> p.age >> p.income >> gender;
		p.is_male = gender == 'M';

		sortedIncomes.insert(p.income);
		if (p.is_male)
		{
			++maleNames[p.name];
		}
		else
		{
			++womanNames[p.name];
		}
	}

	sort(people.begin(), people.end(), [](const Person& l, const Person& r)
		{
			return l.age < r.age;
		});

	return { move(people),
			 getAllSumIncomes(move(sortedIncomes)),
			 getMostPopularName(move(maleNames)),
			 getMostPopularName(move(womanNames)) };
}

int main()
{
	const InformationAboutPeople peopInf(ReadPeople(cin));

	for (string command; cin >> command; ) {
		if (command == "AGE") {
			int adult_age;
			cin >> adult_age;

			auto adult_begin = lower_bound(begin(peopInf.peopleSortedByAge), end(peopInf.peopleSortedByAge), adult_age,
				[](const Person& l, int age) 
				{
					return l.age < age;
				});

			cout << "There are " << std::distance(adult_begin, end(peopInf.peopleSortedByAge))
				<< " adult people for maturity age " << adult_age << '\n';
		}
		else if (command == "WEALTHY") {
			int count;
			cin >> count;

			cout << "Top-" << count << " people have total income " << peopInf.cumulativeWealth[count - 1] << '\n';
		}
		else if (command == "POPULAR_NAME") {
			char gender;
			cin >> gender;
			
			string name = gender == 'M' ? peopInf.maleMostPopularName : peopInf.womanMostPopularName;
			
			if (name.empty())
			{
				cout << "No people of gender "<< gender << "\n";
			}
			else
			{
				cout << "Most popular name among people of gender "<< gender << " is " << name << "\n";
			}
		}
	}
}
