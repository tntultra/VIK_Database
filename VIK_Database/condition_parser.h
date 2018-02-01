#pragma once

#include "node.h"

#include <memory>
#include <iostream>
#include <functional>
#include "Date.h"

using namespace std;

using PREDICATE_FUNCTION = std::function<bool(const CDate&, const std::string&)>;

shared_ptr<CNode> ParseCondition(istream& is);

void TestParseCondition();
