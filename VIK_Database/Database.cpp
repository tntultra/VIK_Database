#include "stdafx.h"
#include "Database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
#include "TestFramework.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

bool operator<(const CEventRecord& lhs, const CEventRecord& rhs) {
	if (lhs.Date_ != rhs.Date_) {
		return (lhs.Date_ < rhs.Date_);
	}
	if (strcmp (lhs.Event_.c_str(), rhs.Event_.c_str()) < 0) {
		return true;
	}
	return false;
}

void CDatabase::Add (const CDate& date, const std::string& event)
{
	CEventRecord newRec{ date, event };
	auto recExists = AllRecords_.find(newRec);
	if (recExists != end(AllRecords_)) {
		return;
	}
	auto& evOnDate = EventsByDate_[date];
	evOnDate.push_back(newRec);
	AllRecords_.insert(newRec);
}

void CDatabase::Print(std::ostream& out)
{
	for (const auto& recordsByDate : EventsByDate_) {
		for (const auto& recordPtr : recordsByDate.second) {
			out << recordPtr << endl;
		}
	}
}

size_t CDatabase::RemoveIf (PREDICATE_FUNCTION pred)
{
	size_t removedCount = 0;

	for (auto d_it = begin(EventsByDate_); d_it != end(EventsByDate_);) {
		auto& evOnDateList = d_it->second;
		for (auto it = begin(evOnDateList); it != end(evOnDateList);) {
			if (pred(it->Date_, it->Event_)) {
				AllRecords_.erase(*it);
				it = evOnDateList.erase(it);
				++removedCount;
			}
			else {
				++it;
			}
		}
		if (d_it->second.size() == 0) {
			d_it = EventsByDate_.erase(d_it);
		}
		else {
			++d_it;
		}
	}
	return removedCount;
}

std::vector<CEventRecord> CDatabase::FindIf (PREDICATE_FUNCTION pred)
{
	std::vector<CEventRecord> matches;
	for (auto it = begin(EventsByDate_); it != end(EventsByDate_); ++it) {
		auto& evOnDateList = it->second;
		std::copy_if(begin(evOnDateList), end(evOnDateList), std::back_inserter(matches), [&](const CEventRecord& rec) {
			return pred(rec.Date_, rec.Event_);
		});
	}
	return matches;
}

std::string CDatabase::Last (CDate&& date)
{
	auto ub = EventsByDate_.upper_bound (date);
	if (ub != EventsByDate_.begin()) {
		auto lastEvent = *--((--ub)->second.end());
		return tostring(lastEvent);
	} 
	return { "No entries" };
}

void CDatabase::ParseCommand(istream& is) {
	string command;
	is >> command;
	if (command == "Add") {
		const auto date = ParseDate(is);
		const auto event = ParseEvent(is);
		Add(date, event);
	}
	else if (command == "Print") {
		Print(cout);
	}
	else if (command == "Del") {
		auto condition = ParseCondition(is);
		auto predicate = [condition](const CDate& date, const string& event) {
			return condition->Evaluate(date, event);
		};
		auto count = RemoveIf(predicate);
		cout << "Removed " << count << " entries" << endl;
	}
	else if (command == "Find") {
		auto condition = ParseCondition(is);
		auto predicate = [condition](const CDate& date, const string& event) {
			return condition->Evaluate(date, event);
		};

		const auto entries = FindIf(predicate);
		for (const auto& entry : entries) {
			cout << entry << endl;
		}
		cout << "Found " << entries.size() << " entries" << endl;
	}
	else if (command == "Last") {
		try {
			cout << Last(ParseDate(is)) << endl;
		}
		catch (invalid_argument&) {
			cout << "No entries" << endl;
		}
	}
	else if (command.empty()) {
		return;
	}
	else {
		throw logic_error("Unknown command: " + command);
	}
}

void TestDatabaseOperations() {
	{
		{
			//example 1
			CDatabase db;
			{
				istringstream is("Add 2017-06-01 1st of June");
				db.ParseCommand(is);
				auto val = db.AllRecords_.begin();
				Assert(((*val).Date_ == CDate{ 2017,6,1 }), "Check added record date");
			}
			{
				istringstream is("Add 2017-07-08 8th of July");
				db.ParseCommand(is);
				Assert((db.AllRecords_.size() == 2), "Added another date");
			}
			{
				istringstream is("Add 2017-07-08 Someone's birthday");
				db.ParseCommand(is);
				auto exDateRecs = db.EventsByDate_[CDate{ 2017,7,8 }];
				Assert((exDateRecs.size() == 2), "Check that record added to alrdy existing date");
				Assert(((*(exDateRecs.begin())).Event_ == "8th of July"), "Check that order is correct");
			}
			{
				istringstream is("Del date == 2017-07-08");
				db.ParseCommand(is);
				Assert((db.AllRecords_.size() == 1), "removed 2 dates");
				auto exDateRecs = db.EventsByDate_.find(CDate{ 2017,7,8 });
				Assert((exDateRecs == db.EventsByDate_.end()), "Check that no deleted date is in index");
			}
		}
		{
			//example 2
			CDatabase db;
			{
				istringstream is("Add 2017-01-01 Holiday");
				db.ParseCommand(is);
			}
			{
				istringstream is("Add 2017-03-08 Holiday");
				db.ParseCommand(is);
			}
			{
				istringstream is("Add 2017-1-1 New Year");
				db.ParseCommand(is);
			}
			{
				istringstream is("Add 2017-1-1 New Year");
				db.ParseCommand(is);
				Assert((db.AllRecords_.size() == 3), "same record isnt added twice");
			}
			{
				istringstream is("Print");
				db.ParseCommand(is);
			}
			{
				istringstream is("Find event != \"working day\"");
				db.ParseCommand(is);
				Assert((db.AllRecords_.size() == 3), "same record isnt added twice");
			}
			{
				istringstream is("Add 2017-05-09 Holiday");
				db.ParseCommand(is);
			}
			{
				istringstream is("Last 2016-12-31");
				db.ParseCommand(is);
			}
			{
				istringstream is("Last 2017-01-01");
				db.ParseCommand(is);
			}
			{
				istringstream is("Last 2017-06-01");
				db.ParseCommand(is);
			}
			{
				istringstream is("Del date > 2017-01-1");
				db.ParseCommand(is);
				Assert((db.AllRecords_.size() == 2), "only 2017.1.1 remains");
			}
			{
				istringstream is("Find");
				db.ParseCommand(is);
			}
			{
				istringstream is("Del");
				db.ParseCommand(is);
				Assert((db.EventsByDate_.size() == 0), "db is empty");
				Assert((db.AllRecords_.size() == 0), "index is empty too");
			}
			
		}
	}
}