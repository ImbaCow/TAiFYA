#include "pch.h"
#include "CMinimizableTable.h"

bool IsVectorsEqual(const std::vector<size_t>& first, const std::vector<size_t>& second)
{
	bool result = first.size() == second.size();
	for (size_t i = 0; result && i < first.size(); ++i)
	{
		result = result && first[i] == second[i];
	}

	return result;
}

bool ContainVector(const std::map<size_t, std::vector<size_t>>& container, const std::vector<size_t>& vector)
{
	bool result = false;
	for (const auto& statePair : container)
	{
		result = result || IsVectorsEqual(statePair.second, vector);
	}

	return result;
}

std::vector<std::map<size_t, std::vector<size_t>>> GroupTableMap(const std::map<size_t, std::vector<size_t>>& mapTable)
{
	std::vector<std::map<size_t, std::vector<size_t>>> resultTableMapGroups;
	for (const auto& statePair : mapTable)
	{
		auto itemIt = std::find_if(resultTableMapGroups.begin(), resultTableMapGroups.end(), [statePair](const std::map<size_t, std::vector<size_t>>& container) {
			return ContainVector(container, statePair.second);
		});

		if (itemIt == resultTableMapGroups.end())
		{
			std::map<size_t, std::vector<size_t>> newContainer{ statePair };
			resultTableMapGroups.push_back(newContainer);
		}
		else
		{
			(*itemIt)[statePair.first] = statePair.second;
		}
	}

	return resultTableMapGroups;
}

std::vector<std::map<size_t, std::vector<size_t>>> GroupTable(const std::vector<std::map<size_t, std::vector<size_t>>>& table)
{
	std::vector<std::map<size_t, std::vector<size_t>>> resultTable;
	for (const auto& tableMap : table)
	{
		for (const auto& tableMapGroup : GroupTableMap(tableMap))
		{
			resultTable.push_back(tableMapGroup);
		}
	}

	return resultTable;
}

void CMinimizableTable::CalculateVectors(std::vector<std::map<size_t, std::vector<size_t>>>& table) const
{
	for (auto& tableMap : table)
	{
		for (auto& statePair : tableMap)
		{
			for (size_t i = 0; i < statePair.second.size(); i++)
			{
				size_t toState = GetToState(statePair.first, i);
				auto newVectorIt = std::find_if(table.begin(), table.end(), [toState](const std::map<size_t, std::vector<size_t>>& container) {
					return container.find(toState) != container.end();
				});
				statePair.second[i] = std::distance(table.begin(), newVectorIt);
			}
		}
	}
}

std::vector<std::map<size_t, std::vector<size_t>>> CMinimizableTable::MinimizeTable(const std::vector<std::map<size_t, std::vector<size_t>>>& table) const
{
	std::vector<std::map<size_t, std::vector<size_t>>> resultTable = table;
	size_t prevSize;
	size_t currSize = resultTable.size();

	do
	{
		resultTable = GroupTable(resultTable);
		CalculateVectors(resultTable);

		prevSize = currSize;
		currSize = resultTable.size();
	} while (prevSize != currSize);

	return resultTable;
}

std::map<size_t, std::vector<size_t>> GetReducedTable(const std::vector<std::map<size_t, std::vector<size_t>>>& table)
{
	std::map<size_t, std::vector<size_t>> resultStates;
	for (const auto& tableMap : table)
	{
		resultStates[tableMap.begin()->first] = tableMap.begin()->second;
	}

	return resultStates;
}

void RemoveUnusedVertexes(std::map<size_t, std::vector<size_t>>& table)
{
	std::set<size_t> stateSet;
	for (const auto& tableItem : table)
	{
		for (const auto& itemArr : tableItem.second)
		{
			stateSet.insert(itemArr);
		}
	}
	for (auto it = table.begin(); it != table.end();)
	{
		if (stateSet.find(it->first) == stateSet.end())
		{
			it = table.erase(it);
		}
		else
		{
			++it;
		}
	}
}

std::map<size_t, std::vector<size_t>> RemoveUnused(const std::map<size_t, std::vector<size_t>>& table)
{
	std::map<size_t, std::vector<size_t>> resultTable = table;
	size_t prevSize;
	size_t currSize = resultTable.size();

	do
	{
		RemoveUnusedVertexes(resultTable);

		prevSize = currSize;
		currSize = resultTable.size();
	} while (prevSize != currSize);

	return resultTable;
}

void CMinimizableTable::Minimize()
{
	std::vector<std::map<size_t, std::vector<size_t>>> table = GetTableForMinimize();
	table = MinimizeTable(table);
	std::map<size_t, std::vector<size_t>> reducedTable = GetReducedTable(table);
	reducedTable = RemoveUnused(reducedTable);

	HandleNewStates(reducedTable);
}
