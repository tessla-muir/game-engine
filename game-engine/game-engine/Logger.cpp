#include "Logger.h"
#include <string>
#include <iostream>
#include <ctime>
#include <chrono>

std::vector<LogEntry> Logger::messages;

std::string GetTimeString() {
	// Get time point
	auto timePoint = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(timePoint);

	// Format time point
	std::string timeString(30, '\0');
	struct tm timeInfo;
	localtime_s(&timeInfo, &time);
	std::strftime(&timeString[0], timeString.size(), "%H:%M:%S", &timeInfo);

	return timeString;
}

void Logger::Log(const std::string& msg) {
	// Create and add log entry
	LogEntry log;
	log.type = LOG_INFO;
	log.message = "LOG   [" + GetTimeString() + "] " + msg;
	messages.push_back(log);

	// Print to console
	std::cout << "\033[32m" << log.message << "\033[0m" << std::endl;
}

void Logger::Warn(const std::string& msg) {
	// Create and add log entry
	LogEntry log;
	log.type = LOG_WARNING;
	log.message = "WARN  [" + GetTimeString() + "] " + msg;
	messages.push_back(log);

	// Print to console
	std::cout << "\033[33m" << log.message << "\033[0m" << std::endl;
}

void Logger::Error(const std::string& msg) {
	// Create and add log entry
	LogEntry log;
	log.type = LOG_ERROR;
	log.message = "ERROR [" + GetTimeString() + "] " + msg;
	messages.push_back(log);

	// Print to console
	std::cout << "\033[91m" << log.message << "\033[0m" << std::endl;
}