#include "database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
#include "test_runner.h"

#include <iostream>
#include <stdexcept>
#include <strstream>

using namespace std;

string ParseEvent(istream& is) {
    // Реализуйте эту функцию
    string event;

    while (is.peek() == ' ')
    {
        is.ignore(1);
    }

    getline(is, event);

    return event;
}

void TestAll();

int main() {
//    TestAll();

    Database db;

    for (string line; getline(cin, line); ) {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "Add")
        {
            const auto date = ParseDate(is);
            const auto event = ParseEvent(is);
            db.Add(date, event);
        }
        else if (command == "Print")
        {
            db.Print(cout);
        }
        else if (command == "Del")
        {
            auto condition = ParseCondition(is);
            auto predicate = [condition](const Date& date, const string& event) {
                return condition->Evaluate(date, event);
            };
            int count = db.RemoveIf(predicate);
            cout << "Removed " << count << " entries" << endl;
        }
        else if (command == "Find")
        {
            auto condition = ParseCondition(is);
            auto predicate = [condition](const Date& date, const string& event) {
                return condition->Evaluate(date, event);
            };

            const auto entries = db.FindIf(predicate);
            for (const auto& entry : entries) {
                cout << entry << endl;
            }
            cout << "Found " << entries.size() << " entries" << endl;
        }
        else if (command == "Last")
        {
            try {
                cout << db.Last(ParseDate(is)) << endl;
            } catch (invalid_argument&) {
                cout << "No entries" << endl;
            }
        }
        else if (command.empty())
        {
            continue;
        }
        else
        {
            throw logic_error("Unknown command: " + command);
        }
    }

    return 0;
}

void TestParseEvent() {
    {
        istringstream is("event");
        AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
    }
    {
        istringstream is("");
        AssertEqual(ParseEvent(is), "", "Parse empty event without leading spaces");
    }
    {
        istringstream is("   sport event ");
        AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
    }
    {
        istringstream is("  first event  \n  second event");
        vector<string> events;
        events.push_back(ParseEvent(is));
        events.push_back(ParseEvent(is));
        AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
    }
}

void TestAddDatabase() {
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        set<string> result = {"event_1", "event_2", "event_3"};

        AssertEqual(db.Find({0,1,2}), result, "Add differennt event");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_1");

        set<string> result = {"event_1", "event_2"};

        AssertEqual(db.Find({0,1,2}), result, "Add not differennt event");
    }
}

void TestParseDate()
{
    {
        istringstream iss("0-10-20");
        Date date(0,10,20);
        AssertEqual(ParseDate(iss), date, "Correct Date_1");
    }
    {
        istringstream iss("1000-12-30");
        Date date(1000,12,30);
        AssertEqual(ParseDate(iss), date, "Correct Date_2");
    }
    {
        istringstream iss("0--12-30");
        try {
            Date date(0,10,20);
            AssertEqual(ParseDate(iss), date, "Incorrect Date_1");
        } catch (...) {
        }
    }
    {
        istringstream iss("0--30");
        try {
            Date date(0,10,20);
            AssertEqual(ParseDate(iss), date, "Incorrect Date_2");
        } catch (...) {
        }
    }
    {
        istringstream iss("0-12-");
        try {
            Date date(0,10,20);
            AssertEqual(ParseDate(iss), date, "Incorrect Date_3");
        } catch (...) {
        }
    }
    {
        istringstream iss("");
        try {
            Date date(0,10,20);
            AssertEqual(ParseDate(iss), date, "Incorrect Date_4");
        } catch (...) {
        }
    }
    {
        istringstream iss("-0-12-30");
        try {
            Date date(0,10,20);
            AssertEqual(ParseDate(iss), date, "Incorrect Date_5");
        } catch (...) {
        }
    }
}

void TestPrintDate()
{
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        ostringstream oss;
        oss << "0000-01-02 event_1\n";
        oss << "0000-01-02 event_2\n";
        oss << "0000-01-02 event_3\n";

        ostringstream oss_result;
        db.Print(oss_result);

        AssertEqual(oss_result.str(), oss.str(), "Print events different");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_1");

        ostringstream oss;
        oss << "0000-01-02 event_1\n";
        oss << "0000-01-02 event_2\n";

        ostringstream oss_result;
        db.Print(oss_result);

        AssertEqual(oss_result.str(), oss.str(), "Print events not different");
    }
}

