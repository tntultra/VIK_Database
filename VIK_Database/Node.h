#pragma once
#include "Date.h"
#include <memory>

class CDate;

enum class Comparison
{
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual
};

template<class ToCompare>
bool Compare(Comparison cmp, const ToCompare& stored, const ToCompare& newVal)
{
	switch (cmp) {
	case Comparison::Less:
		return newVal < stored;
	case Comparison::LessOrEqual:
		return newVal < stored || newVal == stored;
	case Comparison::Greater:
		return newVal > stored;
	case Comparison::GreaterOrEqual:
		return newVal > stored || newVal == stored;
	case Comparison::Equal:
		return newVal == stored;
	case Comparison::NotEqual:
		return newVal != stored;
	}
	return false;
}

class CNode
{
public:
	virtual ~CNode() = default;
	virtual bool Evaluate(const CDate& date, const std::string& event) const = 0;
};

class CEmptyNode : public CNode
{
public:
	virtual ~CEmptyNode() = default;
	virtual bool Evaluate(const CDate& date, const std::string& event) const
	{
		return true;
	}
};


class CDateComparisonNode : public CNode
{
	Comparison Cmp_;
	CDate Date_;
public:
	explicit CDateComparisonNode(Comparison cmp, const CDate& date) :
		Cmp_{cmp},
		Date_{date}
	{}

	virtual ~CDateComparisonNode() = default;
	virtual bool Evaluate(const CDate& date, const std::string& event) const override final {
		return Compare(Cmp_, Date_, date);
		/*switch (Cmp_) {
		case Comparison::Less:
			return date < Date_;
		case Comparison::LessOrEqual:
			return date < Date_ || date == Date_;
		case Comparison::Greater:
			return date > Date_;
		case Comparison::GreaterOrEqual:
			return date > Date_ || date == Date_;
		case Comparison::Equal:
			return date == Date_;
		case Comparison::NotEqual:
			return date != Date_;
		}
		return false;*/
	}
};

class CEventComparisonNode : public CNode
{
	Comparison Cmp_;
	std::string Event_;
public:
	explicit CEventComparisonNode(Comparison cmp, const std::string& Event_) :
		Cmp_{ cmp },
		Event_{ Event_ }
	{}

	virtual ~CEventComparisonNode() = default;
	virtual bool Evaluate(const CDate& date, const std::string& event) const override final {
		return Compare(Cmp_, Event_, event);
		/*switch (Cmp_) {
		case Comparison::Less:
			return event < Event_;
		case Comparison::LessOrEqual:
			return event < Event_ || event == Event_;
		case Comparison::Greater:
			return event > Event_;
		case Comparison::GreaterOrEqual:
			return event > Event_ || event == Event_;
		case Comparison::Equal:
			return event == Event_;
		case Comparison::NotEqual:
			return event != Event_;
		}
		return false;*/
	}
};

enum class LogicalOperation
{
	And,
	Or
};

class CLogicalOperationNode : public CNode
{
	LogicalOperation Op_;
	std::shared_ptr<CNode> LHS_;
	std::shared_ptr<CNode> RHS_;

public:
	explicit CLogicalOperationNode(LogicalOperation op, std::shared_ptr<CNode> lhs, std::shared_ptr<CNode> rhs) :
		Op_{ op },
		LHS_{ lhs },
		RHS_ { rhs }
	{}

	virtual ~CLogicalOperationNode() = default;
	virtual bool Evaluate(const CDate& date, const std::string& event) const override final {
		if (Op_ == LogicalOperation::And) {
			return LHS_->Evaluate(date, event) && RHS_->Evaluate(date, event);
		} else {
			return LHS_->Evaluate(date, event) || RHS_->Evaluate(date, event);
		}
	};
};

