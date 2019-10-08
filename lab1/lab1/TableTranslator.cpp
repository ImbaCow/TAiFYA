#pragma once
#include "pch.h"
#include "TableTranslator.h"

bool operator<(const MealyTableItem& first, const MealyTableItem& second)
{
	return first.state < second.state || first.state == second.state && first.output < second.output;
}

std::shared_ptr<CMooreTable> FromMealyToMoore(const CMealyTable& table)
{
	std::vector<std::vector<MealyTableItem>> stateTable = table.GetStateTable();

	std::set<MealyTableItem> mooreStates;
	for (const auto& line : stateTable)
	{
		for (const auto& val : line)
		{
			mooreStates.insert(val);
		}
	}

	std::vector<size_t> mooreOutputStateMap;
	std::vector<std::vector<size_t>> mooreStateTable = std::vector<std::vector<size_t>>(stateTable.size(), std::vector<size_t>());

	for (const auto& mooreState : mooreStates)
	{
		mooreOutputStateMap.push_back(mooreState.output);
		for (size_t i = 0; i < stateTable.size(); ++i)
		{
			size_t toStateIndex = std::distance(mooreStates.begin(), mooreStates.find(stateTable[i][mooreState.state]));
			mooreStateTable[i].push_back(toStateIndex);
		}
	}

	return std::make_shared<CMooreTable>(mooreOutputStateMap, mooreStateTable);
}

std::shared_ptr<CMealyTable> FromMooreToMealy(const CMooreTable& table)
{
	std::vector<std::vector<size_t>> stateTable = table.GetStateTable();
	std::vector<size_t> outputStateMap = table.GetOutputStateMap();

	std::vector<std::vector<MealyTableItem>> mealyStateTable;
	for (const auto& line : stateTable)
	{
		std::vector<MealyTableItem> mealyLine;
		for (const auto& val : line)
		{
			mealyLine.push_back({ val, outputStateMap[val] });
		}
		mealyStateTable.push_back(mealyLine);
	}
	return std::make_shared<CMealyTable>(mealyStateTable);
}