void TestLastDate()
{
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        string result = "0000-01-02 event_3";

        AssertEqual(db.Last({0,1,2}), result, "Last date_1");
    }
    {
        Database db;

        db.Add({0,1,10}, "event_4");
        db.Add({0,1,10}, "event_5");
        db.Add({0,1,10}, "event_6");

        string result = "0000-01-10 event_6";

        AssertEqual(db.Last({0,1,10}), result, "Last date_2");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        string result = "0000-01-02 event_3";

        try {
            AssertEqual(db.Last({0,1,1}), result, "Last date_3");
        } catch (...) {
        }
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,1,10}, "event_4");
        db.Add({0,1,10}, "event_5");
        db.Add({0,1,10}, "event_6");

        string result = "0000-01-10 event_6";

        AssertEqual(db.Last({0,2,1}), result, "Last date_4");
    }
}

void TestFindIfDate()
{
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        vector<string> result = {"0000-01-02 event_1", "0000-01-02 event_2", "0000-01-02 event_3"};

        auto condition = [](const Date& date, const string& event) {
            return date == Date(0,1,2);
        };

        AssertEqual(db.FindIf(condition), result, "FindIf date_1");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,2,1}, "event_4");
        db.Add({0,2,1}, "event_5");
        db.Add({0,2,1}, "event_6");

        vector<string> result = {"0000-02-01 event_4", "0000-02-01 event_5", "0000-02-01 event_6"};

        auto condition = [](const Date& date, const string& event) {
            return date == Date(0,2,1);
        };

        AssertEqual(db.FindIf(condition), result, "FindIf date_2");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,2,1}, "event_4");
        db.Add({0,2,1}, "event_5");
        db.Add({0,2,1}, "event_6");

        vector<string> result = {"0000-02-01 event_5"};

        auto condition = [](const Date& date, const string& event) {
            return (date == Date(0,2,1)) && (event == "event_5");
        };

        AssertEqual(db.FindIf(condition), result, "FindIf date_3");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,2,1}, "event_4");
        db.Add({0,2,1}, "event_5");
        db.Add({0,2,1}, "event_6");

        vector<string> result;

        auto condition = [](const Date& date, const string& event) {
            return (date == Date(0,2,1)) && (event == "event_10");
        };

        AssertEqual(db.FindIf(condition), result, "FindIf date_4");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,2,1}, "event_4");
        db.Add({0,2,1}, "event_5");
        db.Add({0,2,1}, "event_6");

        vector<string> result = {"0000-01-02 event_1",
                                 "0000-01-02 event_2",
                                 "0000-01-02 event_3",
                                 "0000-02-01 event_4",
                                 "0000-02-01 event_5",
                                 "0000-02-01 event_6"};

        auto condition = [](const Date& date, const string& event) {
            return (date > Date(0,1,1)) && (date < Date(0,2,2));
        };

        AssertEqual(db.FindIf(condition), result, "FindIf date_5");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,2,1}, "event_4");
        db.Add({0,2,1}, "event_5");

        db.Add({0,2,2}, "event_6");

        vector<string> result = {"0000-01-02 event_1",
                                 "0000-01-02 event_2",
                                 "0000-01-02 event_3",
                                 "0000-02-01 event_4",
                                 "0000-02-01 event_5"};

        auto condition = [](const Date& date, const string& event) {
            return (date > Date(0,1,1)) && (date < Date(0,2,2));
        };

        AssertEqual(db.FindIf(condition), result, "FindIf date_5");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,2,1}, "event_4");
        db.Add({0,2,1}, "event_5");

        db.Add({0,2,2}, "event_6");

        vector<string> result = {"0000-01-02 event_1",
                                 "0000-01-02 event_2",
                                 "0000-01-02 event_3",
                                 "0000-02-01 event_4",
                                 "0000-02-01 event_5",
                                 "0000-02-02 event_6"};

        auto condition = [](const Date& date, const string& event) {
            return (date >= Date(0,1,1)) && (date <= Date(0,2,2));
        };

        AssertEqual(db.FindIf(condition), result, "FindIf date_6");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,2,1}, "event_4");
        db.Add({0,2,1}, "event_5");

        db.Add({0,2,2}, "event_6");

        vector<string> result = {"0000-01-02 event_1",
                                 "0000-01-02 event_3",
                                 "0000-02-02 event_6"};

        auto condition = [](const Date& date, const string& event) {
            return (date != Date(0,2,1)) && (event != "event_2");
        };

        AssertEqual(db.FindIf(condition), result, "FindIf date_6");
    }
}

