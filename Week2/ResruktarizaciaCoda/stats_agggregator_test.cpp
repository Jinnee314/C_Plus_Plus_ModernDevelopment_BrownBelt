#include "stats_agggregator.h"
#include "MyUnitTestFreimwork.hpp"

#include <sstream>
using namespace std;

string PrintedValue(const StatsAggregator& aggr) {
	ostringstream output;
	aggr.PrintValue(output);
	return output.str();
}

void StatsAggregatorFamili::TestSum() {
	Sum aggr;
	ASSERT_EQUAL(PrintedValue(aggr), "Sum is 0");

	aggr.Process(3);
	aggr.Process(8);
	aggr.Process(-1);
	aggr.Process(16);

	ASSERT_EQUAL(PrintedValue(aggr), "Sum is 26");
}

void StatsAggregatorFamili::TestMin() {
	Min aggr;
	ASSERT_EQUAL(PrintedValue(aggr), "Min is undefined");

	aggr.Process(3);
	aggr.Process(8);
	aggr.Process(-1);
	aggr.Process(16);

	ASSERT_EQUAL(PrintedValue(aggr), "Min is -1");
}

void StatsAggregatorFamili::TestMax() {
	Max aggr;
	ASSERT_EQUAL(PrintedValue(aggr), "Max is undefined");

	aggr.Process(3);
	aggr.Process(8);
	aggr.Process(-1);
	aggr.Process(16);

	ASSERT_EQUAL(PrintedValue(aggr), "Max is 16");
}

void StatsAggregatorFamili::TestAverage() {
	Average aggr;
	ASSERT_EQUAL(PrintedValue(aggr), "Average is undefined");

	aggr.Process(3);
	aggr.Process(8);
	aggr.Process(-1);
	aggr.Process(16);

	ASSERT_EQUAL(PrintedValue(aggr), "Average is 6");
}

void StatsAggregatorFamili::TestMode() {
	Mode aggr;
	ASSERT_EQUAL(PrintedValue(aggr), "Mode is undefined");

	aggr.Process(3);
	aggr.Process(3);
	aggr.Process(8);
	aggr.Process(8);
	aggr.Process(8);
	aggr.Process(8);
	aggr.Process(-1);
	aggr.Process(-1);
	aggr.Process(-1);
	aggr.Process(16);

	ASSERT_EQUAL(PrintedValue(aggr), "Mode is 8");
}

void StatsAggregatorFamili::TestComposite() {
	Composite aggr;
	aggr.Add(make_unique<Sum>());
	aggr.Add(make_unique<Min>());
	aggr.Add(make_unique<Max>());
	aggr.Add(make_unique<Average>());
	aggr.Add(make_unique<Mode>());

	aggr.Process(3);
	aggr.Process(8);
	aggr.Process(-1);
	aggr.Process(16);
	aggr.Process(16);

	string expected = "Sum is 42\n";
	expected += "Min is -1\n";
	expected += "Max is 16\n";
	expected += "Average is 8\n";
	expected += "Mode is 16\n";
	ASSERT_EQUAL(PrintedValue(aggr), expected);
}
