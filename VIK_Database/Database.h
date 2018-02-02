#pragma once

#include "date.h"
#include "condition_parser.h"

#include <unordered_set>
#include <utility>
#include <list>
#include <map>
#include <vector>
#include <set>

struct CEventRecord {
	CEventRecord() = default;

	explicit CEventRecord(const CDate& date, const std::string& event) :
		Date_{ date },
		Event_{ event }
	{}
	
	CDate Date_;
	std::string Event_;
};

bool operator<(const CEventRecord& lhs, const CEventRecord& rhs);

inline std::ostream& operator<<(std::ostream& os, const CEventRecord& rec) {
	os << rec.Date_ << ' ' << rec.Event_ << '\n';
	return os;
}

class CDatabase
{
	std::map<CDate, std::list<CEventRecord>> EventsByDate_;
	std::set<CEventRecord> AllRecords_;
public:

	void Add (const CDate& date, const std::string& event);
	void Print (std::ostream& out);
	size_t RemoveIf (PREDICATE_FUNCTION pred);
	std::vector<CEventRecord> FindIf (PREDICATE_FUNCTION pred);
	//std::string Last (const CDate&);
	std::string Last (CDate&&);
	void ParseCommand(istream& is);

	friend void TestDatabaseOperations();
};

