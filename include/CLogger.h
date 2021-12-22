#pragma once
#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>
#include <iostream>
#include <string>
#include <time.h>

// Singleton Logger Class.

class CLogger
{
public:
    void Log(const std::string& );
    void closeFile();

    void write(std::string message, ...);

    CLogger& operator<<(const std::string& );
    static CLogger* GetLogger();

private:
    // Default constructor for the Logger class.
    CLogger();
    ~CLogger();
    std::string currentDateTime();

    CLogger(const CLogger&) {}; // copy constructor is private
    CLogger& operator=(const CLogger&) { return *this; };  // operator is private

    static const std::string m_fileName;

    //Singleton logger class object pointer.
    static CLogger* m_singleton;
    static std::ofstream m_Logfile;
};
