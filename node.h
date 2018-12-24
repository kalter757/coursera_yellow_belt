#pragma once

#include <iostream>
#include <memory>
#include <stack>
#include <vector>

#include "date.h"

using namespace std;

enum class Comparison
{
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum class LogicalOperation
{
    And,
    Or
};

struct Node
{
    virtual bool Evaluate(const Date& date, const string& event);
};

struct EmptyNode : public Node
{
};

struct DateComparisonNode : public Node
{
    DateComparisonNode(const Comparison& cmp, const Date& date);

    bool Evaluate(const Date& date, const string& event) override;

    const Comparison _cmp;
    const Date _date;
};

struct EventComparisonNode : public Node
{
    EventComparisonNode(const Comparison& cmp, const string& event);

    bool Evaluate(const Date& date, const string& event) override;

    const Comparison _cmp;
    const string _event;
};

struct LogicalOperationNode : public Node
{
    LogicalOperationNode(const LogicalOperation& operation, const shared_ptr<Node> &left, const shared_ptr<Node> &right );

    bool Evaluate(const Date& date, const string& event) override;

    const LogicalOperation _operation;
    shared_ptr<Node> _left;
    shared_ptr<Node> _right;
};
