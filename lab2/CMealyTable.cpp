#pragma once
#include "pch.h"
#include "CMealyTable.h"

MealyTableItem GetNumFromState(const std::string& state)
{
	char ch;
	MealyTableItem result;
	std::stringstream ss(state);
	ss >> ch >> result.state >> ch >> result.output;

	return result;
}

void CMealyTable::InsertLine(const std::string& line)
{
	std::istringstream ss(line);
	std::string state;
	for (size_t i = 0; ss >> state; ++i)
	{
		m_stateTable[i].push_back(GetNumFromState(state));
	}
}

size_t CMealyTable::GetToState(size_t fromState, size_t xValue) const
{
	return m_stateTable.at(fromState)[xValue].state;
}

void CMealyTable::HandleNewStates(const std::map<size_t, std::vector<size_t>>& newStates)
{
	std::map<size_t, std::vector<MealyTableItem>> newStateTable;
	size_t i = 0;
	for (const auto& statePair : newStates)
	{
		std::vector<MealyTableItem> newStateItem;
		for (size_t j = 0; j < statePair.second.size(); ++j)
		{
			size_t y = m_stateTable.at(statePair.first)[j].output;
			newStateItem.push_back({statePair.second[j], y });
		}
		newStateTable[i] = newStateItem;
		++i;
	}

	m_stateTable = newStateTable;
}

CMealyTable::CMealyTable(const std::map<size_t, std::vector<MealyTableItem>>& stateTable)
	: m_stateTable(stateTable)
{
}

CMealyTable::CMealyTable(std::istream& stream)
{
	std::string line;
	while (std::getline(stream, line))
	{
		InsertLine(line);
	}
}

Graph CMealyTable::ToGraph() const
{
	Graph graph;
	if (!m_stateTable.size())
	{
		return graph;
	}
	std::vector<Graph::vertex_descriptor> vertexArr;
	for (const auto& pair : m_stateTable)
	{
		std::string vertexLabel = 'q' + std::to_string(pair.first);
		vertexArr.push_back(boost::add_vertex({ vertexLabel }, graph));
	}

	for (const auto& statePair : m_stateTable)
	{
		for (size_t i = 0; i < statePair.second.size(); ++i)
		{
			std::string edgeLabel = 'x' + std::to_string(i + 1) + "/y" + std::to_string(statePair.second[i].output);
			boost::add_edge(vertexArr[statePair.first], vertexArr[statePair.second[i].state], { edgeLabel }, graph);
		}
	}

	return graph;
}

void CMealyTable::ToStream(std::ostream& stream) const
{
	if (!m_stateTable.size() || !m_stateTable.begin()->second.size())
	{
		return;
	}
	for (size_t i = 0; i < m_stateTable.begin()->second.size(); i++)
	{
		for (const auto& statePair : m_stateTable)
		{
			stream << 'q' << statePair.second[i].state << 'y' << statePair.second[i].output << ' ';
		}
		stream << std::endl;
	}
}

std::map<size_t, std::vector<MealyTableItem>> CMealyTable::GetStateTable() const
{
	return m_stateTable;
}

std::vector<size_t> ToMinimizeArr(const std::vector<MealyTableItem>& arr)
{
	std::vector<size_t> resultArr;
	for (const auto& pair : arr)
	{
		resultArr.push_back(pair.output);
	}

	return resultArr;
}

std::vector<std::map<size_t, std::vector<size_t>>> CMealyTable::GetTableForMinimize() const
{
	std::map<size_t, std::vector<size_t>> resultMap;
	for (const auto& statePair : m_stateTable)
	{
		resultMap[statePair.first] = ToMinimizeArr(statePair.second);
	}

	std::vector<std::map<size_t, std::vector<size_t>>> resultTable{ resultMap };
	return resultTable;
}
