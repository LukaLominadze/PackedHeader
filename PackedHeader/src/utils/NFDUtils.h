#pragma once

#include "filesystem"
#include "fstream"

namespace Utils
{
	std::filesystem::path OpenFileDialog(const char* filterList, const char* defaultPath = NULL);
	std::filesystem::path OpenFolderDialog(const char* defaultPath = NULL);
	std::filesystem::path OpenSaveDialog(const char* filterList, const char* defaultPath = NULL);
}