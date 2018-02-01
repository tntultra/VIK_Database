#include "stdafx.h"
#include "Database.h"

size_t CEventRecord::NEXT_SEQUENCE_NUMBER = 0;

void CDatabase::Add (const CDate& date, const std::string& event)
{
	auto dateExists = EventsByDate_.find(date);
	if (dateExists != EventsByDate_.end()) {
		auto eventOnDateExists = dateExists->second.find (event);
		if (eventOnDateExists != dateExists->second.end()) {
			return;
		}
	}
	AllRecords_.push_back(CEventRecord{ date,event });
	EventsByDate_[date].insert(&AllRecords_.back());
	DatesByEvent_[event].insert(&AllRecords_.back());
}

void CDatabase::Print(std::ostream& out)
{
	for (auto& record : AllRecords_) {
		out << record;
	}
}
