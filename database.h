#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "date.h"

using namespace std;


class Database {
public:
  void Add(const Date& date, const string& event);

  bool DeleteEvent(const Date& date, const string& event);

  int DeleteDate(const Date& date);

  set<string> Find(const Date& date) const;

  void Print(iostream& stream) const;

private:
  map<Date, set<string>> storage;
};
