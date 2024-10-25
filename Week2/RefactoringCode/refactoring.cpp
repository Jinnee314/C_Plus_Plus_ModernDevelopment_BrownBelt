#include "xml.h"
#include "json.h"

#include "MyUnitTestFreimwork.hpp"

#include <vector>
#include <string>
#include <map>

using namespace std;

Json::Document XmlToJson(const Xml::Document& doc) {
	using namespace Json;
	vector<Node> result;

	for (const auto& n : doc.GetRoot().Children()) {
		result.emplace_back(map<string, Node>{
			{"category", Node(n.AttributeValue<string>("category"))},
			{ "amount", Node(n.AttributeValue<int>("amount")) }
		});
	}

	return Json::Document(Node(std::move(result)));
}

Xml::Document JsonToXml(const Json::Document& doc, string root_name) {
	using namespace Xml;

	Node root(move(root_name), {});
	for (const auto& n : doc.GetRoot().AsArray()) {
		root.AddChild(Node("spend", {
		  {"category", n.AsMap().at("category").AsString()},
		  {"amount", to_string(n.AsMap().at("amount").AsInt())},
			}));
	}
	return Document(root);
}

void TestXmlToJson() {

	Xml::Node root("july", {});
	root.AddChild({ "spend", {{"category", "travel"}, {"amount", "23400"}} });
	root.AddChild({ "spend", {{"category", "food"}, {"amount", "5000"}} });
	root.AddChild({ "spend", {{"category", "transport"}, {"amount", "1150"}} });
	root.AddChild({ "spend", {{"category", "sport"}, {"amount", "12000"}} });
	const Xml::Document xml_doc(std::move(root));

	const auto json_doc = XmlToJson(xml_doc);

	const auto& items = json_doc.GetRoot().AsArray();
	ASSERT_EQUAL(items.size(), 4u);

	const vector<string> expected_category = { "travel", "food", "transport", "sport" };
	const vector<int> expected_amount = { 23400, 5000, 1150, 12000 };

	for (size_t i = 0; i < items.size(); ++i) {
		const map<string, Json::Node>& item = items[i].AsMap();
		ASSERT_EQUAL(item.at("category").AsString(), expected_category[i]);
		ASSERT_EQUAL(item.at("amount").AsInt(), expected_amount[i]);
	}
}

void TestJsonToXml() {

	vector<Json::Node> spending;
	{
		using Json::Node;
		spending = {
		  Node(map<string, Node>{
			{"category", Node("food")}, {"amount", Node(2500)}
		  }),
		  Node(map<string, Node>{
			{"category", Node("transport")}, {"amount", Node(1150)}
		  }),
		  Node(map<string, Node>{
			{"category", Node("restaurants")}, {"amount", Node(5780)}
		  }),
		  Node(map<string, Node>{
			{"category", Node("clothes")}, {"amount", Node(7500)}
		  }),
		  Node(map<string, Node>{
			{"category", Node("travel")}, {"amount", Node(23740)}
		  }),
		  Node(map<string, Node>{
			{"category", Node("sport")}, {"amount", Node(12000)}
		  }),
		} ;
	}
	const Json::Document json_doc{ Json::Node(move(spending)) };
	
	const string root_name = "month";
	const auto xml_doc = JsonToXml(json_doc, root_name);
	const Xml::Node& root = xml_doc.GetRoot();

	ASSERT_EQUAL(root.Name(), root_name);
	const vector<Xml::Node>& children = root.Children();
	ASSERT_EQUAL(children.size(), 6u);

	const vector<string> expected_category = {
	  "food", "transport", "restaurants", "clothes", "travel", "sport"
	};
	const vector<int> expected_amount = { 2500, 1150, 5780, 7500, 23740, 12000 };

	for (size_t i = 0; i < children.size(); ++i) {
		const auto& c = children[i];
		ASSERT_EQUAL(c.Name(), "spend");
		ASSERT_EQUAL(c.AttributeValue<string>("category"), expected_category[i]);
		ASSERT_EQUAL(c.AttributeValue<int>("amount"), expected_amount[i]);
	}
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestXmlToJson);
	RUN_TEST(tr, TestJsonToXml);
	return 0;
}
