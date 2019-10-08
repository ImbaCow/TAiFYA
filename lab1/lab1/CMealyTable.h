#pragma once
#include "pch.h"
#include "ITable.h"

struct  MealyTableItem
{
	size_t state;
	size_t output;
};

class CMealyTable : public ITable
{
public:
	CMealyTable(std::istream& stream);
	CMealyTable(const std::vector<std::vector<MealyTableItem>>& stateTable);
	virtual ~CMealyTable() = default;

	virtual Graph ToGraph() const override;
	virtual void ToStream(std::ostream& stream) const override;
	std::vector<std::vector<MealyTableItem>> GetStateTable() const;

private:
	std::vector<std::vector<MealyTableItem>> m_stateTable;
};
