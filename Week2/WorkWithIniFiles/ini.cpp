#include "ini.h"
using namespace Ini;
using namespace std;

Section& Document::AddSection(std::string name)
{
	auto [iter, flag] = sections.emplace(move(name), Section{});
	return iter->second;
}

const Section& Document::GetSection(const std::string& name) const
{
	return sections.at(name);
}

size_t Document::SectionCount() const
{
	return sections.size();
}

Document Ini::Load(std::istream& input)
{
	Document doc;

	for(string line; getline(input, line);)
	{
		auto& section = doc.AddSection(line.substr(1, line.size() - 2));
		while (getline(input, line) && line != "")
		{
			auto charEq = line.find('=');
			auto val = line.substr(charEq + 1);
			line.resize(charEq);
			section.emplace(move(line), move(val));
		}
	}

	return doc;
}