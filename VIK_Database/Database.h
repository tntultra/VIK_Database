#pragma once

#include "date.h"
#include <utility>
#include <list>
#include <map>
#include <vector>
#include <set>

struct CEventRecord {
	static size_t NEXT_SEQUENCE_NUMBER;

	explicit CEventRecord(const CDate& date, const std::string& event) :
		Date_{ date },
		Event_{ event },
		SequenceNumber{ NEXT_SEQUENCE_NUMBER++ }
	{}
	
	CDate Date_;
	std::string Event_;
	size_t SequenceNumber;

	bool operator()(const CEventRecord* lhs, const CEventRecord* rhs) const {
		return lhs->SequenceNumber < rhs->SequenceNumber;
	}
};

std::ostream& operator<<(std::ostream& os, const CEventRecord& rec) {
	os << rec.Date_ << ' ' << rec.Event_ << '\n';
}

class CDatabase
{
	std::list<CEventRecord> AllRecords_;

	using IndexedRecord = std::set<CEventRecord*, CEventRecord>;
	using IndexerValueType = IndexedRecord::value_type;

	std::map<CDate, std::set<CEventRecord*, CEventRecord>> EventsByDate_;
	std::map<std::string, std::set<CEventRecord*, CEventRecord>> DatesByEvent_;

public:

	void Add (const CDate& date, const std::string& event);
	void Print (std::ostream& out);
	void RemoveIf (<unknown> predicate);
	std::vector<CEventRecord> FindIf (<unknown> predicate);
	std::string Last (<unknown> parseCDate);
};
