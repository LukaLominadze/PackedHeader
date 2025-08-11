#include "Application.h"
#include <iostream>
#include "utils/ConsoleUtils.h"
#include "utils/StringUtils.h"
#include "utils/NFDUtils.h"

bool Application::s_running = false;

Application::Application()
	:m_page(PageType::MAIN)
{
	spdlog::set_pattern("%^[%L] %v%$");
}

Application::~Application()
{
}

void Application::Run()
{
	s_running = true;
	std::string request;

	OnMainPageRender();
	while (s_running) {
		std::getline(std::cin, request);

		Utils::ClearConsole();
		switch (m_page)
		{
		case PageType::MAIN: OnMainPageRequest(request); break;
		case PageType::DEFINES: OnDefinesPageRequest(request); break;
		case PageType::SOURCE_FILES: OnSourceFilesPageRequest(request); break;
		case PageType::INCLUDE_FILES: OnIncludeFilesPageRequest(request); break;
		}
		switch (m_page)
		{
		case PageType::MAIN: OnMainPageRender(); break;
		case PageType::DEFINES: OnDefinesPageRender(); break;
		case PageType::SOURCE_FILES: OnSourceFilesPageRender(); break;
		case PageType::INCLUDE_FILES: OnIncludeFilesPageRender(); break;
		}
	}
}

void Application::ChangePage(PageType type)
{
	m_page = type;
}

void Application::OnMainPageRequest(std::string& request)
{
	if (request == "1") {
		m_page = PageType::DEFINES;
	}
	else if (request == "2") {
		m_page = PageType::SOURCE_FILES;
	}
	else if (request == "3") {
		m_page = PageType::INCLUDE_FILES;
	}
	else if (request == "7") {
		Close();
	}
}

void Application::OnMainPageRender()
{
	spdlog::info("Welcome to PackedHeader!\n");

	spdlog::info("1. Defines");
	spdlog::info("2. Source Files");
	spdlog::info("3. Include Files\n");

	spdlog::info("4. Load Configuration");
	spdlog::info("5. Save Configuration\n");

	spdlog::info("6. Generate Header\n");

	spdlog::warn("7. Quit");
}

void Application::OnDefinesPageRequest(std::string& request)
{
	if (request == "0") {
		m_page = PageType::MAIN;
	}

	if (request.find_first_of(' ') == std::string::npos) {
		return;
	}
	std::vector<std::string> tokens = Utils::SplitString(request, ' ');
	if (tokens[0] == "Add") {
		auto it = std::find(m_defines.begin(), m_defines.end(), tokens[1]);
		if (it == m_defines.end()) {
			m_defines.push_back(tokens[1]);
		}
	}
	else if (tokens[0] == "Remove") {
		auto it = std::find(m_defines.begin(), m_defines.end(), tokens[1]);
		if (it != m_defines.end()) {
			m_defines.erase(it);
		}
	}
}

void Application::OnDefinesPageRender()
{
	std::string defines;
	for (const std::string& define : m_defines) {
		defines += define;
		defines += ", ";
	}
	if (!defines.empty()) {
		defines.pop_back();
		defines.pop_back();
	}

	spdlog::info("0. Back\n");

	spdlog::info("Defines: {0}\n", defines);
	spdlog::info("Add [DEFINE] (For Ex. Add MY_DEFINE)");
	spdlog::info("Remove [DEFINE] (For Ex. Remove MY_DEFIE)");
}

void Application::OnSourceFilesPageRequest(std::string& request)
{
	if (request == "0") {
		m_page = PageType::MAIN;
	}

	if (request == "1") {
		std::filesystem::path filePath = Utils::OpenFileDialog("cpp", std::filesystem::current_path().string().c_str());
		filePath = std::filesystem::relative(filePath);
		auto it = std::find(m_sourceFiles.begin(), m_sourceFiles.end(), filePath.string());
		if (it == m_sourceFiles.end()) {
			m_sourceFiles.push_back(filePath.string());
		}
	}
	else if (request == "2") {
		std::filesystem::path directoryPath = Utils::OpenFolderDialog(std::filesystem::current_path().string().c_str());
		if (directoryPath.empty()) {
			return;
		}
		for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
			if (!entry.is_directory()) {
				std::filesystem::path entryRelative = std::filesystem::relative(entry);
				std::string ext = entryRelative.extension().string();
				if (ext != ".cpp") {
					continue;
				}
				auto it = std::find(m_sourceFiles.begin(), m_sourceFiles.end(), entryRelative.string());
				if (it == m_sourceFiles.end()) {
					m_sourceFiles.push_back(entryRelative.string());
				}
			}
		}
	}
	else if (request == "3") {
		std::filesystem::path directoryPath = Utils::OpenFolderDialog(std::filesystem::current_path().string().c_str());
		if (directoryPath.empty()) {
			return;
		}
		for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
			if (!entry.is_directory()) {
				std::filesystem::path entryRelative = std::filesystem::relative(entry);
				std::string ext = entryRelative.extension().string();
				if (ext != ".cpp") {
					continue;
				}
				auto it = std::find(m_sourceFiles.begin(), m_sourceFiles.end(), entryRelative.string());
				if (it == m_sourceFiles.end()) {
					m_sourceFiles.push_back(entryRelative.string());
				}
			}
		}
	}

	if (request.find_first_of(' ') == std::string::npos) {
		return;
	}
	std::vector<std::string> tokens = Utils::SplitString(request, ' ');
	if (tokens[0] == "Remove") {
		auto it = std::find(m_sourceFiles.begin(), m_sourceFiles.end(), tokens[1]);
		if (it != m_sourceFiles.end()) {
			m_sourceFiles.erase(it);
		}
	}
}

