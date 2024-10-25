#include "Common.h"
#include "MyUnitTestFreimwork.hpp"

#include <sstream>

using namespace std;

enum class Operation
{
	Sum, Prod
};

class ValueExpr : public Expression
{
private:
	int val;

public:
	explicit ValueExpr(int val) : val(val) {}

	int Evaluate() const override
	{
		return val;
	}

	std::string ToString() const override
	{
		return to_string(val);
	}
};

class BinaryOper : public Expression
{
private:
	ExpressionPtr left, right;

public:
	BinaryOper(ExpressionPtr l, ExpressionPtr r) : left(move(l)), right(move(r))
	{}

	int Evaluate() const final
	{
		return EvaluateOnValues(left->Evaluate(), right->Evaluate());
	}

	std::string ToString() const final
	{
		return "(" + left->ToString() + ")" 
			 + getSymbol() 
			 + "(" + right->ToString() + ")";
	}

private:
	virtual char getSymbol() const = 0;
	virtual int EvaluateOnValues(int l, int r) const = 0;
};

class Plus : public BinaryOper
{
public:
	using BinaryOper::BinaryOper;

private:
	char getSymbol() const override
	{
		return '+';
	}

	int EvaluateOnValues(int l, int r) const
	{
		return l + r;
	}
};

class Multiply : public BinaryOper
{
public:
	using BinaryOper::BinaryOper;

private:
	char getSymbol() const override
	{
		return '*';
	}

	int EvaluateOnValues(int l, int r) const
	{
		return l * r;
	}
};


ExpressionPtr Value(int value)
{
	return make_unique<ValueExpr>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right)
{
	return make_unique<Plus>(move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right)
{
	return make_unique<Multiply>(move(left), move(right));
}

string Print(const Expression* e) {
	if (!e) {
		return "Null expression provided";
	}
	stringstream output;
	output << e->ToString() << " = " << e->Evaluate();
	return output.str();
}


void Test() {
	ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
	ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

	ExpressionPtr e2 = Sum(move(e1), Value(5));
	ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

	ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, Test);
	return 0;
}