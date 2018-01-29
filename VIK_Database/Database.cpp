#include "stdafx.h"
#include "Database.h"

void CDatabase::Add (const CDate& date, const std::string& event)
{
	auto dateExists = EventsByDate_.find(date);
	if (dateExists != EventsByDate_.end()) {
		auto eventOnDateExists = dateExists->second.find (event);
		if (eventOnDateExists != dateExists->second.end()) {
			return;
		}
	}
	AllEvents_.push_back(CEventRecord{ date,event });
	EventsByDate_[date].insert(EventIndexerValueType{ event, &AllEvents_.back() });
	DatesByEvent_[event].insert(DateIndexerValueType{ date, &AllEvents_.back() });
}
