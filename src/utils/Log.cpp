//***************************************************************************//
//                                                                           // 
// @File Name:     Log.cpp                                                   //
// @Author:        Vahid Farjood Chafi                                       //
// @Version:       0.0.1                                                     //
// @Date:          13th December 2021                                        //
// @Description:   Logs into file or console                                 //
//                                                                           // 
//***************************************************************************//

#include "Log.h"


vfc::Log* vfc::Log::m_Instance{nullptr};
std::mutex vfc::Log::m_log_mutex;

vfc::Log::Log()
{
    m_File.open("../log/LogFile.log", std::ofstream::out | std::ofstream::app);

    m_File << "-------------------------------------------------------------------------------------------------------------\n";
    m_File << "                                                NEW LOG                                                      \n";
    m_File << "-------------------------------------------------------------------------------------------------------------\n";

    configure();
}

vfc::Log::~Log()
{
   m_File.close();
}

vfc::Log* vfc::Log::getInstance()
{
   if (m_Instance == nullptr) 
   {
      m_Instance = new Log();
   }
   return m_Instance;
}

void vfc::Log::lock(std::string const& data)
{
   std::scoped_lock lock(m_log_mutex);
   if(m_LogType == FILE)
      logIntoFile(data);
   else if(m_LogType == CONSOLE)
      logOnConsole(data);
   else if(m_LogType == ALL)
   {
      logOnConsole(data);
      logIntoFile(data);
   }
}

void vfc::Log::logIntoFile(std::string const& data)
{
   m_File << "[" << m_FileLogNumber << "]" << " " << "[" << getCurrentTime() << "]"<< " " << data << std::endl;
   m_FileLogNumber++;
}

void vfc::Log::logOnConsole(std::string const& data)
{
   std::cout << "[" << m_ConsoleLogNumber << "]" << " " << "[" << getCurrentTime() << "]" << " " << data << std::endl;
   m_ConsoleLogNumber++;
}

std::string vfc::Log::getCurrentTime()
{
   std::string currTime;
   //Current date/time based on current time
   time_t now = time(0); 
   // Convert current time to string
   currTime.assign(ctime(&now));
   // Last charactor of currentTime is "\n", so remove it
   std::string currentTime = currTime.substr(0, currTime.size()-1);
   return currentTime;
}

void vfc::Log::configure(LogMode log_mode, LogLevel log_level, LogType log_type)
{
   m_LogMode = log_mode;
   m_LogLevel = log_level;
   m_LogType = log_type;
}

void vfc::Log::logMode(LogMode log_mode)
{
   m_LogMode = log_mode; 
}

void vfc::Log::logLevel(LogLevel logLevel)
{
   m_LogLevel = logLevel;
}

void vfc::Log::logType(LogType LogType)
{
   m_LogType = LogType;
}