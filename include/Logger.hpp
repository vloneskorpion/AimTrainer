#pragma once

#include "easylogging/easylogging++.h"
INITIALIZE_EASYLOGGINGPP

class Logger
{
    public:
        Logger(const std::string& cofigurationPath) :
            m_loggerConfiguration{cofigurationPath}
        {
            el::Loggers::reconfigureAllLoggers(m_loggerConfiguration);
        }
    
    private:
        el::Configurations m_loggerConfiguration;
};