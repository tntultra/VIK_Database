#include "stdafx.h"

#include "Database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
#include "TestFramework.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

using namespace std;

string ParseEvent(istream& is) {
  // Реализуйте эту функцию
	string temp;
	getline(is, temp);
	string::iterator start, end;
	for (start = temp.begin(); start != temp.end() && isspace(*start); ++start);
	return std::string{ start,temp.end() };
}

void TestAll();

int main() {
  //TestAll();

  fstream fin("commands", ios::in);
  CDatabase db;

  //  for (string line; getline(cin, line); ) {
  for (string line; getline(fin, line); ) {
	istringstream is(line);
	try {
		db.ParseCommand(is);
	}
	catch (const logic_error& error) {
		cout << error.what() << endl;
	}
	catch (...) {
		cout << "Catched unexpected exception" << endl;
	}
  }

  return 0;
}

void TestParseEvent() {
  {
    istringstream is("event");
    AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
  }
  {
    istringstream is("   sport event ");
    AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
  }
  {
    istringstream is("  first event  \n  second event");
    vector<string> events;
    events.push_back(ParseEvent(is));
    events.push_back(ParseEvent(is));
    AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
  }
}

void TestAll() {
  TestRunner tr;
  tr.RunTest(TestParseEvent, "TestParseEvent");
  tr.RunTest(TestParseCondition, "TestParseCondition");
	tr.RunTest(TestDatabaseOperations, "TestDatabaseOperations");
}
