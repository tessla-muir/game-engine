#include "Logger.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <ctime>

std::vector<LogEntry> Logger::messages;

std::string GetTimeString() {
	// Get time point
	auto timePoint = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(timePoint);

	// Format time point
	std::tm time_info;
	localtime_s(&time_info, &time);
	char time_str[9]; // HH:MM:SS\0
	std::strftime(time_str, sizeof(time_str), "%H:%M:%S", &time_info);
	return std::string(time_str);
}

void Logger::Log(const std::string& msg) {
	// Create and add log entry
	LogEntry log;
	log.type = LOG_INFO;
	log.message = "LOG   [" + GetTimeString() + "] " + msg;
	messages.push_back(log);

	// Print to console
	std::cout << log.message << std::endl;
}

void Logger::Debug(const std::string& msg) {
	// Create and add log entry
	LogEntry log;
	log.type = LOG_INFO;
	log.message = "DEBUG [" + GetTimeString() + "] " + msg;
	messages.push_back(log);

	// Print to console
	std::cout << "\033[36m" << log.message << "\033[0m" << std::endl;
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