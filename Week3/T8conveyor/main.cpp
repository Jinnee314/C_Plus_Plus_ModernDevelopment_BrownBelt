#include "MyUnitTestFreimwork.hpp"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <functional>

using namespace std;


struct Email {
	string from;
	string to;
	string body;
};

istream& operator>>(istream& in, Email& email)
{
	getline(in, email.from);
	getline(in, email.to);
	getline(in, email.body);

	return in;
}

ostream& operator<<(ostream& out, const Email& email)
{
	out << email.from << "\n" << email.to << "\n" << email.body << "\n";

	return out;
}

class Worker {
private:
	unique_ptr<Worker> next;

public:
	virtual ~Worker() = default;
	virtual void Process(unique_ptr<Email> email) = 0;
	virtual void Run() {
		// только первому worker-у в пайплайне нужно это имплементировать
		throw logic_error("Unimplemented");
	}

protected:
	// реализации должны вызывать PassOn, чтобы передать объект дальше
	// по цепочке обработчиков
	void PassOn(unique_ptr<Email> email) const
	{
		if(next)
		{
			next->Process(move(email));
		}
	}

public:
	void SetNext(unique_ptr<Worker> next)
	{
		this->next = move(next);
	}
};


class Reader : public Worker {
private:
	istream& input;

public:
	// реализуйте класс
	Reader(istream& input) : input(input) {}

	void Run() override
	{
		for (Email em; input >> em;)
		{
			PassOn(make_unique<Email>(em));
		}
	}

	void Process(unique_ptr<Email> email)
	{
	}
};


class Filter : public Worker {
public:
	using Function = function<bool(const Email&)>;
	Function pred;

public:
	// реализуйте класс
	Filter(Function f) : pred(move(f))
	{}

	void Process(unique_ptr<Email> email) override
	{
		if (pred(*email))
		{
			PassOn(move(email));
		}
	}
};


class Copier : public Worker {
private:
	string emailTo;

public:
	// реализуйте класс
	Copier(string emailTo) : emailTo(move(emailTo)) {}
	
	void Process(unique_ptr<Email> email) override
	{
		if (email->to != emailTo)
		{
			auto newEm = make_unique<Email>(*email);
			newEm->to = emailTo;
			PassOn(move(newEm));
		}
		PassOn(move(email));
	}
};


class Sender : public Worker {
	ostream& out;
public:
	// реализуйте класс
	Sender(ostream& out) : out(out) {}

	void Process(unique_ptr<Email> email) override
	{
		out << *email;
		PassOn(move(email));
	}
};


// реализуйте класс
class PipelineBuilder {
private:
	vector<unique_ptr<Worker>> pipeline;

public:
	// добавляет в качестве первого обработчика Reader
	explicit PipelineBuilder(istream& in)
	{
		//pipeline.reserve(10);
		pipeline.push_back(
			make_unique<Reader>(in)
		);
	}

	// добавляет новый обработчик Filter
	PipelineBuilder& FilterBy(Filter::Function filter)
	{
		pipeline.push_back(
			make_unique<Filter>(move(filter))
		);

		return *this;
	}

	// добавляет новый обработчик Copier
	PipelineBuilder& CopyTo(string recipient)
	{
		pipeline.push_back(
			make_unique<Copier>(move(recipient))
		);

		return *this;
	}

	// добавляет новый обработчик Sender
	PipelineBuilder& Send(ostream& out)
	{
		pipeline.push_back(
			make_unique<Sender>(out)
		);

		return *this;
	}

	// возвращает готовую цепочку обработчиков
	unique_ptr<Worker> Build()
	{
		if (pipeline.size() > 1)
		{
			for (int i = static_cast<int>(pipeline.size() - 2); i > -1; --i)
			{
				pipeline[i]->SetNext(move(pipeline[i + 1]));
			}
		}
		
		return move(pipeline[0]);
	}
};


void TestSanity() {
	string input = (
		"erich@example.com\n"
		"richard@example.com\n"
		"Hello there\n"

		"erich@example.com\n"
		"ralph@example.com\n"
		"Are you sure you pressed the right button?\n"

		"ralph@example.com\n"
		"erich@example.com\n"
		"I do not make mistakes of that kind\n"
		);
	istringstream inStream(input);
	ostringstream outStream;

	PipelineBuilder builder(inStream);
	builder.FilterBy([](const Email& email) {
		return email.from == "erich@example.com";
		});
	builder.CopyTo("richard@example.com");
	builder.Send(outStream);
	auto pipeline = builder.Build();

	pipeline->Run();

	string expectedOutput = (
		"erich@example.com\n"
		"richard@example.com\n"
		"Hello there\n"

		"erich@example.com\n"
		"richard@example.com\n"
		"Are you sure you pressed the right button?\n"

		"erich@example.com\n"
		"ralph@example.com\n"
		"Are you sure you pressed the right button?\n"

		);

	ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestSanity);
	return 0;
}