void TestRemoveIfDate()
{
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        auto condition = [](const Date& date, const string& event) {
            return date == Date(0,1,2);
        };

        AssertEqual(db.RemoveIf(condition), 3, "RemoveIf date_1");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,2,1}, "event_4");
        db.Add({0,2,1}, "event_5");
        db.Add({0,2,1}, "event_6");

        auto condition = [](const Date& date, const string& event) {
            return date == Date(0,2,1);
        };

        AssertEqual(db.RemoveIf(condition), 3, "FindIf date_2");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,2,1}, "event_4");
        db.Add({0,2,1}, "event_5");
        db.Add({0,2,1}, "event_6");

        auto condition = [](const Date& date, const string& event) {
            return event == "event_3";
        };

        AssertEqual(db.RemoveIf(condition), 1, "FindIf date_3");
    }
    {
        Database db;

        db.Add({0,1,2}, "event_1");
        db.Add({0,1,2}, "event_2");
        db.Add({0,1,2}, "event_3");

        db.Add({0,2,1}, "event_4");
        db.Add({0,2,1}, "event_5");
        db.Add({0,2,1}, "event_6");

        auto condition = [](const Date& date, const string& event) {
            return (event == "event_3") || (event == "event_6") ;
        };

        AssertEqual(db.RemoveIf(condition), 2, "FindIf date_4");
    }
}

void TestDateComparisonNode()
{
    {
        DateComparisonNode dateNode(Comparison::Less, {2017,6,15});
        Assert(dateNode.Evaluate({2017,6,14}, "event"), "DateComparisonNode Less OK");
    }
    {
        DateComparisonNode dateNode(Comparison::Less, {2017,6,15});
        Assert(!dateNode.Evaluate({2017,6,15}, "event"), "DateComparisonNode Less NOT OK");
    }

    {
        DateComparisonNode dateNode(Comparison::LessOrEqual, {2017,6,15});
        Assert(dateNode.Evaluate({2017,6,15}, "event"), "DateComparisonNode LessOrEqual OK");
    }
    {
        DateComparisonNode dateNode(Comparison::LessOrEqual, {2017,6,15});
        Assert(!dateNode.Evaluate({2017,6,16}, "event"), "DateComparisonNode LessOrEqual NOT OK");
    }

    {
        DateComparisonNode dateNode(Comparison::Greater, {2017,6,15});
        Assert(dateNode.Evaluate({2017,6,16}, "event"), "DateComparisonNode Greater OK");
    }
    {
        DateComparisonNode dateNode(Comparison::Greater, {2017,6,15});
        Assert(!dateNode.Evaluate({2017,6,15}, "event"), "DateComparisonNode Greater NOT OK");
    }

    {
        DateComparisonNode dateNode(Comparison::GreaterOrEqual, {2017,6,15});
        Assert(dateNode.Evaluate({2017,6,15}, "event"), "DateComparisonNode GreaterOrEqual OK");
    }
    {
        DateComparisonNode dateNode(Comparison::GreaterOrEqual, {2017,6,15});
        Assert(!dateNode.Evaluate({2017,6,14}, "event"), "DateComparisonNode GreaterOrEqual NOT OK");
    }

    {
        DateComparisonNode dateNode(Comparison::Equal, {2017,6,15});
        Assert(dateNode.Evaluate({2017,6,15}, "event"), "DateComparisonNode Equal OK");
    }
    {
        DateComparisonNode dateNode(Comparison::Equal, {2017,6,15});
        Assert(!dateNode.Evaluate({2017,5,15}, "event"), "DateComparisonNode Equal NOT OK");
    }

    {
        DateComparisonNode dateNode(Comparison::NotEqual, {2017,6,15});
        Assert(dateNode.Evaluate({2017,6,14}, "event"), "DateComparisonNode NotEqual OK");
    }
    {
        DateComparisonNode dateNode(Comparison::NotEqual, {2017,6,15});
        Assert(!dateNode.Evaluate({2017,6,15}, "event"), "DateComparisonNode NotEqual NOT OK");
    }
}

