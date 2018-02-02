#pragma once

#include "date.h"
#include <utility>
#include <list>
#include <map>
#include <vector>
#include <set>
#include "condition_parser.h"

struct CEventRecord {
	static size_t NEXT_SEQUENCE_NUMBER;

	CEventRecord() = default;

	explicit CEventRecord(const CDate& date, const std::string& event) :
		Date_{ date },
		Event_{ event },
		SequenceNumber_{ NEXT_SEQUENCE_NUMBER++ }
	{}
	
	CDate Date_;
	std::string Event_;
	size_t SequenceNumber_;


	bool operator()(const CEventRecord* lhs, const CEventRecord* rhs) const {
		return lhs->SequenceNumber_ < rhs->SequenceNumber_;
	}
};

bool operator==(const CEventRecord* lhs, const CEventRecord& rhs);

inline std::ostream& operator<<(std::ostream& os, const CEventRecord& rec) {
	os << rec.Date_ << ' ' << rec.Event_ << '\n';
	return os;
}

class CDatabase
{
	std::list<CEventRecord> AllRecords_;

	using IndexedRecord = std::set<const CEventRecord*, CEventRecord>;
	using IndexedValueType = IndexedRecord::value_type;

	//using IndexedEventRecord = std::map<CDate, CEventRecord*, CEventRecord>;
	//using IndexedEventValueType = IndexedEventRecord::value_type;

	std::map<CDate, IndexedRecord> EventsByDate_;
	//std::map<std::string, IndexedRecord> DatesByEvent_;

public:

	void Add (const CDate& date, const std::string& event);
	void Print (std::ostream& out);
	size_t RemoveIf (PREDICATE_FUNCTION pred);
	std::vector<CEventRecord> FindIf (PREDICATE_FUNCTION pred);
	//std::string Last (const CDate&);
	std::string Last(CDate&&);
	void ParseCommand(istream& is);

	friend void TestDatabaseOperations();
};

