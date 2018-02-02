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


size_t CEventRecord::NEXT_SEQUENCE_NUMBER = 0;

bool operator==(const CEventRecord* lhs, const CEventRecord& rhs) {
	return (lhs->Date_ == rhs.Date_) && (lhs->Event_ == rhs.Event_);
}

void CDatabase::Add (const CDate& date, const std::string& event)
{
	CEventRecord newRec{ date, event };
	auto dateExists = EventsByDate_.find(date);
	if (dateExists != EventsByDate_.end()) {
		//auto eventOnDateExists = dateExists->second.find(&newRec);
		auto eventOnDateExists = std::find (begin(dateExists->second), end(dateExists->second), newRec);
		if (eventOnDateExists != dateExists->second.end()) {
			return;
		}
	}
	AllRecords_.push_back(newRec);
	EventsByDate_[date].insert(IndexedValueType{&AllRecords_.back() });
}

void CDatabase::Print(std::ostream& out)
{
	for (auto& recordsByDate : EventsByDate_) {
		for (auto recordPtr : recordsByDate.second) {
			out << *recordPtr;
		}
	}
}

size_t CDatabase::RemoveIf (PREDICATE_FUNCTION pred)
{
	size_t removedCount = 0;

	AllRecords_.erase(
		std::remove_if(begin(AllRecords_), end(AllRecords_), [&](const CEventRecord& rec) mutable {
			auto remove = pred(rec.Date_, rec.Event_);
			if (remove) {
				auto recsByDateIt = EventsByDate_.find(rec.Date_);
				if (recsByDateIt != EventsByDate_.end()) {
					recsByDateIt->second.erase(&rec);
					if (!recsByDateIt->second.size()) {
						EventsByDate_.erase(rec.Date_);
					}
				}
				++removedCount;
			}
			return remove;
		}),
		end(AllRecords_)
	);
	return removedCount;
	
}

std::vector<CEventRecord> CDatabase::FindIf (PREDICATE_FUNCTION pred)
{
	std::vector<CEventRecord> matches;
	std::copy_if(begin(AllRecords_), end(AllRecords_), std::back_inserter(matches), [&](const CEventRecord& rec) {
		return pred(rec.Date_,rec.Event_);
	});
	return matches;
}

std::string CDatabase::Last (CDate&& date)
{
	auto ub = EventsByDate_.upper_bound (date);
	if (ub != EventsByDate_.end()) {
		auto lastEvent = *(--ub->second.end());
		return lastEvent->Event_;
	} 
	return {"No entries"};
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
				Assert((db.AllRecords_.begin()->Date_ == CDate{ 2017,6,1 }), "Check added record date");
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
				Assert(((*(exDateRecs.begin()))->Event_ == "8th of July"), "Check that order is correct");
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

		}
	}
}