void TestEventComparisonNode()
{
    {
        EventComparisonNode eventNode(Comparison::Less, "event_1");
        Assert(eventNode.Evaluate({0,1,1}, "event_"), "EventComparisonNode Less OK");
    }
    {
        EventComparisonNode eventNode(Comparison::Less, "event_1");
        Assert(!eventNode.Evaluate({0,1,1}, "event_2"), "EventComparisonNode Less NOT OK");
    }

    {
        EventComparisonNode eventNode(Comparison::LessOrEqual, "event_1");
        Assert(eventNode.Evaluate({0,1,1}, "event_1"), "EventComparisonNode LessOrEqual OK");
    }
    {
        EventComparisonNode eventNode(Comparison::LessOrEqual, "event_1");
        Assert(!eventNode.Evaluate({0,1,1}, "event_2"), "EventComparisonNode LessOrEqual NOT OK");
    }

    {
        EventComparisonNode eventNode(Comparison::Greater, "event_1");
        Assert(eventNode.Evaluate({0,1,1}, "event_2"), "EventComparisonNode Greater OK");
    }
    {
        EventComparisonNode eventNode(Comparison::Greater, "event_1");
        Assert(!eventNode.Evaluate({0,1,1}, "event_1"), "EventComparisonNode Greater NOT OK");
    }

    {
        EventComparisonNode eventNode(Comparison::GreaterOrEqual, "event_1");
        Assert(eventNode.Evaluate({0,1,1}, "event_1"), "EventComparisonNode GreaterOrEqual OK");
    }
    {
        EventComparisonNode eventNode(Comparison::GreaterOrEqual, "event_1");
        Assert(!eventNode.Evaluate({0,1,1}, "event"), "EventComparisonNode GreaterOrEqual NOT OK");
    }

    {
        EventComparisonNode eventNode(Comparison::Equal, "event_1");
        Assert(eventNode.Evaluate({0,1,1}, "event_1"), "EventComparisonNode Equal OK");
    }
    {
        EventComparisonNode eventNode(Comparison::Equal, "event_1");
        Assert(!eventNode.Evaluate({0,1,1}, "event_2"), "EventComparisonNode Equal NOT OK");
    }

    {
        EventComparisonNode eventNode(Comparison::NotEqual, "event_1");
        Assert(eventNode.Evaluate({0,1,1}, "event"), "EventComparisonNode NotEqual OK");
    }
    {
        EventComparisonNode eventNode(Comparison::NotEqual, "event_1");
        Assert(!eventNode.Evaluate({0,1,1}, "event_1"), "EventComparisonNode NotEqual NOT OK");
    }
}

void TestLogicalOperationNode()
{
    {
        shared_ptr<Node> leftPtr  = make_shared<EventComparisonNode>(Comparison::Less, "event_1");
        shared_ptr<Node> rightPtr = make_shared<EventComparisonNode>(Comparison::NotEqual, "event");

        LogicalOperationNode logicalNode(LogicalOperation::And, leftPtr, rightPtr);
        Assert(logicalNode.Evaluate({0,1,1}, "event_"), "LogicalOperationNode And OK");
    }
    {
        shared_ptr<Node> leftPtr  = make_shared<EventComparisonNode>(Comparison::Less, "event_1");
        shared_ptr<Node> rightPtr = make_shared<EventComparisonNode>(Comparison::NotEqual, "event");

        LogicalOperationNode logicalNode(LogicalOperation::And, leftPtr, rightPtr);
        Assert(!logicalNode.Evaluate({0,1,1}, "event"), "LogicalOperationNode And NOT OK");
    }

    {
        shared_ptr<Node> leftPtr  = make_shared<EventComparisonNode>(Comparison::GreaterOrEqual, "event_1");
        shared_ptr<Node> rightPtr = make_shared<EventComparisonNode>(Comparison::NotEqual, "event");

        LogicalOperationNode logicalNode(LogicalOperation::Or, leftPtr, rightPtr);
        Assert(logicalNode.Evaluate({0,1,1}, "event_2"), "LogicalOperationNode Or OK");
    }
    {
        shared_ptr<Node> leftPtr  = make_shared<EventComparisonNode>(Comparison::GreaterOrEqual, "event_1");
        shared_ptr<Node> rightPtr = make_shared<EventComparisonNode>(Comparison::NotEqual, "event");

        LogicalOperationNode logicalNode(LogicalOperation::Or, leftPtr, rightPtr);
        Assert(logicalNode.Evaluate({0,1,1}, "even"), "LogicalOperationNode Or OK 2");
    }
    {
        shared_ptr<Node> leftPtr  = make_shared<EventComparisonNode>(Comparison::GreaterOrEqual, "event_1");
        shared_ptr<Node> rightPtr = make_shared<EventComparisonNode>(Comparison::Equal, "event");

        LogicalOperationNode logicalNode(LogicalOperation::Or, leftPtr, rightPtr);
        Assert(!logicalNode.Evaluate({0,1,1}, "even"), "LogicalOperationNode Or NOT OK");
    }
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(TestAddDatabase, "TestAddDatabase");
    tr.RunTest(TestParseDate, "TestParseDate");
    tr.RunTest(TestPrintDate, "TestPrintDate");
    tr.RunTest(TestLastDate, "TestLastDate");
    tr.RunTest(TestParseEvent, "TestParseEvent");
    tr.RunTest(TestFindIfDate, "TestFindIfDate");
    tr.RunTest(TestRemoveIfDate, "TestRemoveIfDate");
    tr.RunTest(TestDateComparisonNode, "TestDateComparisonNode");
    tr.RunTest(TestEventComparisonNode, "TestEventComparisonNode");
    tr.RunTest(TestLogicalOperationNode, "TestLogicalOperationNode");
    tr.RunTest(TestParseCondition, "TestParseCondition");
}
































