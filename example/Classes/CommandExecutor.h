#ifndef _COMMAND_EXECUTOR_H_
#define _COMMAND_EXECUTOR_H_

#include <map>
#include <iostream>
#include "rapidjson/document.h"
#include "Adjust/AdjustConfig2dx.h"
#include "Adjust/AdjustEvent2dx.h"

class CommandExecutor
{
public:
    CommandExecutor() {};

    void ExecuteCommand(std::string ,std::string ,std::string);
private:
    std::string basePath;

    void Factory(rapidjson::Document& params);
    AdjustConfig2dx* Config(rapidjson::Document& params);
    void Start(rapidjson::Document& params);
    AdjustEvent2dx* Event(rapidjson::Document& params);
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
    double ToDouble(std::string const& s);
    std::string GetValue(int idx, rapidjson::Document& params, std::string const& key);
};

#endif //_COMMAND_EXECUTOR_H_
