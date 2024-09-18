#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <shared_mutex>

class Logger
{
public:
	/**
	 Constructor
	*/
	Logger();

	/**
	 Destructor
	*/
	~Logger();

	/**
	 * Writing logs to the file
	 *
	 * @param text
	*/
    void writeLogFile(std::string text);

	/**
	* Reading log file
	*/
	void readLogFile();

private:
    std::fstream log_file;

    std::shared_mutex log_shared_mutex;
};
