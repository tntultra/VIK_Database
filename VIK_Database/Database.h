#pragma once

#include "date.h"
#include <utility>
#include <list>
#include <map>
#include <vector>

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

class CDatabase
{
	std::list<CEventRecord> AllEvents_;
	std::map<CDate, std::map<std::string, CEventRecord*, CEventRecord>> EventsByDate_;
	std::map<std::string, std::map<CDate, CEventRecord*, CEventRecord>> DatesByEvent_;

	using EventIndexerValueType = std::map<std::string, CEventRecord*, CEventRecord>::value_type;
	using DateIndexerValueType = std::map<CDate, CEventRecord*, CEventRecord>::value_type;

public:

	void Add (const CDate& date, const std::string& event);
	void Print (const std::ostream& cout);
	void RemoveIf (<unknown> predicate);
	std::vector<CEventRecord> FindIf (<unknown> predicate);
	std::string Last (<unknown> parseCDate);
};
