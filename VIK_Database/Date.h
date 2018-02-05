#pragma once
#include <string>
#include <sstream>

template <typename T>
std::string tostring(T val)
{
	std::stringstream ss;
	ss << val;
	return ss.str();
}

class CDate
{
private:
	int year;
	int month;
	int day;
public:
	CDate() :year(-1), month(-1), day(-1) {};
	CDate(const int& y, const int& m, const int& d)
	{
		SetYear(y);
		SetMonth(m);
		SetDay(d);
	}
	int GetYear() const
	{
		return year;
	}
	int GetMonth() const
	{
		return month;
	}
	int GetDay() const
	{
		return day;
	}
	void SetYear(const int& y)
	{
		year = y;
	}
	void SetMonth(const int& m)
	{
		if (m>12 || m < 1)
		{
			auto err("Month value is invalid: " + tostring(m));
			throw std::invalid_argument(err);
		}
		month = m;
	}
	void SetDay(const int& d)
	{
		if (d>31 || d<1)
		{
			auto err("Day value is invalid: " + tostring(d));
			throw std::invalid_argument(err);
		}
		day = d;
	}
};

bool operator<(const CDate& lhs, const CDate& rhs);
bool operator>(const CDate& lhs, const CDate& rhs);
bool operator==(const CDate& lhs, const CDate& rhs);
bool operator!= (const CDate& lhs, const CDate& rhs);
std::ostream& operator<<(std::ostream& stream, const CDate& date);
bool validateSymbol(std::istream& stream, const char ch = '-');
std::istream& operator>>(std::istream& stream, CDate& date);

CDate ParseDate(std::istream& is);
