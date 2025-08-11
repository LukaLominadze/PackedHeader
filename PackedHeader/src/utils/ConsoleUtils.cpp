#include "ConsoleUtils.h"
#include <iostream>
#if PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace Utils
{
	#if PLATFORM_WINDOWS
	void ClearConsole() {

        char fill = ' ';

        COORD tl = { 0,0 };
        CONSOLE_SCREEN_BUFFER_INFO s;
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

        GetConsoleScreenBufferInfo(console, &s);

        DWORD written, cells = s.dwSize.X * s.dwSize.Y;

        FillConsoleOutputCharacter(console, fill, cells, tl, &written);
        FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
        SetConsoleCursorPosition(console, tl);
	}
	#elif PLATFORM_LINUX
    void ClearConsole() {
        std::cout << "\033[2J\033[1;1H";
    }
	#endif
}