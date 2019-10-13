#pragma once
#include "pch.h"

struct VertexProps
{
	std::string label;
};

struct EdgeProps
{
	std::string label;
};

typedef boost::adjacency_list<
	boost::vecS,
	boost::vecS,
	boost::directedS,
	VertexProps,
	EdgeProps>
	Graph;

class ITable
{
public:
	virtual ~ITable() = default;

	virtual Graph ToGraph() const = 0;
	virtual void ToStream(std::ostream& stream) const = 0;
};
