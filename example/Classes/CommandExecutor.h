#ifndef _COMMAND_EXECUTOR_H_
#define _COMMAND_EXECUTOR_H_

#include <map>
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
    std::string basePath;
    std::map<std::string, void*> savedInstances;

    void Factory(rapidjson::Document& params);
    void Config(rapidjson::Document& params);
    void Start(rapidjson::Document& params);
    void Event(rapidjson::Document& params);
    void TrackEvent(rapidjson::Document& params);
    void SetReferrer(rapidjson::Document& params);
    void Pause(rapidjson::Document& params);
    void Resume(rapidjson::Document& params);
    void SetEnabled(rapidjson::Document& params);
    void SetOfflineMode(rapidjson::Document& params);
    void SendFirstPackages(rapidjson::Document& params);
    void AddSessionCallbackParameter(rapidjson::Document& params);
    void AddSessionPartnerParameter(rapidjson::Document& params);
    void RemoveSessionCallbackParameter(rapidjson::Document& params);
    void RemoveSessionPartnerParameter(rapidjson::Document& params);
    void ResetSessionCallbackParameters(rapidjson::Document& params);
    void ResetSessionPartnerParameters(rapidjson::Document& params);
    void SetPushToken(rapidjson::Document& params);
    void Teardown(rapidjson::Document& params);
    void OpenDeeplink(rapidjson::Document& params);
    void SendReferrer(rapidjson::Document& params);
    void TestBegin(rapidjson::Document& params);
    void TestEnd(rapidjson::Document& params);
    bool ToBool(std::string const& s);
    rapidjson::Value& GetFirstParameterValue(rapidjson::Document& params, std::string const& key);
};

#endif //_COMMAND_EXECUTOR_H_
