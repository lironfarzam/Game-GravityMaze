#include "CLogger.h"
#include <iostream>
#include <stdarg.h>

const std::string CLogger::m_fileName = "log.txt";

CLogger* CLogger::m_singleton = NULL;
std::ofstream CLogger::m_Logfile;

CLogger::CLogger()
{
    m_Logfile.clear();
}

CLogger::~CLogger()
{
    if (m_singleton != NULL)
        closeFile();
}

void CLogger::closeFile() {
    m_Logfile.close();
}

CLogger* CLogger::GetLogger() {
    if (m_singleton == NULL) {
        m_singleton = new CLogger();
        try {
            m_Logfile.open(m_fileName.c_str(), std::ios_base::app /*std::ios_base::trunc*/ /*std::ios::out | std::ios::app*/);
        } catch (std::ifstream::failure e) {
            // still ok but no logging possible
        }
    }
    return m_singleton;
}


void CLogger::Log(const std::string& msg)
{
    std::cout << msg << std::endl;

    if(m_Logfile.is_open()) {
        m_Logfile << "\n" << currentDateTime() << ":\t" << msg << "\n";
    }
}

CLogger& CLogger::operator<<(const std::string& msg)
{
    if(m_Logfile.is_open()) {
        m_Logfile << "\n" << currentDateTime() << ":\t";
        m_Logfile << msg << "\n";
    }
    return *this;
}

std::string CLogger::currentDateTime()
{
    return "";
}

void CLogger::write(std::string message, ...) {
    va_list args;
    va_start(args, message);
    char buffer[1024];
    vsprintf(buffer, message.c_str(), args);

    if(m_Logfile.is_open())
        {
            m_Logfile << "\n" << currentDateTime() << ":\t";
            m_Logfile << buffer << "\n";
            m_Logfile.flush();
        }

    std::cout << buffer << std::endl;

    va_end(args);
}
