#include "Logger.h"

Logger::Logger()
{
    log_file = std::fstream("log.txt", std::ios::in | std::ios::out);

    std::filesystem::permissions("log.txt",
        std::filesystem::perms::group_all | std::filesystem::perms::others_all,
        std::filesystem::perm_options::remove);

    if (!log_file)
        log_file = std::fstream("log.txt", std::ios::in | std::ios::out | std::ios::trunc);
}

Logger::~Logger()
{
    log_file.close();
}

void Logger::writeLogFile(std::string text)
{
    log_shared_mutex.lock();
    
    if (log_file)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(now);
        //std::cout << "Current Time and Date: " << ctime(&end_time) << std::endl;

        log_file.seekp(0, std::ios_base::end);
        log_file << text << std::endl;
    }
    else
    {
        std::cout << "Could not open file log.txt !" << std::endl;
    }

    log_shared_mutex.unlock();
}

void Logger::readLogFile()
{
    log_shared_mutex.lock();
    
    if (log_file)
    {
        log_file.seekp(0, std::ios_base::beg);
        while (!log_file.eof())
        {
            std::string text;

            getline(log_file, text);
            std::cout << text << std::endl;
        }
    }

    log_shared_mutex.unlock();
}
