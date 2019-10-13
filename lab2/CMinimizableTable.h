#pragma once
#include "pch.h"
#include "ITable.h"

class CMinimizableTable : public ITable
{
public:
	virtual ~CMinimizableTable() = default;

	virtual void Minimize();

protected:
	virtual std::vector<std::map<size_t, std::vector<size_t>>> GetTableForMinimize() const = 0;
	virtual size_t GetToState(size_t fromState, size_t xValue) const = 0;
	virtual void HandleNewStates(const std::map<size_t, std::vector<size_t>>& newStates) = 0;

private:
	void CalculateVectors(std::vector<std::map<size_t, std::vector<size_t>>>& table) const;
	std::vector<std::map<size_t, std::vector<size_t>>> MinimizeTable(const std::vector<std::map<size_t, std::vector<size_t>>>& table) const;
};
