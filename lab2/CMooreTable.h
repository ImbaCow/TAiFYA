#pragma once
#include "pch.h"
#include "CMinimizableTable.h"

class CMooreTable : public CMinimizableTable
{
public:
	CMooreTable(std::istream& stream);
	CMooreTable(const std::vector<size_t>& outputStateMap, const std::map<size_t, std::vector<size_t>>& stateTable);
	virtual ~CMooreTable() = default;

	virtual Graph ToGraph() const override;
	virtual void ToStream(std::ostream& stream) const override;

	std::vector<size_t> GetOutputStateMap() const;
	std::map<size_t, std::vector<size_t>> GetStateTable() const;

protected:
	virtual std::vector<std::map<size_t, std::vector<size_t>>> GetTableForMinimize() const override;
	virtual size_t GetToState(size_t fromState, size_t xValue) const override;
	virtual void HandleNewStates(const std::map<size_t, std::vector<size_t>>& newStates) override;

private:
	std::vector<size_t> m_outputStateMap;
	std::map<size_t, std::vector<size_t>> m_stateTable;

	void InsertLine(const std::string& line);
};
