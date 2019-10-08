#pragma once
#include "pch.h"
#include "ITable.h"

class CMooreTable : public ITable
{
public:
	CMooreTable(std::istream& stream);
	CMooreTable(const std::vector<size_t>& outputStateMap, const std::vector<std::vector<size_t>>& stateTable);
	virtual ~CMooreTable() = default;

	virtual Graph ToGraph() const override;
	virtual void ToStream(std::ostream& stream) const override;
	std::vector<size_t> GetOutputStateMap() const;
	std::vector<std::vector<size_t>> GetStateTable() const;

private:
	std::vector<size_t> m_outputStateMap;
	std::vector<std::vector<size_t>> m_stateTable;
};
