#include "stdafx.h"
#include "Date.h"
#include <iomanip>
#include <vector>

using namespace std;

bool operator<(const CDate& lhs, const CDate& rhs)
{
	if (lhs.GetYear() != rhs.GetYear()) return lhs.GetYear()<rhs.GetYear();
	if (lhs.GetMonth() != rhs.GetMonth()) return lhs.GetMonth()<rhs.GetMonth();
	if (lhs.GetDay() != rhs.GetDay()) return lhs.GetDay() < rhs.GetDay();
	return false;
}

bool operator>(const CDate& lhs, const CDate& rhs)
{
	return (lhs != rhs) && !(lhs < rhs);
}

bool operator== (const CDate& lhs, const CDate& rhs) {
	return (lhs.GetYear() == rhs.GetYear()) && (lhs.GetMonth() == rhs.GetMonth()) && (lhs.GetDay() == rhs.GetDay());
}

bool operator!= (const CDate& lhs, const CDate& rhs) {
	return !(lhs == rhs);
}

ostream& operator<<(ostream& stream, const CDate& date)
{
	stream << setfill('0');
	stream << setw(4) << date.GetYear() << "-" << setw(2) << date.GetMonth() << "-"
		<< setw(2) << date.GetDay();
	return stream;
}

bool validateSymbol(istream& stream, const char ch)
{
	return stream.peek() == ch;
}

istream& operator>>(istream& stream, CDate& date)
{
	vector<int> vec(3);
	stringstream ss;
	string dateline;
	stream >> dateline;
	ss << dateline;
	char cnt(0);
	for (auto& item : vec)
	{
		ss >> item;
		if (cnt<2 && !validateSymbol(ss))
		{
			throw runtime_error("Wrong date format: " + dateline);
		}
		ss.ignore(1, '-');
		cnt++;
	}
	if (ss) throw runtime_error("Wrong date format: " + dateline);
	date.SetYear(vec[0]);
	date.SetMonth(vec[1]);
	date.SetDay(vec[2]);
	return stream;
}

CDate ParseDate (std::istream& is)
{
	CDate date;
	is >> date;
	return date;
}
