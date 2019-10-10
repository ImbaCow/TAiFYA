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

std::vector<MealyTableItem> ParseLine(const std::string& line)
{
	std::vector<MealyTableItem> resultArr;

	std::istringstream ss(line);
	std::string state;
	while (ss >> state)
	{
		resultArr.push_back(GetNumFromState(state));
	}

	return resultArr;
}

CMealyTable::CMealyTable(const std::vector<std::vector<MealyTableItem>>& stateTable)
	: m_stateTable(stateTable)
{
}

CMealyTable::CMealyTable(std::istream& stream)
{
	std::string line;
	while (std::getline(stream, line))
	{
		m_stateTable.push_back(ParseLine(line));
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
	for (size_t i = 0; i < m_stateTable[0].size(); i++)
	{
		std::string vertexLabel = 'q' + std::to_string(i);
		vertexArr.push_back(boost::add_vertex({ vertexLabel }, graph));
	}

	for (size_t i = 0; i < m_stateTable.size(); i++)
	{
		const std::vector<MealyTableItem>& line = m_stateTable[i];
		for (size_t j = 0; j < line.size(); j++)
		{
			std::string edgeLabel = 'x' + std::to_string(i + 1) + "/y" + std::to_string(line[j].output);
			boost::add_edge(vertexArr[j], vertexArr[line[j].state], { edgeLabel }, graph);
		}
	}

	return graph;
}

void CMealyTable::ToStream(std::ostream& stream) const
{
	for (const auto& line : m_stateTable)
	{
		for (const auto val : line)
		{
			stream << 'q' << val.state << 'y' << val.output << ' ';
		}
		stream << std::endl;
	}
}

std::vector<std::vector<MealyTableItem>> CMealyTable::GetStateTable() const
{
	return m_stateTable;
}
