
#pragma once

#include "utils/Assert.h"
#include <sstream>
#include <ctime>
#include <cctype>   // for isspace
#include <iostream>
#include <algorithm>

namespace pg {

enum LogLevel {
    Inhibit = 0,
    Error,
    Warning,
    Info,
    Debug,
    Debug2,
    Debug3,
    Debug4,
    All
};

inline std::string LogLevelToString( LogLevel level ) {
    switch ( level ) {
        case LogLevel::Inhibit:   return "Inhibit";   break;
        case LogLevel::Error:     return "Error";     break;
        case LogLevel::Warning:   return "Warning";   break;
        case LogLevel::Info:      return "Info";      break;
        case LogLevel::Debug:     return "Debug";     break;
        case LogLevel::Debug2:    return "Debug2";    break;
        case LogLevel::Debug3:    return "Debug3";    break;
        case LogLevel::Debug4:    return "Debug4";    break;
        case LogLevel::All:       return "All";       break;
        default:                  return "All";
    }
}

inline LogLevel StringToLogLevel( const std::string& level ) {
    if ( level == "Inhibit" ) {
        return LogLevel::Inhibit;
    } else if ( level == "Error" ) {
        return LogLevel::Error;
    } else if ( level == "Warning" ) {
        return LogLevel::Warning;
    } else if ( level == "Info" ) {
        return LogLevel::Info;
    } else if ( level == "Debug" ) {
        return LogLevel::Debug;
    } else if ( level == "Debug2" ) {
        return LogLevel::Debug2;
    } else if ( level == "Debug3" ) {
        return LogLevel::Debug3;
    } else if ( level == "Debug4" ) {
        return LogLevel::Debug4;
    } else if ( level == "All" ) {
        return LogLevel::All;
    } else {
        return LogLevel::All;
    }
}

inline std::string NowTime() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%H:%M:%S",timeinfo);
    std::string str( buffer );
    return str;
}

class Log {

    public:
        Log() = default;
        ~Log() {
            os_ << std::endl;
            std::cerr << os_.str();
            std::cerr.flush();  //syncrohize underlying device
        }

        inline static LogLevel& ReportingLevel() {
            static LogLevel level{LogLevel::Debug4};
            return level;
        }

        inline std::ostringstream& get( LogLevel level = LogLevel::Info ) {
            //TO-DO: use std::put_time here to format the time output.
            os_ << " [" << NowTime() << " " << LogLevelToString( level ) << "] ";
            os_ << std::string( level < LogLevel::Debug ? 0 : level - LogLevel::Debug, '\t' );
            return os_;
        }

    private:
        std::ostringstream os_{};
};

}

#define LOG(level) \
if ( level > pg::Log::ReportingLevel() ) ; \
else pg::Log().get( level )

#define LOG_ERROR LOG(pg::LogLevel::Error)
#define LOG_WARNING LOG(pg::LogLevel::Warning)
#define LOG_INFO LOG(pg::LogLevel::Info)
#define LOG_DEBUG LOG(pg::LogLevel::Debug)
#define LOG_DEBUG2 LOG(pg::LogLevel::Debug2)
#define LOG_DEBUG3 LOG(pg::LogLevel::Debug3)
#define LOG_DEBUG4 LOG(pg::LogLevel::Debug4)
