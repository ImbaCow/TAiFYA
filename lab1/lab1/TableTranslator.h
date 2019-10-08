#pragma once
#include "pch.h"
#include "CMealyTable.h"
#include "CMooreTable.h"

std::shared_ptr<CMooreTable> FromMealyToMoore(const CMealyTable& table);
std::shared_ptr<CMealyTable> FromMooreToMealy(const CMooreTable& table);