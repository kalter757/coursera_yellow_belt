#include "node.h"

bool Node::Evaluate(const Date &date, const string &event)
{
    return true;
}


DateComparisonNode::DateComparisonNode(const Comparison &cmp, const Date &date)
    : _cmp(cmp)
    , _date(date){
}

bool DateComparisonNode::Evaluate(const Date &date, const string &event)
{
    switch (_cmp) {
    case Comparison::Less:
    {
        return date < _date;
    }
    case Comparison::LessOrEqual:
    {
        return date <= _date;
    }
    case Comparison::Greater:
    {
        return date > _date;
    }
    case Comparison::GreaterOrEqual:
    {
        return date >= _date;
    }
    case Comparison::Equal:
    {
        return date == _date;
    }
    case Comparison::NotEqual:
    {
        return date != _date;
    }
    }

    return true;
}


EventComparisonNode::EventComparisonNode(const Comparison &cmp, const string &event)
    : _cmp(cmp)
    , _event(event){
}

bool EventComparisonNode::Evaluate(const Date &date, const string &event)
{
    switch (_cmp) {
    case Comparison::Less:
    {
        return event < _event;
    }
    case Comparison::LessOrEqual:
    {
        return event <= _event;
    }
    case Comparison::Greater:
    {
        return event > _event;
    }
    case Comparison::GreaterOrEqual:
    {
        return event >= _event;
    }
    case Comparison::Equal:
    {
        return event == _event;
    }
    case Comparison::NotEqual:
    {
        return event != _event;
    }
    }

    return true;
}


LogicalOperationNode::LogicalOperationNode(const LogicalOperation &operation, const shared_ptr<Node> &left, const shared_ptr<Node> &right)
    : _operation(operation)
    , _left(left)
    , _right(right) {
}

bool LogicalOperationNode::Evaluate(const Date &date, const string &event)
{
    if (_operation == LogicalOperation::And)
    {
        return _left->Evaluate(date, event) && _right->Evaluate(date, event);
    }

    if (_operation == LogicalOperation::Or)
    {
        return _left->Evaluate(date, event) || _right->Evaluate(date, event);
    }

    return false;
}























