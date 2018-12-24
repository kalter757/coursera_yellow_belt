#include "database.h"


void Database::Add(const Date& date, const string& event) {
    auto result = storage[date].insert(event);
    if ( result.second )
    {
        storage_vector[date].push_back(event);
    }
}

bool Database::DeleteEvent(const Date& date, const string& event) {
//    if (storage.count(date) > 0 && storage[date].count(event) > 0) {
//        storage[date].erase(event);
//        return true;
//    }
    return false;
}

int Database::DeleteDate(const Date& date) {
    if (storage.count(date) == 0) {
        return 0;
    } else {
        const int event_count = storage[date].size();
        storage.erase(date);
        return event_count;
    }
}

set<string> Database::Find(const Date& date) const {
    if (storage.count(date) > 0) {
        return storage.at(date);
    } else {
        return {};
    }
}

void Database::Print(ostream &stream) const {
    for (const auto& item : storage_vector) {
        for (const string& event : item.second) {
            stream << item.first << " " << event << endl;
        }
    }
}

string Database::Last(const Date &date)
{
    auto result = storage.lower_bound(date);

    ostringstream stream;
    if (result == storage.end())
    {
        stream << storage.rbegin()->first << " " <<  *storage.rbegin()->second.rbegin() << endl;
    }
    else
    {
        if (date < result->first)
        {
            throw invalid_argument("Less date event!");
        }

        stream << result->first << " " << *result->second.rbegin() << endl;
    }

    return stream.str();
}

vector<string> Database::FindIf(function<bool (const Date& date, const string& event)> func)
{
    vector<string> result;

    for (const auto& date_events : storage_vector)
    {
        for (const auto& event : date_events.second)
        {
            if (func(date_events.first, event))
            {
                ostringstream stream;
                stream << date_events.first << " " << event;
                result.push_back(stream.str());
            }
        }
    }

    return result;
}
