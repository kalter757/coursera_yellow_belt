#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Date {
public:
  // конструктор выбрасывает исключение, если его аргументы некорректны
  Date(int new_year, int new_month, int new_day);

  int GetYear() const;
  int GetMonth() const;
  int GetDay() const;

private:
  int year;
  int month;
  int day;
};
