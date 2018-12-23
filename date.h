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

  // даты будут по умолчанию выводиться в нужном формате
  ostream& operator<<(ostream& stream) {
    stream << setw(4) << setfill('0') << year <<
        "-" << setw(2) << setfill('0') << month <<
        "-" << setw(2) << setfill('0') << day;
    return stream;
  }

private:
  int year;
  int month;
  int day;
};
