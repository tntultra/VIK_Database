#include "stdafx.h"
#include "Database.h"
#include <algorithm>

size_t CEventRecord::NEXT_SEQUENCE_NUMBER = 0;

void CDatabase::Add (const CDate& date, const std::string& event)
{
	CEventRecord newRec{ date, event };
	auto dateExists = EventsByDate_.find(date);
	if (dateExists != EventsByDate_.end()) {
		auto eventOnDateExists = dateExists->second.find(&newRec);
		if (eventOnDateExists != dateExists->second.end()) {
			return;
		}
	}
	AllRecords_.push_back(newRec);
	EventsByDate_[date].insert(IndexedValueType{&AllRecords_.back() });
	//DatesByEvent_[event].insert(IndexedValueType { &AllRecords_.back()});
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
	std::remove_if (begin(AllRecords_), end(AllRecords_), [&](const CEventRecord& rec) mutable {
		auto remove = pred(rec.Date_, rec.Event_);
		if (remove) {
			EventsByDate_[rec.Date_].erase(&rec);
			++removedCount;
		}
		return remove;
	});
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

