#include "pch.h"
#include "CMooreTable.h"

size_t GetNumFromState(const std::string& state)
{
	char ch;
	size_t result;
	std::stringstream ss(state);
	ss >> ch >> result;

	return result;
}

std::vector<size_t> ParseLine(const std::string& line)
{
	std::vector<size_t> resultArr;

	std::istringstream ss(line);
	std::string state;
	while (ss >> state)
	{
		resultArr.push_back(GetNumFromState(state));
	}

	return resultArr;
}

void CMooreTable::InsertLine(const std::string& line)
{
	std::istringstream ss(line);
	std::string state;
	for (size_t i = 0; ss >> state; ++i)
	{
		m_stateTable[i].push_back(GetNumFromState(state));
	}
}

CMooreTable::CMooreTable(const std::vector<size_t>& outputStateMap, const std::map<size_t, std::vector<size_t>>& stateTable)
	: m_outputStateMap(outputStateMap)
	, m_stateTable(stateTable)
{
}

CMooreTable::CMooreTable(std::istream& stream)
{
	std::string line;
	std::getline(stream, line);
	m_outputStateMap = ParseLine(line);

	while (std::getline(stream, line))
	{
		InsertLine(line);
	}
}

std::vector<size_t> CMooreTable::GetOutputStateMap() const
{
	return m_outputStateMap;
}

std::map<size_t, std::vector<size_t>> CMooreTable::GetStateTable() const
{
	return m_stateTable;
}

std::vector<std::map<size_t, std::vector<size_t>>> CMooreTable::GetTableForMinimize() const
{
	std::vector<std::map<size_t, std::vector<size_t>>> resultTable;

	std::map<size_t, std::vector<size_t>> container;
	for (const auto& tableMap : m_stateTable)
	{
		size_t yValue = m_outputStateMap[tableMap.first];
		container[tableMap.first] = std::vector<size_t>(tableMap.second.size(), yValue);
	}
	resultTable.push_back(container);

	return resultTable;
}

size_t CMooreTable::GetToState(size_t fromState, size_t xValue) const
{
	return m_stateTable.at(fromState)[xValue];
}

void CMooreTable::HandleNewStates(const std::map<size_t, std::vector<size_t>>& newStates)
{
	m_stateTable = newStates;
}

Graph CMooreTable::ToGraph() const
{
	Graph graph;

	std::vector<Graph::vertex_descriptor> vertexMap(m_stateTable.size());
	for (const auto& statePair : m_stateTable)
	{
		std::string vertexLabel = 'q' + std::to_string(statePair.first) + "/y" + std::to_string(m_outputStateMap[statePair.first]);
		vertexMap[statePair.first] = boost::add_vertex({ vertexLabel }, graph);
	}

	for (const auto& statePair : m_stateTable)
	{
		for (size_t j = 0; j < statePair.second.size(); ++j)
		{
			std::string edgeLabel = 'x' + std::to_string(j + 1);
			boost::add_edge(vertexMap[statePair.first], vertexMap[statePair.second[j]], { edgeLabel }, graph);
		}
	}

	return graph;
}

void CMooreTable::ToStream(std::ostream& stream) const
{
	if (!m_stateTable.size() || !m_stateTable.begin()->second.size())
	{
		return;
	}
	for (size_t i = 0; i < m_outputStateMap.size(); ++i)
	{
		stream << 'y' + std::to_string(m_outputStateMap[i]) << ' ';
	}
	stream << std::endl;
	for (size_t i = 0; i < m_stateTable.begin()->second.size(); i++)
	{
		for (const auto& statePair : m_stateTable)
		{
			stream << 'q' << statePair.second[i] << 'y' << m_outputStateMap[statePair.second[i]] << ' ';
		}
		stream << std::endl;
	}
}
