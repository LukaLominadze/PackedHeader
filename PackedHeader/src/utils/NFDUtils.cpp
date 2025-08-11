#include "NFDUtils.h"
#include "StringUtils.h"

#include "nfd.h"

namespace Utils
{
	std::filesystem::path OpenFileDialog(const char* filterList, const char* defaultPath)
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog(filterList, defaultPath, &outPath);
		if (result == NFD_OKAY)
		{
			std::filesystem::path result(outPath);
			auto tokens = Utils::SplitString(result.string(), "/");
			if (tokens.size() == 1) {
				tokens = Utils::SplitString(result.string(), "\\");
			}
			auto fileTokens = Utils::SplitString(tokens.at(tokens.size() - 1), ".");
			if (fileTokens.size() < 2) {
				return std::filesystem::path{};
			}
			auto filterTokens = Utils::SplitString(filterList, ";");
			for (auto filter : filterTokens) {
				if (fileTokens[1] == filter) {
					free(outPath);
					return result;
				}
			}
			return std::filesystem::path{};
		}
		else if (result == NFD_CANCEL)
		{
			return std::filesystem::path{};
		}
		else
		{
			return std::filesystem::path{};
		}
		return std::filesystem::path{};
	}

	std::filesystem::path OpenFolderDialog(const char* defaultPath)
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_PickFolder(defaultPath, &outPath);
		if (result == NFD_OKAY)
		{
			std::filesystem::path result(outPath);
			free(outPath);
			if (result.empty()) {
				return std::filesystem::path{};
			}
			return result;
		}
		else if (result == NFD_CANCEL)
		{
			return std::filesystem::path{};
		}
		else
		{
			return std::filesystem::path{};
		}
		return std::filesystem::path{};
	}

	std::filesystem::path OpenSaveDialog(const char* filterList, const char* defaultPath)
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_SaveDialog(filterList, defaultPath, &outPath);
		if (result == NFD_OKAY)
		{
			std::filesystem::path result(outPath);
			free(outPath);
			if (result.empty()) {
				return std::filesystem::path{};
			}
			return result;
		}
		else if (result == NFD_CANCEL)
		{
			return std::filesystem::path{};
		}
		else
		{
			return std::filesystem::path{};
		}
		return std::filesystem::path{};
	}
}