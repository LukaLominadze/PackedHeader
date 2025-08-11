#pragma once

#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

#include <functional>
#include <vector>

enum class PageType {
	MAIN = 0,
	DEFINES
};

class Application
{
public:
	Application();
	~Application();

	void Run();
	void ChangePage(PageType type);

	inline static bool IsRunning() { return s_running; }
	inline static void Close() { s_running = false; }
private:
	void OnMainPageRequest(std::string& request);
	void OnMainPageRender();
	void OnDefinesPageRequest(std::string& request);
	void OnDefinesPageRender();
private:
	PageType m_page;

	std::vector<std::string> m_defines, m_sourceFiles, m_headerFiles;

	static bool s_running;
};

