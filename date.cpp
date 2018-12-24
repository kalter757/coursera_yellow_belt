#include "date.h"

// конструктор выбрасывает исключение, если его аргументы некорректны
Date::Date(int new_year, int new_month, int new_day) {
    year = new_year;
    if (new_month > 12 || new_month < 1) {
        throw logic_error("Month value is invalid: " + to_string(new_month));
    }
    month = new_month;
    if (new_day > 31 || new_day < 1) {
        throw logic_error("Day value is invalid: " + to_string(new_day));
    }
    day = new_day;
}

int Date::GetYear() const {
    return year;
}
int Date::GetMonth() const {
    return month;
}
int Date::GetDay() const {
    return day;
}

Date ParseDate(istream& stream)
{
    string date_str;
    if (stream.peek() == ' ')
        stream.ignore(1);

    getline(stream, date_str, ' ');
    istringstream is(date_str);

    bool ok = true;

    int year = 0;
    ok = ok && (is >> year);
    ok = ok && (is.peek() == '-');
    is.ignore(1);

    int month = 0;
    ok = ok && (is >> month);
    ok = ok && (is.peek() == '-');
    is.ignore(1);

    int day = 0;
    ok = ok && (is >> day);
    ok = ok && (is.eof());

    if (!ok)
    {
        throw logic_error("Wrong date format: " + date_str);
    }

    return {year, month, day};
}

ostream& operator<<(ostream& stream, const Date &rhs) {
  stream << setw(4) << setfill('0') << rhs.GetYear() <<
      "-" << setw(2) << setfill('0') << rhs.GetMonth() <<
      "-" << setw(2) << setfill('0') << rhs.GetDay();
  return stream;
}

bool operator<(const Date &lhs, const Date &rhs)
{
    return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) < make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool operator==(const Date &lhs, const Date &rhs)
{
    return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) == make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool operator!=(const Date &lhs, const Date &rhs)
{
    return !(lhs == rhs);
}

bool operator>(const Date &lhs, const Date &rhs)
{
    return (lhs != rhs) && (rhs < lhs);
}

bool operator<=(const Date &lhs, const Date &rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

bool operator>=(const Date &lhs, const Date &rhs)
{
    return (lhs > rhs) || (lhs == rhs);
}
