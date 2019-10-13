#pragma once
#include "pch.h"
#include "CMinimizableTable.h"

struct MealyTableItem
{
	size_t state;
	size_t output;
};

class CMealyTable : public CMinimizableTable
{
public:
	CMealyTable(std::istream& stream);
	CMealyTable(const std::map<size_t, std::vector<MealyTableItem>>& stateTable);
	virtual ~CMealyTable() = default;

	virtual Graph ToGraph() const override;
	virtual void ToStream(std::ostream& stream) const override;

	std::map<size_t, std::vector<MealyTableItem>> GetStateTable() const;

protected:
	virtual std::vector<std::map<size_t, std::vector<size_t>>> GetTableForMinimize() const override;
	virtual size_t GetToState(size_t fromState, size_t xValue) const override;
	virtual void HandleNewStates(const std::map<size_t, std::vector<size_t>>& newStates) override;

private:
	std::map<size_t, std::vector<MealyTableItem>> m_stateTable;

	void InsertLine(const std::string& line);
};
