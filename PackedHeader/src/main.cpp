#include <iostream>

#include <spdlog/spdlog.h>

int main() {
	spdlog::set_pattern("%^[%H:%M:%S %z] [%n] [---%L---] [thread %t] %v%$");

	spdlog::info("This is a message");
	spdlog::warn("This is a test application");
	spdlog::error("An error!");
}