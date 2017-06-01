#ifndef _COMMAND_EXECUTOR_H_
#define _COMMAND_EXECUTOR_H_

#include <iostream>
#include "rapidjson/document.h"

class CommandExecutor
{
public:
    CommandExecutor() {};

    void ExecuteCommand(std::string ,std::string ,std::string);
private:
    static const std::string DefaultConfigName;
    static const std::string DefaultEventName;

    void Teardown(rapidjson::Document& params);
};

#endif //_COMMAND_EXECUTOR_H_
