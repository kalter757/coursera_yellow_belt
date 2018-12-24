#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

#include <stdint.h>

#include "date.h"

using namespace std;


class Database {
public:
  void Add(const Date& date, const string& event);

  set<string> Find(const Date& date) const;

  void Print(ostream& stream) const;

  string Last(const Date& date);

  vector<string> FindIf(function<bool (const Date& date, const string& event)> func);

  int RemoveIf(function<bool (const Date& date, const string& event)> func);

private:
  map<Date, set<string>> storage;
  map<Date, vector<string>> storage_vector;
};
