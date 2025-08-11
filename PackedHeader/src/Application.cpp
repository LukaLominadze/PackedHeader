#include "Application.h"
#include <iostream>
#include "utils/ConsoleUtils.h"

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
		}
		switch (m_page)
		{
		case PageType::MAIN: OnMainPageRender(); break;
		case PageType::DEFINES: OnDefinesPageRender(); break;
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

	spdlog::info("7. Quit");
}

void Application::OnDefinesPageRequest(std::string& request)
{
	if (request == "0") {
		m_page = PageType::MAIN;
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

	spdlog::info("0. Back");

	spdlog::info("Defines: {0}", defines);
}

