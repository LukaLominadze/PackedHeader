#pragma once

#include <vector>
#include <string>

namespace Utils
{
	// From Walnut - https://github.com/StudioCherno/Walnut
	std::vector<std::string> SplitString(const std::string_view string, const std::string_view& delimiters);
	std::vector<std::string> SplitString(const std::string_view string, const char delimiter);
}