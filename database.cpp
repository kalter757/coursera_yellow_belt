#include "database.h"


void Database::Add(const Date& date, const string& event) {
    auto result = storage[date].insert(event);
    if ( result.second )
    {
        storage_vector[date].push_back(event);
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
        stream << storage.rbegin()->first << " " <<  *storage.rbegin()->second.rbegin();
    }
    else
    {
        if (date < result->first)
        {
            throw invalid_argument("Less date event!");
        }

        stream << result->first << " " << *result->second.rbegin();
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

int Database::RemoveIf(function<bool (const Date &, const string &)> func)
{
    int removed = 0;

    for (auto& record : storage)
    {
        Date date = record.first;
        for(auto it = record.second.begin(); it != record.second.end(); )
        {
            if (func(record.first, *it)) {
                it = record.second.erase(it);
                ++removed;
            } else {
                ++it;
            }
        }

        auto it = stable_partition(storage_vector[record.first].begin(), storage_vector[record.first].end(), [&func,&date](const string& event){return !func(date, event);});
        if (it != storage_vector[record.first].end())
            storage_vector[record.first].erase(it, storage_vector[record.first].end());
    }

    for(auto it = storage.begin(); it != storage.end(); )
    {
        if (it->second.empty()) {
            it = storage.erase(it);
        } else {
            ++it;
        }
    }

    for(auto it = storage_vector.begin(); it != storage_vector.end(); )
    {
        if (it->second.empty()) {
            it = storage_vector.erase(it);
        } else {
            ++it;
        }
    }

    return removed;
}



















