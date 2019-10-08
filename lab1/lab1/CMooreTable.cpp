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

CMooreTable::CMooreTable(const std::vector<size_t>& outputStateMap, const std::vector<std::vector<size_t>>& stateTable)
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
		m_stateTable.push_back(ParseLine(line));
	}
}

std::vector<size_t> CMooreTable::GetOutputStateMap() const
{
	return m_outputStateMap;
}

std::vector<std::vector<size_t>> CMooreTable::GetStateTable() const
{
	return m_stateTable;
}

Graph CMooreTable::ToGraph() const
{
	Graph graph;

	std::vector<Graph::vertex_descriptor> vertexMap(m_outputStateMap.size());
	for (size_t i = 0; i < m_outputStateMap.size(); ++i)
	{
		std::string vertexLabel = 'q' + std::to_string(i) + "/y" + std::to_string(m_outputStateMap[i]);
		vertexMap[i] = boost::add_vertex({ vertexLabel }, graph);
	}

	for (size_t i = 0; i < m_stateTable.size(); ++i)
	{
		const std::vector<size_t>& line = m_stateTable[i];
		for (size_t j = 0; j < line.size(); ++j)
		{
			std::string edgeLabel = 'x' + std::to_string(i + 1);
			boost::add_edge(vertexMap[j], vertexMap[line[j]], { edgeLabel }, graph);
		}
	}

	return graph;
}

void CMooreTable::ToStream(std::ostream& stream) const
{
	for (size_t i = 0; i < m_outputStateMap.size(); ++i)
	{
		stream << 'y' + std::to_string(m_outputStateMap[i]) << ' ';
	}
	stream << std::endl;
	for (const auto& line : m_stateTable)
	{
		for (const size_t val : line)
		{
			stream << 'q' << val << ' ';
		}
		stream << std::endl;
	}
}
