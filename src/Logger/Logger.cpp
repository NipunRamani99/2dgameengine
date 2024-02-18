#include "Logger.h"
#include <ctime>    
#include <chrono>
#include <iostream>
#include <iomanip>

std::vector<LogEntry> Logger::messages;

void Logger::Log(const std::string& string) {
	//TODO: Implement Log
    LogEntry entry;
    std::string currentDateTime = getCurrentDateTime();
    entry.message = "LOG: [" + currentDateTime + "] - " + string;
    entry.type = LOG_INFO;
    std::cout << "\x1B[32m"  << "LOG: [" << currentDateTime << "] - " << string << "\033[0m" << std::endl;
    messages.push_back(entry);
}

void Logger::Err(const std::string& string) {
	//TODO: Implement Err
    LogEntry entry;
    std::string currentDateTime = getCurrentDateTime();
    entry.message = "ERR: [" + currentDateTime + "] - " + string;
    entry.type = LOG_ERROR;
    std::cout << "\x1B[91m" << "ERR: [" << currentDateTime << "] - " << string << "\033[0m" << std::endl;
    messages.push_back(entry);
}

std::string Logger::getCurrentDateTime()
{
    // Get the current time point
    auto currentTime = std::chrono::system_clock::now();

    // Convert the time point to a time_t object
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    // Convert the time_t object to a struct tm
    struct tm localTime;
    localtime_s(&localTime, &currentTime_t);

    // Format and print the date and time
    char buffer[80];  // Assuming 80 characters is enough for the formatted string
    std::strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M:%S", &localTime);
    return std::string(buffer);
}