void Application::OnSourceFilesPageRender()
{
	std::string sourceFiles;
	for (const std::string& source : m_sourceFiles) {
		sourceFiles += source;
		sourceFiles += ", ";
	}
	if (!sourceFiles.empty()) {
		sourceFiles.pop_back();
		sourceFiles.pop_back();
	}

	spdlog::info("0. Back\n");

	spdlog::info("Source Files: {0}\n", sourceFiles);

	spdlog::info("1. Add File");
	spdlog::info("2. Add Directory");
	spdlog::info("3. Add Directory Recursively");

	spdlog::info("4. Remove [FILENAME] (Ex. Remove Sample.cpp)");
}

void Application::OnIncludeFilesPageRequest(std::string& request)
{
	if (request == "0") {
		m_page = PageType::MAIN;
	}

	if (request == "1") {
		std::filesystem::path filePath = Utils::OpenFileDialog("h;hpp", std::filesystem::current_path().string().c_str());
		filePath = std::filesystem::relative(filePath);
		auto it = std::find(m_headerFiles.begin(), m_headerFiles.end(), filePath.string());
		if (it == m_headerFiles.end()) {
			m_headerFiles.push_back(filePath.string());
		}
	}
	else if (request == "2") {
		std::filesystem::path directoryPath = Utils::OpenFolderDialog(std::filesystem::current_path().string().c_str());
		if (directoryPath.empty()) {
			return;
		}
		for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
			if (!entry.is_directory()) {
				std::filesystem::path entryRelative = std::filesystem::relative(entry);
				std::string ext = entryRelative.extension().string();
				if (ext != ".h" && ext != ".hpp") {
					continue;
				}
				auto it = std::find(m_headerFiles.begin(), m_headerFiles.end(), entryRelative.string());
				if (it == m_headerFiles.end()) {
					m_headerFiles.push_back(entryRelative.string());
				}
			}
		}
	}
	else if (request == "3") {
		std::filesystem::path directoryPath = Utils::OpenFolderDialog(std::filesystem::current_path().string().c_str());
		if (directoryPath.empty()) {
			return;
		}
		for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
			if (!entry.is_directory()) {
				std::filesystem::path entryRelative = std::filesystem::relative(entry);
				std::string ext = entryRelative.extension().string();
				if (ext != ".h" && ext != ".hpp") {
					continue;
				}
				auto it = std::find(m_headerFiles.begin(), m_headerFiles.end(), entryRelative.string());
				if (it == m_headerFiles.end()) {
					m_headerFiles.push_back(entryRelative.string());
				}
			}
		}
	}

	if (request.find_first_of(' ') == std::string::npos) {
		return;
	}
	std::vector<std::string> tokens = Utils::SplitString(request, ' ');
	if (tokens[0] == "Remove") {
		auto it = std::find(m_headerFiles.begin(), m_headerFiles.end(), tokens[1]);
		if (it != m_headerFiles.end()) {
			m_headerFiles.erase(it);
		}
	}
}

void Application::OnIncludeFilesPageRender()
{
	std::string includeFiles;
	for (const std::string& include : m_headerFiles) {
		includeFiles += include;
		includeFiles += ", ";
	}
	if (!includeFiles.empty()) {
		includeFiles.pop_back();
		includeFiles.pop_back();
	}

	spdlog::info("0. Back\n");

	spdlog::info("Include Files: {0}\n", includeFiles);

	spdlog::info("1. Add File");
	spdlog::info("2. Add Directory");
	spdlog::info("3. Add Directory Recursively");

	spdlog::info("4. Remove [FILENAME] (Ex. Remove Sample.h)");
}

