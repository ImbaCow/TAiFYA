#include "pch.h"
#include "CMealyTable.h"
#include "CMooreTable.h"

using namespace std;

void PrintGraph(ostream& stream, Graph graph)
{
	boost::dynamic_properties dp;
	dp.property("label", boost::get(&VertexProps::label, graph));
	dp.property("label", boost::get(&EdgeProps::label, graph));
	dp.property("node_id", boost::get(boost::vertex_index, graph));

	boost::write_graphviz_dp(stream, graph, dp);
}

std::shared_ptr<ITable> GetMinimizeTableTable(std::istream& stream)
{
	std::string tempLine;
	std::getline(stream, tempLine);
	std::getline(stream, tempLine);
	std::getline(stream, tempLine);

	std::string tableType;
	std::getline(stream, tableType);

	if (tableType == "mealy")
	{
		std::shared_ptr<CMealyTable> mealyTable = std::make_shared<CMealyTable>(stream);
		mealyTable->Minimize();
		return mealyTable;
	}
	else if (tableType == "moore")
	{
		std::shared_ptr<CMooreTable> mooreTable = std::make_shared<CMooreTable>(stream);
		mooreTable->Minimize();
		return mooreTable;
	}
	return nullptr;
}

void HandleInput(const std::string& inputName, const std::string& outputName)
{
	std::ifstream input(inputName);
	std::ofstream output(outputName);
	std::ofstream graphOutput("graph.dot");

	std::shared_ptr<ITable> tablePtr = GetMinimizeTableTable(input);
	if (!tablePtr)
	{
		return;
	}

	tablePtr->ToStream(output);
	PrintGraph(graphOutput, tablePtr->ToGraph());
}

int main(int argc, char* argv[])
{
	HandleInput(argv[1], argv[2]);
}
